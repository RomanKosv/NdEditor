#ifndef TRANSFORMATION_SUPPORT_H
#define TRANSFORMATION_SUPPORT_H
#include "parsing_funs_ver_2.h"

namespace concrete_parsing_ver_2 {
tuple<Transform, EvalMaybe<ExprResSucces>> intreprete_transform(StdContext c, ParseNode<EvalMaybe<ExprResSucces>,StdContext> node);
tuple<StdContext, Transform> make_transform_all_context(StdContext c);
BoolFun make_transform_fun(Transform t, Figure cond, StdContext context);
ParseResult<EvalMaybe<BoolFun>, StdContext, NoParseSintaxInfo> p_transform(Text target, Pos start);
}
#endif // TRANSFORMATION_SUPPORT_H
