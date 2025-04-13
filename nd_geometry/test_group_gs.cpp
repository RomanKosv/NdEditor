#include "group_geom_sys.h"
#include "std_space_factory.h"
#include "naive_geometry_objects_factory.h"
#include "../parser/debug.hh"
#include "std_space_factory.h"
using Eigen::SparseVector;
static Log test_logger{true,"[nd_parser_realisation_1 group geom test]"};
typedef double Scalar;
typedef SparseVector<Scalar> Vec;
void test(){
    //компилится и на том спасибо
    nd_parser_realisation_1::GroupGeomSys<Scalar,Vec> gs;
    gs.algebra=linear_algebra_utilites::make_algebra<Scalar,Vec>(
        [](Vec a, Vec b)->Scalar{
            return a.dot(b);
        }
    );
    gs.obj_factory=nd_geometry::wrap_factory<Vec>();
    gs.order=linear_algebra_utilites::make_order<Scalar>();
    gs.polyhedron_gs=make_shared<nd_parser_realisation_1::PolyhedronGeomSys<Vec,Scalar>>(
        nd_parser_realisation_1::PolyhedronGeomSys<Vec,Scalar>{});
    gs.polyhedron_gs->algebra=gs.algebra;
    gs.polyhedron_gs->obj_factory=gs.obj_factory;
    gs.polyhedron_gs->order=gs.order;
    nd_geometry::GeometryObjectFactory<Vec> factory=gs.obj_factory;
    std::vector<nd_geometry::Group<Vec>> example_objs;
    example_objs.push_back(factory.make_group({}));
    example_objs.push_back(factory.make_group({factory.make_polyhedron({})}));
    nd_parser_realisation_1::StdSpaceFactory<Scalar> space{10000};
    Vec x=space.get_one(space.x);
    Vec y=space.get_one(space.y);
    Vec z=space.get_one(space.z);
    Vec scale=space.get_one(space.scale);
    //не доделал, по хорошему нужны полноценные тесты всех функций gs
}
