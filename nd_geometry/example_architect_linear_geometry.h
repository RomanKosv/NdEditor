#ifndef EXAMPLE_ARCHITECT_LINEAR_GEOMETRY_H
#define EXAMPLE_ARCHITECT_LINEAR_GEOMETRY_H
#include "geom_sys.h"
#include "linear_geometry_std_structures.h"
#include "../linear_algebra/linear_algebra.h"
#include <memory>
#include <Eigen/Sparse>
#include <set>
using Eigen::SparseVector;
using namespace nd_geometry;
template<typename Expression,typename Coeficent>
struct ExampleLinearGeometrySystemAlgorithms:public ExprGeomSys<Expression>
{
public:
    GeometryObjectFactory<Expression> factory;
    LinearAlgebra<Expression,Coeficent> algebra;
    typedef Group<Expression> Obj;
    Obj intersect_of(Obj o1, Obj o2)override{
    };
    Obj union_of(Obj o1, Obj o2)=0;
    Obj project_parallel(Obj o, Expression d)=0;
    Obj project_in(Obj o, std::set<Expression> dims)=0;
    Obj inversion_of(Obj o)=0;
};
#endif // EXAMPLE_ARCHITECT_LINEAR_GEOMETRY_H
