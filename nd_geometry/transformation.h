#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include "../linear_algebra/linear_algebra_utilites.h"
#include "linear_geometry_std_structures.h"
#include <vector>
#include "../array_utilites.h"
namespace nd_geometry{
template<typename Expr, typename Coef>
struct FirstTransformationRealisation{
    std::shared_ptr<ExprGeomSys<Expr>> gs;
    LinearAlgebra<Expr,Coef> algebra;
    ExprGeomSys<Expr> factory;
    Group<Expr> transformation_conditions;
    struct DimPair{
        Expr previous;
        Expr next;
    };
    std::vector<DimPair> pairs;
    HalfSpace<Expr> set_dim(std::vector<DimPair> pairs, HalfSpace<Expr> hs){
        Expr expr=*hs.get_upper_bound();
        for(DimPair it:pairs){
            expr=linear_algebra_utilites::set_equal(algebra,expr,it.previous,it.next);
        }
        return factory.make_halfspace(expr,hs.is_strong());
    }
    Polyhedron<Expr> set_dim(std::vector<DimPair> pairs, Polyhedron<Expr> figure){
        auto fn=[this,pairs](HalfSpace<Expr> it){
            return set_dim(pairs,it);
        };
        return factory.make_polyhedron(array_utilites::do_map(
            *figure.get_faces(),fn));
    }
    Group<Expr> set_dim(std::vector<DimPair> pairs, Group<Expr> figure){
        auto fn=[this,pairs](Polyhedron<Expr> it){
            return set_dim(pairs,it);
        };
        return factory.make_group(array_utilites::do_map(
            *figure.get_components(),fn));
    }
    std::vector<DimPair> reversed_pairs(){
        return array_utilites::do_map(pairs,[](DimPair p){return DimPair{p.next,p.previous};});
    }
    Group<Expr> transform(Group<Expr> figure){
        Group<Expr> dependence=gs->intersect_of(figure,transformation_conditions);
        std::vector<Expr> next_space=array_utilites::do_map(pairs,[](DimPair p){return p.next;});
        Group<Expr> projection=gs->project_in(dependence,next_space);
        Group<Expr> res=set_dim(projection,reversed_pairs());
        return res;
    }
};
}
#endif // TRANSFORMATION_H
