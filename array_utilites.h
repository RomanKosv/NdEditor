#ifndef ARRAY_UTILITES_H
#define ARRAY_UTILITES_H
#include <vector>
#include <functional>
namespace array_utilites {
using std::vector;
template<typename T>
vector<T> concatenate(vector<T> a,vector<T> b){
    vector<T> res;
    for(auto i:a) res.push_back(i);
    for(auto i:b) res.push_back(i);
    return res;
}
template<typename A, typename B>
vector<B> do_map(const vector<A> source, std::function<B(A)> fun){
    vector<B> res;
    for(A it:source){
        res.push_back(fun(it));
    }
    return res;
}
}
#endif // ARRAY_UTILITES_H
