#ifndef STD_PARSING_FUNS_H
#define STD_PARSING_FUNS_H
#include "../parser/std_context.h"
#include "../parser/parsing_errors.h"
using common_parsing::ParseResult;
using common_parsing::Parse;
using common_parsing::None;
namespace concrete_parsing_1{
struct ParseFigureRes{
    std::variant<ParseSyntaxError<EvalMaybe<Figure>,StdContext>,EvalMaybe<Figure>> val;
};
ParseFigureRes parse_figure(Text target,StdContext context);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_layer(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_or_layer(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_and_layer(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_brackets_layer(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_brackets(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_less(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_less_or_eq(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_more(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_more_or_eq(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_eq(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_no_eq(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_var(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_fun(Text target, Pos start);
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_not(Text target, Pos start);
ParseResult<string,StdContext,NoParseSintaxInfo> p_name(Text target, Pos start);
// static Parse<None,None,TerminalNoParseInfo> p_less_op;
// static Parse<None,None,TerminalNoParseInfo> p_less_or_eq_op;
// static Parse<None,None,TerminalNoParseInfo> p_more_op;
// static Parse<None,None,TerminalNoParseInfo> p_more_or_eq_op;
// static Parse<None,None,TerminalNoParseInfo> p_eq_op;
// static Parse<None,None,TerminalNoParseInfo> p_no_eq_op;
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_eval_layer(Text target, Pos start);
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_fun(Text target, Pos start);
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_brackets(Text target, Pos start);
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_var(Text target, Pos start);
//static Parse<None,None,TerminalNoParseInfo> p_not_op;
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_layer(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_add_layer(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_mult_layer(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_brackets_layer(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_brackets(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_fun(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_literal(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_var(Text target, Pos start);
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_unary_minus(Text target, Pos start);
}
#endif // STD_PARSING_FUNS_H
