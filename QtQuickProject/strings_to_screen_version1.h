#ifndef STRINGS_TO_SCREEN_VERSION1_H
#define STRINGS_TO_SCREEN_VERSION1_H
#include <vector>
#include <tuple>
#include <string>
#include "model.h"
#include "triangles.h"
#include "objectentry.h"
#include <Eigen/Dense>
#include "parser/std_parsing_funs.h"
#include "triangulation_with_eigen_1.h"
using std::vector;
using std::tuple;
using std::string;
using Eigen::Vector3f;
using concrete_parsing_1::StdContext;
using concrete_parsing_1::ExprResSucces;
using concrete_parsing_1::EvalMaybe;
using concrete_parsing_1::NumExpr;
using concrete_parsing_1::Scalar;

class strings_to_screen_version1
{
public:
    static vector<Vector3f> out_box_normals;
    static float out_box_distance;
    typedef concrete_parsing_1::Figure Figure;
    typedef ParseResult<EvalMaybe<concrete_parsing_1::ExprResSucces>,StdContext,NoParseSintaxInfo> ParseRes;
    typedef EvalMaybe<concrete_parsing_1::ExprResSucces> Object;
    typedef StdContext Context;
    typedef nd_geometry::Polyhedron<concrete_parsing_1::NumExpr> Polyhedron;
    strings_to_screen_version1();
    vector<triangle> get_render(Model&,Context&);
    vector<triangle> triangulate(vector<Vector3f> normals,vector<float> distances,float epsilon = 1e-5f);
    tuple<vector<Vector3f>,vector<float>> imagine_3d(Polyhedron&);
    vector<tuple<vector<Vector3f>,vector<float>>> imagine_3d(Figure&);
    vector<Figure> search_figures(vector<Object>&);
    vector<Object> intreprete(Model&,Context&);
    Context get_standart_context();
private:
    Context default_context;
};

#endif // STRINGS_TO_SCREEN_VERSION1_H
