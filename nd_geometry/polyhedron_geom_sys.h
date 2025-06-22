#ifndef POLYHEDRON_GEOM_SYS_H
#define POLYHEDRON_GEOM_SYS_H
#include "chernikovrulessupport.h"
using namespace nd_geometry;
namespace nd_parser_realisation_1{
template<typename Expr,typename Coef>
class PolyhedronGeomSys{
public:
    GeometryObjectFactory<Expr> obj_factory;
    LinearAlgebra<Expr,Coef> algebra;
    LinearOrder<Coef> order;
    Expr zero;
    Coef epsilon;
    virtual Polyhedron<Expr> intersect_of(Polyhedron<Expr> o1, Polyhedron<Expr> o2){
        std::vector<HalfSpace<Expr>> vec;
        for(auto hs:*o1.get_faces()){
            vec.push_back(hs);
        }
        for(auto hs:*o2.get_faces()){
            vec.push_back(hs);
        }
        //Group<Expr> res=obj_factory.make_group({obj_factory.make_polyhedron(vec)});
        return obj_factory.make_polyhedron(vec);
    };
    virtual Group<Expr> union_of(Polyhedron<Expr> o1, Polyhedron<Expr> o2){
        return obj_factory.make_group({o1,o2});
    };
    virtual Polyhedron<Expr> project_parallel(Polyhedron<Expr> o, Expr d){
        std::vector<HalfSpace<Expr>> negative;
        std::vector<HalfSpace<Expr>> neutral;
        std::vector<HalfSpace<Expr>> positive;
        for(HalfSpace<Expr> hs:*o.get_faces()){
            CompRes comp=linear_algebra_utilites::dot_collyniaryty_to_0(
                algebra, order, *hs.get_upper_bound(), d);
            switch (comp) {
            case CompRes::Less:
                negative.push_back(hs);
                break;
            case CompRes::Eq:
                neutral.push_back(hs);
                break;
            default:
                positive.push_back(hs);
                break;
            }
        }
        for(HalfSpace<Expr> i:negative){
            for(HalfSpace<Expr> j:positive){
                Expr negative_vec=*i.get_upper_bound();
                Expr positive_vec=*j.get_upper_bound();
                auto newexpr=linear_algebra_utilites::annihilate_inequalites_component(algebra,order,negative_vec,positive_vec,d);
                bool is_strong=i.is_strong()||j.is_strong();
                neutral.push_back(obj_factory.make_halfspace(newexpr,is_strong));
            }
        }
        return obj_factory.make_polyhedron(neutral);
    };
    virtual Polyhedron<Expr> project_in_ortogonal(Polyhedron<Expr> o, std::vector<Expr> dims){
        std::vector<Expr> project_dims;
        for(HalfSpace<Expr> hs:*o.get_faces()){
            Expr perpendicular=linear_algebra_utilites::get_indepedent_to_ortogonal(
                algebra,*hs.get_upper_bound(),dims);
            Coef sq_ln=linear_algebra_utilites::like_sqrt_len(algebra,perpendicular);
            if(order.compare_first_to_second(sq_ln,epsilon)==CompRes::More){
                dims.push_back(perpendicular);
                project_dims.push_back(perpendicular);
            }
        }
        ChernikovRulesSupport<Coef,Expr> support;
        return support.project_parallel_dims(o, project_dims, algebra, order, obj_factory);
    }
    virtual Polyhedron<Expr> project_in(Polyhedron<Expr> o, std::vector<Expr> dims){
        /*
        for(Expr curdim:dims){
            for(HalfSpace<Expr> hs:*o.get_faces()){//дальше абсолютно не правильный код
                Expr perpendicular=
                    linear_algebra_utilites::perpendicular_component(
                    algebra,*hs.get_upper_bound(),curdim);
                if(!linear_algebra_utilites::is_zero(algebra,perpendicular)){
                    return project_in(project_parallel(o,perpendicular),dims);
                }
            }
        }
        return o;
        */
        //теперь должно работать
        return project_in_ortogonal(
            o,
            linear_algebra_utilites::ortanogalise_space_with_epsilon(
                algebra,dims,order,epsilon)
            );
    };
    virtual Group<Expr> inversion_of(Polyhedron<Expr> o){
        //попытался пофиксить баг
        if(o.get_faces()->size()==0){
            return obj_factory.make_group({obj_factory.make_polyhedron({obj_factory.make_halfspace(zero,true)})});
        }
        std::vector<Polyhedron<Expr>> halfspaces;
        for(HalfSpace<Expr> hs:*o.get_faces()){
            auto inversed=obj_factory.make_halfspace(-*hs.get_upper_bound(),!hs.is_strong());
            halfspaces.push_back(
                obj_factory.make_polyhedron({inversed}));
        }
        return obj_factory.make_group(halfspaces);
    };
};
}
#endif // POLYHEDRON_GEOM_SYS_H
