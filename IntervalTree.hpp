#ifndef INTERVALTREE_H
#define INTERVALTREE_H

#define MAX(a,b) ({ __typeof__ (a) _a = (a);    \
                    __typeof__ (b) _b = (b);    \
                    _a > _b ? _a : _b; })

#include <vector>
#include "Interval.hpp"

class IntervalTree; // to achieve co-dependency
class Node {
  public:
    Interval*       i;
    Node*      parent;
    Node*  left_child;
    Node* right_child;
    int           max;
    int    left_depth;
    int   right_depth;
    IntervalTree*  it; // is null unless the Node is the root of the IntervalTree

    int balancing_factor() { return right_depth - left_depth; }

    Node(Interval& i);
    Node(Interval& i, int max);
    ~Node();

    void  add(Interval& i);
    void  search(const Interval& i, std::vector<Interval*>& ris);
    Node& copy() const;

  private:
    void rotation_right();
    void rotation_left ();
    void rebalance();
};

class IntervalTree {
  public:
    Node* root;
    int   n;

    IntervalTree ();
    ~IntervalTree();

    void add(int begin, int end);
    void add(Interval& i);
    std::vector<Interval*>& search(int begin, int end) const;
    IntervalTree& copy() const;

    void dump();
};

#endif