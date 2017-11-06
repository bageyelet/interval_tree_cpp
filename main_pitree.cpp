#include "PiTree.hpp"

using namespace std;

int main(int argc, char const *argv[]) {
    // Page* p = new Page(0, 1);
    // Interval i1(0, 1, p);
    // static_cast<Page*>(i1.data)->add(20, 30);
    // static_cast<Page*>(i1.data)->add(30, 40);
    // std::cout << i1;

    // std::cout << "copying... \n";
    // Interval& i2 = i1.copy();
    // std::cout << "i1:\n" << i1 << "\n";
    // std::cout << "i2:\n" << i2 << "\n";
    // static_cast<Page*>(i2.data)->add(23, 30);
    // std::cout << "i1:\n" << i1 << "\n";
    // std::cout << "i2:\n" << i2 << "\n";
    // static_cast<Page*>(i1.data)->add(24, 30);
    // std::cout << "i1:\n" << i1 << "\n";
    // std::cout << "i2:\n" << i2 << "\n";    
    // delete &i2;

    PiTree* pt = new PiTree();
    pt->add(1, 10);
    pt->add(2, 20);
    pt->add(3, 30);
    pt->add(100, 200);
    pt->add(299, 10000);
    cout << *pt;

    vector<Interval*>& ris = pt->search(11, 111);
    for (vector<Interval*>::iterator it = ris.begin() ; it != ris.end(); ++it) {
        cout << **it << "\n";
    }
    delete &ris;

    PiTree& pt2 = pt->copy();
    cout << "\n\n" << *pt << "\n";
    cout << "\n\n" << pt2 << "\n";

    pt2.add(0, 4);

    cout << "\n\n" << *pt << "\n";
    cout << "\n\n" << pt2 << "\n"; 

    delete pt; delete &pt2;
    return 0;
}