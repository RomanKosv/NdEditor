#ifndef CONTEXT_VER_2_H
#define CONTEXT_VER_2_H

#include <Eigen/Sparse>
#include "parser/debug.hh"
#include "nd_geometry/naive_geometry_objects_factory.h"
#include "nd_geometry/linear_geometry_std_structures.h"
#include "parser/function_parse_architect.hh"
#include "nd_geometry/std_space_factory.h"
#include "linear_algebra/linear_algebra.h"
#include "linear_algebra/linear_algebra_utilites.h"
#include "array_utilites.h"
#include "nd_geometry/transformation.h"
#include "nd_geometry/group_geom_sys.h"
#include "parser/wrappers.hh"
#include <string>
#include <map>
#include <functional>
#include <variant>
#include <Eigen/Geometry>
using std::map;
using std::string;
using std::function;
using std::variant;
using Eigen::SparseVector;
using nd_geometry::Group;
using common_parsing::ParseNode;
using nd_parser_realisation_1::StdSpaceFactory;
using common_parsing::Maybe;
using nd_geometry::LinearAlgebra;
using nd_parser_realisation_1::GroupGeomSys;
using nd_geometry::FirstTransformationRealisation;

namespace concrete_parsing_ver_2{
static Log context_log{false,"[std_context_log]"};
struct StdContext;
typedef double Scalar;
typedef SparseVector<Scalar> NumExpr;
typedef Group<NumExpr> Figure;
enum class EvalTypes{
    scalar,
    numexpr,
    figure
};
struct EvalError{
    string message;
};
template<typename Res, typename Context>
struct EvalRes{
    Res res;
    ParseNode<Res,Context> parse;
};
struct ExprResSucces{
    std::variant<NumExpr,Figure> res;
    NumExpr get_num();
    Figure get_figure();
    bool is_num() const;
    bool is_logic() const;
};
template<typename T>
struct EvalMaybe{
    std::variant<T,EvalError> res;
    bool isOk(){
        return res.index()==0;
    }
    T getOk(){
        return get<T>(res);
    }
    EvalError getErr(){
        return get<EvalError>(res);
    }
    static Maybe<EvalError> get_first_no_ok(vector<EvalMaybe<T>> vec){
        for(auto i:vec){
            if(!i.isOk()) return Maybe<EvalError>{get<EvalError>(i.res)};
        }
        return Maybe<EvalError>{};
    }
};
struct NumFun{
    function<EvalMaybe<NumExpr>(vector<EvalMaybe<ExprResSucces>>,StdContext)> fun;
};
struct BoolFun{
    function<EvalMaybe<Figure>(vector<EvalMaybe<ExprResSucces>>,StdContext)> fun;
};
struct EvalFun{
    variant<NumFun,BoolFun> fun;
    bool is_num() const;
    bool is_logic() const;
    NumFun get_num() const;
    BoolFun get_figure() const;
};

enum class MultOp{
    mult,
    div
};
enum class SumOp{
    add,
    rem
};
typedef common_parsing::None AndOp;
typedef common_parsing::None OrOp;
enum class CompOp{
    less,
    less_or_eq,
    more,
    more_or_eq,
    eq,
    no_eq
};
//struct NewVarVal{};

typedef FirstTransformationRealisation<NumExpr,Scalar> Transform;

struct StdContext{
    Scalar epsilon=1e-5;
    map<string,ExprResSucces> vars;
    map<string,EvalFun> funs;
    //map<string,NumExpr> numvars;
    //map<string,Figure> figures;
    map<size_t,string> dim_names;
    //map<string,NumFun> numfuns;
    //map<string,BoolFun> boolfuns;

    StdSpaceFactory<Scalar> space;
    LinearAlgebra<NumExpr,Scalar> algebra;
    GroupGeomSys<Scalar,NumExpr> gs;
    StdContext();
    StdContext(StdSpaceFactory<Scalar> _space, LinearAlgebra<NumExpr,Scalar> _algebra,GroupGeomSys<Scalar,NumExpr> _gs);
    void initalize_vars_and_functions();
    bool is_scalar(NumExpr expr);
    bool is_zero(NumExpr expr);
    bool is_vector(NumExpr expr);
    bool is_match_type(ExprResSucces val, EvalTypes type);
    Maybe<Scalar> get_scalar(NumExpr expr);
    string get_dim_name(size_t id);;
    EvalMaybe<NumExpr> do_div(NumExpr a, NumExpr b);
    EvalMaybe<NumExpr> do_mult(NumExpr a, NumExpr b);
    EvalMaybe<NumExpr> eval_mult_op(vector<EvalMaybe<NumExpr>> args, vector<MultOp> ops);
    EvalMaybe<NumExpr> eval_sum_op(vector<EvalMaybe<NumExpr>> args,vector<SumOp> ops);;
    EvalMaybe<Figure> eval_and_op(vector<EvalMaybe<Figure>> args,vector<AndOp>);;
    EvalMaybe<Figure> eval_or_op(vector<EvalMaybe<Figure>> args,vector<OrOp>);;
    EvalMaybe<ExprResSucces> get_common_var(string name);
    EvalMaybe<NumExpr> get_numvar(string name);
    EvalMaybe<Figure> get_logic_var(string name);
    EvalMaybe<NumFun> get_num_fun(string name);
    EvalMaybe<BoolFun> get_logic_fun(string name);
    EvalMaybe<EvalFun> get_common_fun(string name);
    common_parsing::Fun<EvalMaybe<ExprResSucces>,EvalMaybe<ExprResSucces>,StdContext> wrap_common_fun(EvalFun fun);
    EvalMaybe<Figure> inverse(EvalMaybe<Figure> figure);
    EvalMaybe<Figure> comp(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right,CompOp op);
    /*
    EvalMaybe<Figure> less(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> less_or_eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> more(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> more_or_eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> no_eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    */
    string to_str(NumExpr expr);
    template<typename T>
    string to_str(EvalMaybe<T> expr){
        if(expr.isOk()){
            return to_str(expr.getOk());
        }else{
            return "EvalError{"+expr.getErr().message+"}";
        }
    }
    string to_str(nd_geometry::HalfSpace<NumExpr> hs);
    string to_str(nd_geometry::Polyhedron<NumExpr> p);
    string to_str(Figure g);
    string to_str(ExprResSucces res);
    string to_str(EvalTypes type);
    EvalMaybe<Figure> move_figure(Figure figure, NumExpr dim, Scalar distance);
    EvalMaybe<Figure> move_figure(EvalMaybe<ExprResSucces> figure, EvalMaybe<ExprResSucces> dim, EvalMaybe<ExprResSucces> distance);
    EvalMaybe<Figure> move_figure(std::vector<EvalMaybe<ExprResSucces>> args);
    Transform get_empty_transform();
    Transform get_transform_with_static_scale();
    Transform get_full_transform_static_scale();
    optional<EvalError> check_match_types(vector<EvalMaybe<ExprResSucces>> & args, vector<EvalTypes> types, string funname);
    EvalMaybe<ExprResSucces> rotate_figure(vector<EvalMaybe<ExprResSucces>> & args);
};
}
#endif // CONTEXT_VER_2_H
