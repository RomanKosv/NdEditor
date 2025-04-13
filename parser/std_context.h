#ifndef STD_CONTEXT_H
#define STD_CONTEXT_H
#include <Eigen/Sparse>
#include "nd_geometry/linear_geometry_std_structures.h"
#include "parser/function_parse_architect.hh"
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

struct StdContext{
    typedef double Scalar;
    typedef SparseVector<Scalar> Vec;
    typedef Group<Vec> Figure;
    struct NumExpr
    {
        Vec vec;
        ParseNode<Vec,StdContext> node;
    };
    struct EvalRes{
        variant<NumExpr,Figure> res;
        EvalRes(NumExpr expr):res{expr}{};
        EvalRes(Figure figure):res{figure}{};
    };
    enum class EvalTypes{
        scalar,
        numexpr,
        figure
    };
    struct TypeMissMatch{
        EvalRes val;
        EvalTypes expected;
        EvalTypes found;
    };
    struct DivOnZeroErr{
        NumExpr num;
        Scalar divider;
    };
    struct MultPowError{
        NumExpr left;
        NumExpr right;
    };
    struct DivPowError{
        NumExpr left;
        NumExpr right;
    };
    map<string,NumExpr> numvars;
    map<string,Figure> figures;
};

#endif // STD_CONTEXT_H
