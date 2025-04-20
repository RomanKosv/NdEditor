#ifndef LINEAR_ALGEBRA_UTILITES_H
#define LINEAR_ALGEBRA_UTILITES_H
#include "linear_algebra.h"
#include <cassert>
using namespace nd_geometry;
namespace linear_algebra_utilites{
template<typename Expr, typename Coef>
Coef like_sqrt_len(LinearAlgebra<Expr,Coef> la, Expr expr){
    return la.like_dot_product(expr,expr);
}
template<typename Expression, ArifmetycOp Coeficent>
Coeficent project(LinearAlgebra<Expression,Coeficent> la, Expression exp, Expression on){
    return la.like_dot_product(exp,on)/la.like_dot_product(on,on);
}
template<typename Expr, typename Coef>
CompRes dot_collyniaryty_to_0(LinearAlgebra<Expr,Coef> la, LinearOrder<Coef> lo, Expr a, Expr b){
    return lo.compare_to_zero(la.like_dot_product(a,b));
}
template<typename Expr, ArifmetycOp Coef>
Expr annihilate_component(LinearAlgebra<Expr,Coef> la, Expr a, Expr b, Expr component){
    return la.add(la.mult(a,project(la,b,component)),la.mult(b,-project(la,a,component)));
}
template<typename Expr, ArifmetycOp Coef>
Expr annihilate_inequalites_component(LinearAlgebra<Expr,Coef> la, LinearOrder<Coef> lo, Expr negative, Expr positive, Expr component){
    assert(dot_collyniaryty_to_0(la,lo,negative,component)!=CompRes::More);
    assert(dot_collyniaryty_to_0(la,lo,positive,component)!=CompRes::Less);
    return la.add(la.mult(negative,project(la,positive,component)),la.mult(positive,-project(la,negative,component)));
}
template<typename Expr, ArifmetycOp Coef>
Expr set_equal(LinearAlgebra<Expr,Coef> la, Expr expr, Expr component, Expr equal){
    Coef comp_part=project(la,expr,component);
    Expr indepedent=la.add(expr,la.mult(component,-comp_part));
    Expr res=la.add(indepedent,la.mult(equal,comp_part));
    return res;
}
template<typename Expr, ArifmetycOp Coef>
Expr perpendicular_component(LinearAlgebra<Expr,Coef> la, Expr expr, Expr parallel){
    Coef k=project(la,expr,parallel);
    Coef mk=-k;
    Expr mult=la.mult(parallel,mk);
    return la.add(expr,mult);
}
template<ArifmetycOp Coef,typename Expr> requires (LinearOp<Coef,Expr>)
LinearAlgebra<Expr,Coef> make_algebra(std::function<Coef(Expr,Expr)> like_dot_product){
    return LinearAlgebra<Expr,Coef>{
        [](Expr v, Coef c){
            return v*c;
        },
        [](Expr a, Expr b){
            return a+b;
        },
        [=](Expr a, Expr b){
            return like_dot_product(a,b);
        }
    };
}
template<Ordable Coef>
LinearOrder<Coef> make_order(){
    return LinearOrder<Coef>{
        [](Coef c){
            if(c<0){
                return CompRes::Less;
            }else if(c==0){
                return CompRes::Eq;
            }else{
                return CompRes::More;
            }
        },
        [](Coef a, Coef b){
            if(a<b){
                return CompRes::Less;
            }else if(a==b){
                return CompRes::Eq;
            }else{
                return CompRes::More;
            }
        }
    };
}
template<Ordable Coef, typename Expr>
bool is_zero(LinearAlgebra<Expr,Coef> la, Expr expr){
    return like_sqrt_len(la,expr)==0;
}
template<typename Expr, typename Coef>
Expr get_indepedent_to_ortogonal(LinearAlgebra<Expr,Coef> la, Expr vec, std::vector<Expr> ortogonal_space){
    Expr res=vec;
    for(Expr i:ortogonal_space){
        res=perpendicular_component(la,res,i);
    }
    return res;
}
template<typename Expr, typename Coef>
std::vector<Expr> ortanogalise_space(LinearAlgebra<Expr,Coef> la, std::vector<Expr> space){
    std::vector<Expr> ortagonalised;
    for(Expr i:space){
        ortagonalised.push_back(get_indepedent_to_ortogonal(la,i,ortagonalised));
    }
    return ortagonalised;
}

}
#endif // LINEAR_ALGEBRA_UTILITES_H
