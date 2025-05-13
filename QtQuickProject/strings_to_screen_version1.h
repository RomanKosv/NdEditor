#ifndef STRINGS_TO_SCREEN_VERSION1_H
#define STRINGS_TO_SCREEN_VERSION1_H
#include <vector>
#include <tuple>
#include <string>
//#include "model.h"
#include "triangles.h"
//#include "objectentry.h"
#include <Eigen/Dense>
#include "transformation_support.h"
#include "triangulation_with_eigen_1.h"
using std::vector;
using std::tuple;
using std::string;
using Eigen::Vector3f;
using concrete_parsing_ver_2::StdContext;
using concrete_parsing_ver_2::ExprResSucces;
using concrete_parsing_ver_2::EvalMaybe;
using concrete_parsing_ver_2::NumExpr;
using concrete_parsing_ver_2::Scalar;
using concrete_parsing_ver_2::NoParseSintaxInfo;
using concrete_parsing_ver_2::to_end;
using concrete_parsing_ver_2::get_node;
typedef concrete_parsing_ver_2::Figure Figure;
typedef ParseResult<EvalMaybe<concrete_parsing_ver_2::ExprResSucces>,StdContext,NoParseSintaxInfo> ParseRes;
typedef EvalMaybe<concrete_parsing_ver_2::ExprResSucces> Object;
typedef StdContext Context;
typedef nd_geometry::Polyhedron<concrete_parsing_ver_2::NumExpr> Polyhedra;
class strings_to_screen_version1
{
public:
    static vector<Vector3f> out_box_normals;
    float out_box_distance = 50;
    strings_to_screen_version1();
    //vector<triangle> get_render(Model&,Context&);
    vector<triangle> triangulate(vector<Vector3f> normals,vector<float> distances,float epsilon = 1e-5f);
    tuple<vector<Vector3f>,vector<float>> imagine_3d(Polyhedra&, float strong_inequality_align = -2e-5f);
    vector<tuple<vector<Vector3f>,vector<float>>> imagine_3d(Figure&);
    vector<Figure> search_figures(vector<Object>&);
    //vector<Object> intreprete(Model&,Context&);
    Context get_standart_context();
    //std::optional<Object> intreprete(ObjectEntry*, Context&);
private:
    Context default_context;
};

#endif // STRINGS_TO_SCREEN_VERSION1_H
