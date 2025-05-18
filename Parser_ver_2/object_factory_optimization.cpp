#include "object_factory_optimization.h"
bool concrete_parsing_ver_2::is_solvable(HalfSpace<NumExpr> inequality, StdSpaceFactory<Scalar> space, Scalar epsilon)
{
    if(!is_trivial_assertion(inequality,space,epsilon)) return true;
    NumExpr& expr=*inequality.get_upper_bound();
    return expr.coeff(space.scale)<0||(expr.coeff(space.scale)<=epsilon&&!inequality.is_strong());
}

bool concrete_parsing_ver_2::is_trivial_assertion(HalfSpace<NumExpr> inequality, StdSpaceFactory<Scalar> space, Scalar epsilon)
{
    NumExpr& expr=*inequality.get_upper_bound();
    for(NumExpr::InnerIterator it(expr); it; ++it){
        if(it.index()!=space.scale&&abs(it.value())>epsilon){
            return false;
        }
    }
    return true;
}

vector<HalfSpace<concrete_parsing_ver_2::NumExpr> > concrete_parsing_ver_2::filter_trivial(vector<HalfSpace<NumExpr> > source, StdSpaceFactory<Scalar> space, Scalar epsilon)
{
    vector<HalfSpace<NumExpr>> res;
    for(auto& it:source){
        if(!is_solvable(it,space,epsilon)){
            return {it};
            //на этом этапе обнаружен баг с инверсией, если 0 граней
        }else if(!is_trivial_assertion(it,space,epsilon)){
            res.push_back(it);
        }
    }
    return res;
}

GeometryObjectFactory<concrete_parsing_ver_2::NumExpr> concrete_parsing_ver_2::wrap_geometry_factory_filter_trivial(Scalar epsilon, StdSpaceFactory<Scalar> space)
{
    auto f=wrap_factory<NumExpr>();
    f.make_polyhedron=[epsilon,space](vector<HalfSpace<NumExpr>> faces){
        return wrap_polyhedron<NumExpr>(filter_trivial(faces,space,epsilon));
    };
    return f;
}
