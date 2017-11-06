#ifndef H_PITREE
#define H_PITREE

#include "IntervalTree.hpp"
#include <iostream>
#include <vector>

class Page : public Data {
  public:
    int        begin;
    int          end;
    bool    lazycopy;
    IntervalTree* it;

    Page(int begin, int end);
    ~Page();

    void add(int begin, int end);
    // void add(int begin, int end, Data* data);
    virtual bool operator==(const Data& that)    const; // equal if the same address
    virtual std::ostream& print(std::ostream& o) const;
    virtual Data& copy();

  private:
    Page(int begin, int end, IntervalTree* it);

    void copy_on_write();
};

#define DEFAULT_PAGESIZE 128

class PiTree {
    friend std::ostream& operator<<(std::ostream& o, const PiTree& i);

  public:
    IntervalTree* pages;
    bool       lazycopy;
    int       page_size;

    PiTree(int page_size);
    PiTree() : PiTree(DEFAULT_PAGESIZE) { };
    ~PiTree();

    void add(int begin, int end);
    // void add(int begin, int end, Data* data);
    std::vector<Interval*>& search(int begin, int end) const;
    PiTree& copy();

  private:
    PiTree(int page_size, IntervalTree* it);

    void copy_on_write();
    Interval* find_page(int begin, int end);
};

#endif