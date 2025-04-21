#include <iostream>
#include "parser/test_inequality_system_parsing_1.h"

using namespace std;

int main()
{
    /*
    SparseVector<double> vec(500);
    vec.coeffRef(1)=1;
    vec=vec;
    cout<<vec*(-0.0)<<"\n";
    LinearAlgebra<SparseVector<double>,double> a=linear_algebra_utilites::make_algebra<double,SparseVector<double>>([](SparseVector<double> a, SparseVector<double> b)->double{return a.dot(b);});
    function<SparseVector<double>(SparseVector<double>,double)> mult=[](SparseVector<double> a,double c)->SparseVector<double>{
        return a*c;
    };
    cout<<mult(vec,-0.0)<<"\n";
    a.mult=mult;
    cout<<a.mult(vec,-0.0)<<"\n";
    LinearAlgebra<SparseVector<double>,double> b=a;
    cout<<b.mult(vec,-0.0)<<"\n";
*/
    test_inequalyty_systems_and_num_expr_1();
}
