#include "Interval.hpp"

Interval::Interval(int b, int e) : begin(b), end(e) {
    this->data = nullptr;
}

Interval::Interval(int b, int e, Data* d) : begin(b), end(e), data(d) {}

Interval::~Interval() {
    if (this->data != nullptr) delete this->data;
}

bool Interval::containsPoint(int p) const {
    return this->begin <= p && this->end >= p;
}

bool Interval::overlaps(const Interval& that) const {
    return ! (this->begin > that.end || this->end < that.begin);
}

bool Interval::contains(const Interval& that) const {
    return this->begin <= that.begin && this->end >= that.end;
}

bool Interval::operator==(const Interval& that) const {
    if (this->data == nullptr && that.data == nullptr)
        return this->begin == that.begin && this->end == that.end;
    if ((this->data == nullptr && that.data != nullptr) || (this->data != nullptr && that.data == nullptr))
        return false;
    return this->begin == that.begin && this->end == that.end && *(this->data) == *(that.data);
}

bool Interval::operator!=(const Interval& that) const {
    return ! (*this==that);
}

Interval& Interval::copy() const {
    Interval* new_interval;
    if (this->data == nullptr)
        new_interval = new Interval(begin, end, data);
    else
        new_interval = new Interval(begin, end, &(data->copy()));
    return *new_interval;
}

std::ostream& operator<<(std::ostream& o, const Interval& i) {
    if (i.data == nullptr)
        o << "[" << i.begin << ", " << i.end << "] ( NULL )";
    else
        o << "[" << i.begin << ", " << i.end << "] ( " << *(i.data) << " )";
    return o;
}

std::ostream& operator<<(std::ostream& o, const Data& d) {
    return d.print(o);
}