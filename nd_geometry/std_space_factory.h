#ifndef STD_SPACE_FACTORY_H
#define STD_SPACE_FACTORY_H
#include <Eigen/Sparse>
#include <memory>
using Eigen::SparseVector;
namespace nd_parser_realisation_1{
template<typename Coef>
struct StdSpaceFactory{
    size_t scale=0;
    size_t x=1;
    size_t y=2;
    size_t z=3;
    std::shared_ptr<size_t> next=std::make_shared<size_t>(4);
    size_t sparse_size=50000;
    StdSpaceFactory(size_t size){
        sparse_size=size;
    }
    StdSpaceFactory(){
    }
    size_t get_next(){
        size_t cur=*next;
        (*next)++;
        return cur;
    }
    SparseVector<Coef> get_one(size_t index){
        SparseVector<Coef> res(sparse_size);
        res.coeffRef(index)=1;
        return res;
    }
    SparseVector<Coef> get_scale(){
        return get_one(scale);
    }
    SparseVector<Coef> get_zero(){
        return SparseVector<Coef>(sparse_size);
    }
};
}
#endif // STD_SPACE_FACTORY_H
