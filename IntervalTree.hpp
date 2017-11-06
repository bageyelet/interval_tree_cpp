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
    void  search_exact(const Interval& i, std::vector<Interval*>& ris);
    Node& copy() const;

  private:
    void rotation_right();
    void rotation_left ();
    void rebalance();
};

class IntervalTree {
    friend std::ostream& operator<<(std::ostream& o, const IntervalTree& i);

  public:
    Node*           root;
    int                n;
    long general_purpose;

    IntervalTree ();
    ~IntervalTree();

    void add(int begin, int end);
    void add(Interval& i);
    std::vector<Interval*>& search(int begin, int end) const;
    std::vector<Interval*>& search(int begin, int end, std::vector<Interval*>& ris) const;
    std::vector<Interval*>& search_exact(int begin, int end) const; // returns all intervals that have this begin and this end
    IntervalTree& copy() const;

    void dump();
};

#endif