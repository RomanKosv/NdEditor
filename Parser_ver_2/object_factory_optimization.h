#ifndef OBJECT_FACTORY_OPTIMIZATION_H
#define OBJECT_FACTORY_OPTIMIZATION_H
#include "context_ver_2.h"
namespace concrete_parsing_ver_2 {
bool is_solvable(HalfSpace<NumExpr> inequality, StdSpaceFactory<Scalar> space, Scalar epsilon);
bool is_trivial_assertion(HalfSpace<NumExpr> inequality, StdSpaceFactory<Scalar> space, Scalar epsilon);
vector<HalfSpace<NumExpr>> filter_trivial(vector<HalfSpace<NumExpr>> source, StdSpaceFactory<Scalar> space, Scalar epsilon);
GeometryObjectFactory<NumExpr> wrap_geometry_factory_filter_trivial(Scalar epsilon, StdSpaceFactory<Scalar> space);
}
#endif // OBJECT_FACTORY_OPTIMIZATION_H
