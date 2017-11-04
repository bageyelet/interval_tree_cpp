#include "IntervalTree.hpp"

using namespace std;

int main() {
    IntervalTree* it = new IntervalTree();
    Interval* i = new Interval(2,3); 
    it->add(*i); // do not pass interval on stack!
    it->add( 1, 2 );
    it->add( 4, 5 );
    it->add( 3, 7 );
    it->add(10, 29);
    it->add( 0, 5 );
    it->add(15, 20);
    it->dump();

    cout << "\nSEARCH\n";

    vector<Interval*>& ris = it->search(2,6);
    for (vector<Interval*>::iterator it = ris.begin() ; it != ris.end(); ++it) {
        cout << **it << "\n";
    }
    delete &ris;

    IntervalTree& it_copy = it->copy();
    delete it;
    it_copy.dump();

    delete &it_copy;

    return 0;
}