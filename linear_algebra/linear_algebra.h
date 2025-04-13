#ifndef LINEAR_ALGEBRA_H
#define LINEAR_ALGEBRA_H

#include <functional>
namespace nd_geometry{
template<typename Dim, typename Expr>
struct SpaceFactory
{
public:
    std::function<Dim()> new_dim;
    std::function<Dim()> get_scale_dim;
    std::function<Expr(Dim)> get_1;
    std::function<Expr()> get_zero_expr;
};
template<typename Expression, typename Coeficent>
struct LinearAlgebra{
public:
    std::function<Expression(Expression,Coeficent)> mult;
    std::function<Expression(Expression,Expression)> add;
    std::function<Coeficent(Expression,Expression)> like_dot_product;
};
enum class CompRes{
    Less,
    Eq,
    More
};
template<typename Coeficent>
struct LinearOrder{
    std::function<CompRes(Coeficent)> compare_to_zero;//нужно порой
    std::function<CompRes(Coeficent,Coeficent)> compare_first_to_second;//не уверен что нужно
};
template<typename T>
concept ArifmetycOp = requires (T t) {
    T {-t};
    T {t/t};
    T {t*t};
    T {t+t};
    T {t-t};
};
template<typename Coef, typename Vec>
concept LinearOp = requires (Coef c, Vec v) {
    Vec {-v};
    Vec {v*c};
    Vec {v/c};
    Vec {v+v};
    Vec {v-v};
};
template<typename T>
concept Ordable = requires (T t) {
    bool {t<0};
    bool {t>0};
    bool {t==0};
    bool {t<=0};
    bool {t>=0};
    bool {t<t};
    bool {t>t};
    bool {t==t};
    bool {t<=t};
    bool {t>=t};
};
}
#endif // LINEAR_ALGEBRA_H
