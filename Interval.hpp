#ifndef INTERVAL_H
#define INTERVAL_H

#include <iostream>

class Data {
    friend std::ostream& operator<<(std::ostream& o, const Data& d);

  public:
    virtual ~Data() {}

    virtual bool operator==(const Data& that)    const = 0;
    virtual std::ostream& print(std::ostream& o) const = 0;
    virtual Data& copy() const = 0;
};

class Interval {
    friend std::ostream& operator<<(std::ostream& o, const Interval& i);
    
  public:
    int   begin;
    int   end;
    Data* data;

    Interval(int b, int e);
    Interval(int b, int e, Data* d);
    ~Interval(); // Data should be freed here? Now it is -> data must be on heap!

    bool containsPoint(int p) const;
    bool overlaps(const Interval& that) const;
    bool contains(const Interval& that) const;
    bool operator==(const Interval& that) const;
    bool operator!=(const Interval& that) const;
    Interval& copy() const; // the copied interval must be freed!
};

#endif