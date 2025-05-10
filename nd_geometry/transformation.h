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
    GeometryObjectFactory<Expr> factory;
    //Group<Expr> transformation_conditions;
    struct DimPair{
        Expr previous;
        Expr next;
    };
    Expr set_dim(std::vector<DimPair> pairs, Expr e) const{
        Expr expr=e;
        for(DimPair it:pairs){
            expr=linear_algebra_utilites::set_equal(algebra,expr,it.previous,it.next);
        }
        return expr;
    }
    HalfSpace<Expr> set_dim(std::vector<DimPair> pairs, HalfSpace<Expr> hs) const{
        return factory.make_halfspace(set_dim(pairs,*hs.get_upper_bound()),hs.is_strong());
    }
    Polyhedron<Expr> set_dim(std::vector<DimPair> pairs, Polyhedron<Expr> figure) const{
        auto fn=[this,pairs](HalfSpace<Expr> it){
            return set_dim(pairs,it);
        };
        return factory.make_polyhedron(array_utilites::do_map<HalfSpace<Expr>,HalfSpace<Expr>>(
            *figure.get_faces(),fn));
    }
    Group<Expr> set_dim(std::vector<DimPair> pairs, Group<Expr> figure) const{
        auto fn=[this,pairs](Polyhedron<Expr> it){
            return set_dim(pairs,it);
        };
        return factory.make_group(array_utilites::do_map<Polyhedron<Expr>,Polyhedron<Expr>>(
            *figure.get_components(),fn));
    }
    std::vector<DimPair> pairs;
    std::vector<DimPair> reversed_pairs() const{
        return array_utilites::do_map<DimPair,DimPair>(pairs,[](DimPair p){return DimPair{p.next,p.previous};});
    }
    Group<Expr> transform(Group<Expr> figure, Group<Expr> transformation_conditions) const{
        Group<Expr> dependence=gs->intersect_of(figure,transformation_conditions);
        std::vector<Expr> next_space=array_utilites::do_map<DimPair,Expr>(pairs,[](DimPair p){return p.next;});
        Group<Expr> projection=gs->project_in(dependence,next_space);
        Group<Expr> res=set_dim(reversed_pairs(), projection);
        return res;
    }
    Group<Expr> transform_accurate(Group<Expr> figure, Group<Expr> transformation_conditions) const{
        Group<Expr> dependence=gs->intersect_of(figure,transformation_conditions);
        std::vector<Expr> previous_space=array_utilites::do_map<DimPair,Expr>(pairs,[](DimPair p){return p.previous;});
        Group<Expr> projection=dependence;
        for(auto& i:previous_space){
            projection=gs->project_parallel(projection,i);
        }
        Group<Expr> res=set_dim(reversed_pairs(), projection);
        return res;
    }
};
}
#endif // TRANSFORMATION_H
