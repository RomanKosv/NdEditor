#ifndef LINEAR_GEOMETRY_STD_STRUCTURES_H
#define LINEAR_GEOMETRY_STD_STRUCTURES_H
#include <functional>
#include <vector>
#include <memory>
#include "geom_sys.h"
namespace nd_geometry{
template<typename Expression>
struct HalfSpace{
    std::function<std::shared_ptr<Expression>()> get_upper_bound;
    std::function<bool()> is_strong;
};
template<typename Expression>
struct Polyhedron{
    std::function<std::shared_ptr<std::vector<HalfSpace<Expression>>>()> get_faces;
};
template<typename Expression>
struct Group{
    std::function<std::shared_ptr<std::vector<Polyhedron<Expression>>>()> get_components;
};
template<typename Expression>
struct GeometryObjectFactory{
    std::function<HalfSpace<Expression>(Expression,bool)> make_halfspace;
    std::function<Polyhedron<Expression>(std::vector<HalfSpace<Expression>>)> make_polyhedron;
    std::function<Group<Expression>(std::vector<Polyhedron<Expression>>)> make_group;
};
template<typename Expression>
using ExprGeomSys=GeometrySystem<Expression,Group<Expression>>;

}
#endif // LINEAR_GEOMETRY_STD_STRUCTURES_H
