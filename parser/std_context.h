#ifndef STD_CONTEXT_H
#define STD_CONTEXT_H
#include <Eigen/Sparse>
#include "../nd_geometry/linear_geometry_std_structures.h"
#include "../parser/function_parse_architect.hh"
#include "../nd_geometry/std_space_factory.h"
#include "../parser/parsing_errors.h"
#include "../linear_algebra/linear_algebra.h"
#include "../linear_algebra/linear_algebra_utilites.h"
#include "../array_utilites.h"
#include "../nd_geometry/group_geom_sys.h"
#include <string>
#include <map>
#include <functional>
#include <variant>
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
namespace concrete_parsing_1{
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
    NumExpr get_num(){
        return get<NumExpr>(res);
    }
    Figure get_figure(){
        return get<Figure>(res);
    }
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
    function<EvalMaybe<Figure>(vector<EvalMaybe<ExprResSucces>>)> fun;
};
struct EvalFun{
    variant<NumFun,BoolFun> fun;
    bool is_num(){
        return fun.index()==0;
    }
    bool is_logic(){
        return fun.index()==1;
    }
    NumFun get_num(){
        return get<NumFun>(fun);
    }
    BoolFun get_figure(){
        return get<BoolFun>(fun);
    }
};

enum class MultOp{
    mult,
    div
};
enum class SumOp{
    add,
    rem
};
struct AndOp{};
struct OrOp{};

struct StdContext{
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
    bool is_scalar(NumExpr expr){
        NumExpr perpendicular=linear_algebra_utilites::perpendicular_component(
            algebra,expr,space.get_one(space.scale));
        return linear_algebra_utilites::is_zero(algebra,perpendicular);
    }
    bool is_match_type(ExprResSucces val, EvalTypes type){
        switch (type) {
        case EvalTypes::figure:
            return val.res.index()==1;
            break;
        case EvalTypes::numexpr:
            return val.res.index()==0;
        case EvalTypes::scalar:
            return is_match_type(val,EvalTypes::numexpr)&&is_scalar(get<NumExpr>(val.res));
        default:
            assert(false);
            break;
        }
    }
    Maybe<Scalar> get_scalar(NumExpr expr){
        if(is_scalar(expr)){
            return Maybe<Scalar>{
                linear_algebra_utilites::project(algebra,expr,space.get_one(space.scale))};
        }else{
            return Maybe<Scalar>{};
        }
    }
    string get_dim_name(size_t id){
        if(dim_names.contains(id)){
            return dim_names[id];
        }else{
            return "<unnamed dimension "+to_string(id)+" >";
        }
    };
    EvalMaybe<NumExpr> do_div(NumExpr a, NumExpr b){
        if(is_scalar(b)){
            if(get_scalar(b).get_ok()==0){
                return EvalMaybe<NumExpr>{EvalError{"div on 0"}};
            }else{
                return EvalMaybe<NumExpr>{a/get_scalar(b).get_ok()};
            }
        }else{
            return EvalMaybe<NumExpr>{EvalError{"div on linear expr"}};
        }
    }
    EvalMaybe<NumExpr> do_mult(NumExpr a, NumExpr b){
        if(is_scalar(a)){
            return EvalMaybe<NumExpr>{b*get_scalar(a).get_ok()};
        }else if(is_scalar(b)){
            return EvalMaybe<NumExpr>{a*get_scalar(b).get_ok()};
        }else{
            return EvalMaybe<NumExpr>{EvalError{"mult two linears"}};
        }
    }
    EvalMaybe<NumExpr> eval_mult_op(vector<EvalMaybe<NumExpr>> args, vector<MultOp> ops){
        assert(args.size()!=0);
        Maybe<EvalError> err=EvalMaybe<NumExpr>::get_first_no_ok(args);
        if(!err.isEmpty()){
            return EvalMaybe<NumExpr>{get<EvalError>(err.val)};
        }else{
            vector<NumExpr> numargs=array_utilites::do_map<EvalMaybe<NumExpr>,NumExpr>(args,
                [](EvalMaybe<NumExpr> arg){return arg.getOk();});
            NumExpr res=numargs[0];
            for(size_t i=1; i<args.size(); i++){
                if(ops[i-1]==MultOp::div){
                    EvalMaybe<NumExpr> divres=do_div(res,numargs[i]);
                    if(!divres.isOk()) return divres;
                }else{
                    EvalMaybe<NumExpr> multres=do_mult(res,numargs[i]);
                    if(!multres.isOk()) return multres;
                }
            }
            return EvalMaybe<NumExpr>{res};
        }
    }
    EvalMaybe<NumExpr> eval_sum_op(vector<EvalMaybe<NumExpr>> args,vector<SumOp> ops){
        assert(args.size()!=0);
        Maybe<EvalError> err=EvalMaybe<NumExpr>::get_first_no_ok(args);
        if(!err.isEmpty()){
            return EvalMaybe<NumExpr>{get<EvalError>(err.val)};
        }else{
            vector<NumExpr> numargs=array_utilites::do_map<EvalMaybe<NumExpr>,NumExpr>(args,
                [](EvalMaybe<NumExpr> arg){return arg.getOk();});
            NumExpr res=numargs[0];
            for(size_t i=1; i<args.size(); i++){
                if(ops[i-1]==SumOp::add){
                    res=res+numargs[i];
                }else{
                    res=res-numargs[i];
                }
            }
            return EvalMaybe<NumExpr>{res};
        }
    };
    EvalMaybe<Figure> eval_and_op(vector<EvalMaybe<Figure>> args,vector<AndOp>){
        assert(args.size()!=0);
        Maybe<EvalError> err=EvalMaybe<Figure>::get_first_no_ok(args);
        if(!err.isEmpty()){
            return EvalMaybe<Figure>{get<EvalError>(err.val)};
        }else{
            vector<Figure> boolargs=array_utilites::do_map<EvalMaybe<Figure>,Figure>(args,
                [](EvalMaybe<Figure> arg){return arg.getOk();});
            Figure res=boolargs[0];
            for(size_t i=1; i<args.size(); i++){
                res=gs.intersect_of(res,boolargs[i]);
            }
            return EvalMaybe<Figure>{res};
        }
    };
    EvalMaybe<Figure> eval_or_op(vector<EvalMaybe<Figure>> args,vector<OrOp>){
        assert(args.size()!=0);
        Maybe<EvalError> err=EvalMaybe<Figure>::get_first_no_ok(args);
        if(!err.isEmpty()){
            return EvalMaybe<Figure>{get<EvalError>(err.val)};
        }else{
            vector<Figure> boolargs=array_utilites::do_map<EvalMaybe<Figure>,Figure>(args,
                [](EvalMaybe<Figure> arg){return arg.getOk();});
            Figure res=boolargs[0];
            for(size_t i=1; i<args.size(); i++){
                res=gs.union_of(res,boolargs[i]);
            }
            return EvalMaybe<Figure>{res};
        }
    };
    EvalMaybe<NumExpr> get_numvar(string name){
        if(vars.contains(name)&&is_match_type(vars[name],EvalTypes::numexpr)){
            return EvalMaybe<NumExpr>{vars[name].get_num()};
        }else{
            return EvalMaybe<NumExpr>{EvalError{"cant find num var "+name}};
        }
    }
    EvalMaybe<Figure> get_logic_var(string name){
        if(vars.contains(name)&&is_match_type(vars[name],EvalTypes::numexpr)){
            return EvalMaybe<Figure>{vars[name].get_figure()};
        }else{
            return EvalMaybe<Figure>{EvalError{"cant find logic var "+name}};
        }
    }
    EvalMaybe<NumFun> get_num_fun(string name){
        if(funs.contains(name)&&funs[name].is_num()){
            return EvalMaybe<NumFun>{funs[name].get_num()};
        }else{
            return EvalMaybe<NumFun>{EvalError{"cant find num fun "+name}};
        }
    }
    EvalMaybe<BoolFun> get_logic_fun(string name){
        if(funs.contains(name)&&funs[name].is_logic()){
            return EvalMaybe<BoolFun>{funs[name].get_figure()};
        }else{
            return EvalMaybe<BoolFun>{EvalError{"cant find logic fun "+name}};
        }
    }
    EvalMaybe<Figure> less(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> less_or_eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> more(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> more_or_eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
    EvalMaybe<Figure> no_eq(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right);
};
}
#endif // STD_CONTEXT_H
