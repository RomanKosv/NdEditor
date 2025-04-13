#ifndef NAIVE_GEOMETRY_OBJECTS_FACTORY_H
#define NAIVE_GEOMETRY_OBJECTS_FACTORY_H
#include "linear_geometry_std_structures.h"
namespace nd_geometry{
template<typename Expression>
HalfSpace<Expression> wrap_halfspace(Expression expr,bool strong){
    std::shared_ptr<Expression> sh_expr=std::make_shared<Expression>(expr);
    return HalfSpace<Expression>{
        [sh_expr](){
            return sh_expr;
        },
        [strong](){
            return strong;
        }
    };
}
template<typename Expr>
Polyhedron<Expr> wrap_polyhedron(std::vector<HalfSpace<Expr>> hs){
    std::shared_ptr<std::vector<HalfSpace<Expr>>> shared=
        std::make_shared<std::vector<HalfSpace<Expr>>>(hs);
    return Polyhedron<Expr>{
        [shared](){
            return shared;
        }
    };
}
template<typename Expr>
Group<Expr> wrap_group(std::vector<Polyhedron<Expr>> polyhedrones){
    std::shared_ptr<std::vector<Polyhedron<Expr>>> shared=
        std::make_shared<std::vector<Polyhedron<Expr>>>(polyhedrones);
    return Group<Expr>{
        [shared](){
            return shared;
        }
    };
}
template<typename Expr>
GeometryObjectFactory<Expr> wrap_factory(){
    return GeometryObjectFactory<Expr>{
        wrap_halfspace<Expr>,
        wrap_polyhedron<Expr>,
        wrap_group<Expr>
    };
}
};
#endif // NAIVE_GEOMETRY_OBJECTS_FACTORY_H
