/*
 * function_parse_architect.hh
 *
 *  Created on: Mar 30, 2025
 *      Author: aleksandr
 */

#ifndef FUNCTION_PARSE_ARCHITECT_HH_
#define FUNCTION_PARSE_ARCHITECT_HH_

#include <functional>
#include <vector>
#include <memory>
#include <variant>
using namespace std;
namespace common_parsing{
typedef shared_ptr<string> Text;
typedef size_t Pos;
template<typename Res, typename Context, typename Operand, typename Operator>
using OpFun=function<Res(vector<Operand>,vector<Operator>,Context)>;
template<typename Res, typename Arg, typename Context>
using Fun=function<Res(vector<Arg>,Context)>;
template<typename Res, typename Literal, typename Context>
using LiteralIntreprete=function<Res(Literal, Context)>;
template<typename Res, typename Context>
using Intreprete=function<Res(Context)>;
template<typename Res, typename Context>
struct ParseNode{
	Pos start;
	Pos end;
	Text target;
	Intreprete<Res,Context> intreprete;
};
template<typename Info>
struct NotParseNode{
	Pos start;
	Text target;
	Info info;
};
template<typename Res, typename Context, typename NoParseInfo>
using ParseResult=variant<NotParseNode<NoParseInfo>,ParseNode<Res,Context>>;
template<typename Res, typename Context, typename NoParseInfo>
bool isOk(ParseResult<Res,Context,NoParseInfo> res){
	return res.index()!=0;
}
template<typename Res, typename Context, typename NoParseInfo>
using Parse=function<ParseResult<Res,Context,NoParseInfo>(Text,Pos)>;
}
#endif /* FUNCTION_PARSE_ARCHITECT_HH_ */
