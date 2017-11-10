#include "PiTree.hpp"
#include <cassert>

// ********************** Page **********************

Page::Page(int begin, int end) {
    assert(begin <= end);

    this->it       = new IntervalTree();
    this->begin    = begin;
    this->end      = end;
    this->lazycopy = false;
}

Page::Page(int begin, int end, IntervalTree* it) {
    assert(begin <= end);

    this->it       = it;
    this->begin    = begin;
    this->end      = end;
    this->lazycopy = false;
}

Page::~Page() { // I'm going to delete the tree only if is not shared with someone else
    if (this->it->general_purpose == 0)
        delete this->it;
    else
        this->it->general_purpose -= 1;
}

void Page::copy_on_write() {
    if (this->lazycopy) {
        this->lazycopy = false;
        if (this->it->general_purpose > 0) { // general purpose is used
                                             // to check if the tree is
                                             // actually shared or not!
            this->it->general_purpose -= 1;
            this->it = &it->copy(); // do a deep copy!
        }
    }
}

void Page::add(int begin, int end) {
    assert(begin <= end);

    this->copy_on_write();
    it->add(begin, end);
}

void Page::add(int begin, int end, Data* data) {
    if (data == nullptr) {
        this->add(begin, end);
        return;
    }

    assert(begin <= end);
    this->copy_on_write();
    Interval* new_interval = new Interval(begin, end, data);
    it->add(*new_interval);
}

bool Page::operator==(const Data& that) const {
    const Page* p = dynamic_cast<const Page*> (&that);

    if (! p) return false;
    return this == p;
}

std::ostream& Page::print(std::ostream& o) const {
    // return o << this << " -> Page(" << this->begin << ", " << this->end << "): it_addr:" << it << " it->root_addr:" << it->root << "\n" << *it << "\n";
    return o << "page at: " << this << " with it at: " << this->it;
}

Data& Page::copy() {
    this->lazycopy = true;
    
    Page* page     = new Page(this->begin, this->end, this->it);
    page->lazycopy = true;
    this->it->general_purpose += 1;
    return *page;
}

// ********************** PiTree **********************

PiTree::PiTree(int page_size) { // see header for constructor without page_size
    this->pages     = new IntervalTree();
    this->lazycopy  = false;
    this->page_size = page_size;
}

PiTree::PiTree(int page_size, IntervalTree* it) {
    this->pages     = it;
    this->lazycopy  = false;
    this->page_size = page_size;
}

PiTree::~PiTree() { // I'm going to delete the tree only if is not shared with someone else
    if (this->pages->general_purpose == 0)
        delete this->pages;
    else
        this->pages->general_purpose -= 1;
}

void PiTree::copy_on_write() {
    if (this->lazycopy) {
        this->lazycopy = false;
        if (this->pages->general_purpose > 0) { // general purpose is used
                                                // to check if the tree is
                                                // actually shared or not!
            this->pages->general_purpose -= 1;
            this->pages = &pages->copy(); // do a deep copy of the outer tree, the inner trees are not copied! (for each node Page::copy() will be called)
        }
    }
}

PiTree& PiTree::copy() {
    this->lazycopy = true;
    
    PiTree* new_pitree   = new PiTree(this->page_size, this->pages);
    new_pitree->lazycopy = true;
    this->pages->general_purpose += 1;
    return *new_pitree;
}

Interval* PiTree::find_page(int begin, int end) {
    std::vector<Interval*>& ris = this->pages->search_exact(begin, end);

    if (ris.size() == 0) {
        delete &ris;
        return nullptr;
    } else if (ris.size() == 1) {
        Interval* i = ris.front();
        delete &ris;
        return i;
    } else {
        delete &ris;
        std::cerr << "PiTree::find_page(): Malformed PiTree\n";
        throw 1;
    }
}

void PiTree::add(int begin, int end, Data* data) {
    assert(begin <= end);

    int p_begin = begin / this->page_size;
    int p_end   = end   / this->page_size + 1;

    this->copy_on_write();

    Interval* i = this->find_page(p_begin, p_end);
    if (i == nullptr) {
        Page* p = new Page(p_begin, p_end);
        p->add(begin, end, data);
        i = new Interval(p_begin, p_end, p);
        this->pages->add(*i);
    } else
        static_cast<Page*>(i->data)->add(begin, end, data);
}

void PiTree::add(int begin, int end) {
    assert(begin <= end);

    this->add(begin, end, nullptr);
}

std::ostream& operator<<(std::ostream& o, const PiTree& i) {
    return o << "*** PiTree *** at " << &i << "\n" << i.pages << "\n" << *(i.pages);
}

std::vector<Interval*>& PiTree::search(int begin, int end) const {
    assert(begin <= end);
    
    std::vector<Interval*>& ris = *(new std::vector<Interval*>());
    int p_begin = begin / this->page_size;
    int p_end   = end   / this->page_size + 1;

    std::vector<Interval*>& page_ris = this->pages->search(p_begin, p_end);
    for (std::vector<Interval*>::iterator it = page_ris.begin(); it != page_ris.end(); ++it) {
        static_cast<Page*>((*it)->data)->it->search(begin, end, ris);
    }
    delete &page_ris;
    return ris;
}