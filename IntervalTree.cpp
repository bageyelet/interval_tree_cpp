#include <iostream>
#include "IntervalTree.hpp"

Node::Node(Interval& i) {
    this->i           = &i;
    this->parent      = nullptr;
    this->left_child  = nullptr;
    this->right_child = nullptr;
    this->max         = i.end;
    this->left_depth  = 0;
    this->right_depth = 0;
    this->it          = nullptr;
}

Node::Node(Interval& i, int max) : Node(i) {
    this->max = max;
}

Node::~Node() {
    delete this->i;
}

void  Node::add(Interval& i) {
    if (this->max < i.end)
        this->max = i.end;
    if (i.begin >= this->i->begin) {
        if (this->right_child == nullptr) {
            this->right_child = new Node(i);
            this->right_child->parent = this;
            this->right_child->rebalance();
        } else {
            this->right_child->add(i);
        }
    } else {
        if (this->left_child == nullptr) {
            this->left_child = new Node(i);
            this->left_child->parent  = this;
            this->left_child->rebalance();
        } else {
            this->left_child->add(i);
        }
    }
}

void  Node::search(const Interval& i, std::vector<Interval*>& ris) {
    if (i.overlaps(*(this->i)))
        ris.push_back(this->i);
    if (this->left_child != nullptr && this->left_child->max >= i.begin)
        this->left_child->search(i, ris);
    if (this->right_child != nullptr && this->i->begin <= i.end && this->right_child->max >= i.begin)
        this->right_child->search(i, ris);
}

Node& Node::copy() const {
    Node* new_node = new Node(this->i->copy());
    new_node->parent      = this->parent;
    new_node->left_child  = this->left_child;
    new_node->right_child = this->right_child;
    new_node->max         = this->max;
    new_node->left_depth  = this->left_depth;
    new_node->right_depth = this->right_depth;
    new_node->it          = this->it;
    return *new_node;
}

void Node::rotation_right() {
   /*

        this               z
       /    \             / \
      z      &    -->    %  this
     / \                   /    \
    %   t                 t      &

    */
    if (this->left_child == nullptr) {
        std::cerr << "Node::rotation_right(): left_child of this is null\n";
        throw 1;
    }

    Node* z = this->left_child;
    Node* t = z->right_child;  // it can be null

    if (this->it == nullptr) { // the node is not the root, I have to set the correct parent of z and this
        if (this->parent->left_child == this)
            this->parent->left_child  = z;
        else if (this->parent->right_child == this)
            this->parent->right_child = z;
        else {
            std::cerr << "Node::rotation_right(): this is malformed, wrong parent pointer\n";
            throw 2;
        }
    } else { // the node is the root of the tree
        this->it->root = z;
        z->it = this->it;
        this->it = nullptr;
    }
    z->parent = this->parent; // if this was the root, then z->parent will be NULL
    this->parent = z;

    z->right_child = this;
    this->left_child = t;
    if (t != nullptr)
        t->parent = this;

    // set correct depth and max
    this->left_depth = z->right_depth;
    z->right_depth = 1 + MAX(this->left_depth, this->right_depth);

    int m = this->i->end;
    if (this->left_child  != nullptr) m = MAX(this->left_child->max,  m);
    if (this->right_child != nullptr) m = MAX(this->right_child->max, m);
    this->max = m;

    m = MAX(this->max, z->i->end);
    if (z->left_child != nullptr) m = MAX(z->left_child->max, m);
    z->max = m;
}

void Node::rotation_left()  {
   /*

      this                   z
     /    \                 / \
    %     z     -->     this   &
         / \           /   \
        t   &         %     t

    */
    if (this->right_child == nullptr) {
        std::cerr << "Node::rotation_left(): right_child of this is null\n";
        throw 4;
    }

    Node* z = this->right_child;
    Node* t = z->left_child;  // it can be null

    if (this->it == nullptr) { // the node is not the root, I have to set the correct parent of z and this
        if (this->parent->left_child == this)
            this->parent->left_child  = z;
        else if (this->parent->right_child == this)
            this->parent->right_child = z;
        else {
            std::cerr << "Node::rotation_left(): this is malformed, wrong parent pointer\n";
            throw 5;
        }
    } else { // the node is the root of the tree
        this->it->root = z;
        z->it = this->it;
        this->it = nullptr;
    }
    z->parent = this->parent; // if this was the root, then z->parent will be NULL
    this->parent = z;

    z->left_child = this;
    this->right_child = t;
    if (t != nullptr)
        t->parent = this;

    // set correct depth and max
    this->right_depth = z->left_depth;
    z->left_depth = 1 + MAX(this->left_depth, this->right_depth);

    int m = this->i->end;
    if (this->left_child  != nullptr) m = MAX(this->left_child->max,  m);
    if (this->right_child != nullptr) m = MAX(this->right_child->max, m);
    this->max = m;

    m = MAX(this->max, z->i->end);
    if (z->right_child != nullptr) m = MAX(z->left_child->max, m);
    z->max = m;
}

void Node::rebalance() {
    Node* p = this->parent;
    if (p == nullptr) // this is the root
        return;

    if (this == p->left_child)
        p->left_depth  = 1 + MAX(this->left_depth, this->right_depth);
    else if (this == p->right_child)
        p->right_depth = 1 + MAX(this->left_depth, this->right_depth);
    else {
        std::cerr << "Node::rebalance(): this is malformed, wrong parent pointer\n";
        throw 3;
    }

    if (p->balancing_factor() <= -2)
        if (p->left_child->right_depth <= p->left_child->right_depth)  // p must have a left_child since has a negative BF
            p->rotation_right();
        else {
            p->left_child->rotation_left();
            p->rotation_right();
        } 
    else if (p->balancing_factor() >= 2)
        if (p->right_child->left_depth <= p->right_child->right_depth) // p must have a right_child since has a positive BF
            p->rotation_left();
        else {
            p->right_child->rotation_right();
            p->rotation_left();
        }
    else
        p->rebalance();
}

IntervalTree::IntervalTree() {
    this->root = nullptr;
    this->n    = 0;
}

void recursive_delete(Node* n) {
    if (n->right_child != nullptr)
        recursive_delete(n->right_child);
    if (n->left_child  != nullptr)
        recursive_delete(n->left_child );
    delete n;
}

IntervalTree::~IntervalTree() {
    if (this->root != nullptr)
        recursive_delete(this->root);
}

void IntervalTree::add(Interval& i) { // The interval is not copied, must be on heap!
    if (this->root == nullptr) {
        this->root     = new Node(i);
        this->root->it = this;
        this->n       += 1;
    } else {
        this->root->add(i);
    }
}

void IntervalTree::add(int begin, int end) {
    Interval* i = new Interval(begin, end);
    this->add(*i);
}

std::vector<Interval*>& IntervalTree::search(int begin, int end) const {
    std::vector<Interval*>* ris = new std::vector<Interval*>();
    if (this->root == nullptr) {
        return *ris;
    }
    Interval* i = new Interval(begin, end);
    this->root->search(*i, *ris);
    delete i;
    return *ris;
}

Node* recursive_copy(Node* old_node, Node* parent) {
    Node* new_node = new Node(old_node->i->copy());
    new_node->parent      = parent;
    new_node->max         = old_node->max;
    new_node->left_depth  = old_node->left_depth;
    new_node->right_depth = old_node->right_depth;
    new_node->it          = nullptr;

    if (old_node->left_child  != nullptr)
        new_node->left_child   = recursive_copy(old_node->left_child,  new_node);
    if (old_node->right_child != nullptr)
        new_node->right_child  = recursive_copy(old_node->right_child, new_node);

    return new_node;
}

IntervalTree& IntervalTree::copy() const {
    IntervalTree* new_it = new IntervalTree();

    if (this->root != nullptr) {
        new_it->root = recursive_copy(this->root, nullptr);
        new_it->root->it = new_it;
    }
    return *new_it;
}

void dumpR(Node* n, int tabs) {
    for (int i=0; i<tabs; ++i) std::cout << "    ";
    std::cout << *(n->i) << " p: ";
    if (n->parent != nullptr) std::cout << *(n->parent->i);
    else std::cout << "NULL";
    std::cout << " m: " << n->max << " bf: " << n->balancing_factor() << "\n";

    if (n->left_child  != nullptr) {
        for (int i=0; i<tabs; ++i) std::cout << "    ";
        std::cout << " Left:\n";
        dumpR(n->left_child,  tabs+1);
    }
    if (n->right_child != nullptr) {
        for (int i=0; i<tabs; ++i) std::cout << "    ";
        std::cout << " Right:\n";
        dumpR(n->right_child, tabs+1);
    }
}

void IntervalTree::dump() {
    std::cout << "*** INTERVAL TREE ***\n";
    if (this->root == nullptr)
        std::cout << "empty tree\n";
    else {
        dumpR(this->root, 0);
    }
}