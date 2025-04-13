#ifndef GROUP_GEOM_SYS_H
#define GROUP_GEOM_SYS_H
#include "polyhedron_geom_sys.h"
#include "../array_utilites.h"
using namespace nd_geometry;
using namespace array_utilites;
namespace nd_parser_realisation_1{
template<typename Coef, typename Expr>
class GroupGeomSys:public GeometrySystem<Expr,Group<Expr>>{
public:
    GeometryObjectFactory<Expr> obj_factory;
    LinearAlgebra<Expr,Coef> algebra;
    LinearOrder<Coef> order;
    std::shared_ptr<PolyhedronGeomSys<Expr,Coef>> polyhedron_gs;
    virtual Group<Expr> intersect_of(Group<Expr> o1, Group<Expr> o2)override{
        std::vector<Polyhedron<Expr>> intersections;
        for(Polyhedron<Expr> i:*o1.get_components()){
            for(Polyhedron<Expr> j:*o2.get_components()){
                intersections.push_back(polyhedron_gs->intersect_of(i,j));
            }
        }
        return obj_factory.make_group(intersections);
    };
    virtual Group<Expr> union_of(Group<Expr> o1, Group<Expr> o2)override{
        return obj_factory.make_group(concatenate((*o1.get_components()),(*o2.get_components())));
    };
    virtual Group<Expr> project_parallel(Group<Expr> o, Expr d)override{
        std::vector<Polyhedron<Expr>> projections;
        for(Polyhedron<Expr> i:*o.get_components()){
            projections.push_back(polyhedron_gs->project_parallel(i,d));
        }
        return obj_factory.make_group(projections);
    };
    virtual Group<Expr> project_in(Group<Expr> o, std::vector<Expr> dims)override{
        std::vector<Polyhedron<Expr>> projections;
        for(Polyhedron<Expr> i:*o.get_components()){
            projections.push_back(polyhedron_gs->project_in(i,dims));
        }
        return obj_factory.make_group(projections);
    };
    virtual Group<Expr> inversion_of(Group<Expr> o)override{
        Group<Expr> res=obj_factory.make_group({});
        for(Polyhedron<Expr> i:*o.get_components()){
            res=intersect_of(res,polyhedron_gs->inversion_of(i));
        }
        return res;
    };
};
}
#endif // GROUP_GEOM_SYS_H
