#ifndef TEST_INEQUALITY_SYSTEM_PARSING_1_H
#define TEST_INEQUALITY_SYSTEM_PARSING_1_H
#include "../parser/std_parsing_funs.h"
using namespace concrete_parsing_1;
template<typename Res>
string node_to_str(ParseResult<EvalMaybe<Res>,StdContext,NoParseSintaxInfo> res, StdContext c);
void test_inequalyty_systems_and_num_expr_1();

#endif // TEST_INEQUALITY_SYSTEM_PARSING_1_H
