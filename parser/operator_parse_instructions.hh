/*
 * operator_parse_instructions.hh
 *
 *  Created on: Mar 31, 2025
 *      Author: aleksandr
 */

#ifndef OPERATOR_PARSE_INSTRUCTIONS_HH_
#define OPERATOR_PARSE_INSTRUCTIONS_HH_


#include <vector>
#include "../parser/debug.hh"
#include "function_parse_architect.hh"
namespace common_parsing{
static Log OPERATOR_PARSE_LOGGER{false,"[OPERATOR_PARSE_INSTRUCTIONS_LOGGER]"};
template<typename Res, typename Operand, typename Operator,
typename Context, typename NoParseInfo>
struct OperatorParseInstruction{
	Parse<Operand,Context,NoParseInfo> parse_operand;
	Parse<Operator, Context, NoParseInfo> parse_operator;
	OpFun<Res, Context, Operand, Operator> intreprete_fun;
};
template<typename Res, typename Operand, typename Operator,
typename Context, typename NoParseInfo>
ParseResult<Res,Context,NoParseInfo> UseOperatorParseInstruction(Text target, Pos start,
		OperatorParseInstruction<Res,Operand,Operator,Context,NoParseInfo> instruction)
{
    OPERATOR_PARSE_LOGGER.print("UseOperatorParseInstruction");
    ParseResult<Operand,Context,NoParseInfo> res1=instruction.parse_operand(target,start);
    if(!isOk(res1)){
        return get<NotParseNode<NoParseInfo>>(res1);
    }else{
        Pos pos=get<ParseNode<Operand,Context>>(res1).end;
        Pos succes_pos=pos;
        vector<Intreprete<Operand,Context>> operand_list_1({get<ParseNode<Operand,Context>>(res1).intreprete});
        vector<Intreprete<Operator,Context>> operator_list_2;
        bool stop=false;
        do{
            ParseResult<Operator,Context,NoParseInfo> operator_res=instruction.parse_operator(target,pos);
            if(!isOk(operator_res)){
                stop=true;
            }else{
                pos=get<ParseNode<Operator,Context>>(operator_res).end;
                ParseResult<Operand,Context,NoParseInfo> operand_res=instruction.parse_operand(target,pos);
                if(!isOk(operand_res)){
                    stop=true;
                }else{
                    pos=get<ParseNode<Operand,Context>>(operand_res).end;
                    operand_list_1.push_back(get<ParseNode<Operand,Context>>(operand_res).intreprete);
                    operator_list_2.push_back(get<ParseNode<Operator,Context>>(operator_res).intreprete);
                    succes_pos=pos;
                }
            }
        }while(!stop);
        Intreprete<Res,Context> intreprete=[=](Context context){
            vector<Operator> operators2;
            for(auto i:operator_list_2){
                operators2.push_back(i(context));
            }
            vector<Operand> operands1;
            for(auto i:operand_list_1){
                operands1.push_back(i(context));
            }
            return instruction.intreprete_fun(operands1,operators2,context);
        };
        ParseNode<Res,Context> node;
        node.start=start;
        node.end=succes_pos;
        node.target=target;
        node.intreprete=intreprete;
        return node;
    }
}
template<typename Res, typename Operand, typename Operator,
typename Context, typename NoParseInfo>
Parse<Res,Context, NoParseInfo> parse_operator(OperatorParseInstruction<Res,Operand,Operator,Context,NoParseInfo> instruction){
    OPERATOR_PARSE_LOGGER.print("parse_operator");
	return [=](Text target, Pos start){
		return UseOperatorParseInstruction(target,start,instruction);
	};
}
}

#endif /* OPERATOR_PARSE_INSTRUCTIONS_HH_ */
