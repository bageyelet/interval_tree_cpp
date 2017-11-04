#include <cassert>
#include "Interval.hpp"

class MyData : public Data {
  public:
    int x;

    MyData(int xx) : x(xx) {}
    ~MyData() {}

    virtual bool operator==(const Data& that) const {
        const MyData* d = dynamic_cast<const MyData*> (&that);
        if (! d)
            return false;
        return this->x == d->x;
    }

    virtual std::ostream& print(std::ostream& o) const {
        return o << "{" << this->x << "}";
    }

    virtual Data& copy() const {
        return *(new MyData(this->x));
    }
};

int main() {
    Interval i1(10, 20);
    Interval i2(15, 30);
    Interval i3(11, 15);
    Interval i4(21, 30);
    Interval i5(15, 30);
    Interval& i7 = i5.copy();

    assert(i2 == i5);
    assert(i1 != i2);
    assert( i1.overlaps(i2));
    assert(!i1.overlaps(i4));
    assert( i1.contains(i3));
    assert(!i1.contains(i2));
    assert( i1.containsPoint(20));
    assert(!i1.containsPoint(21));
    assert(i5 == i7);

    std::cout << i1 << "\n";

    MyData* myd = new MyData(3);
    Interval i6(19, 20, myd);
    Interval& i8 = i6.copy();

    std::cout << i8 << "\n";
    delete &i8;
    delete &i7;

}