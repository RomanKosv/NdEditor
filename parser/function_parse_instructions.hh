/*
 * function_parse_instructions.hh
 *
 *  Created on: Mar 31, 2025
 *      Author: aleksandr
 */

#ifndef FUNCTION_PARSE_INSTRUCTIONS_HH_
#define FUNCTION_PARSE_INSTRUCTIONS_HH_

#include "function_parse_architect.hh"
#include "outparse.hh"
#include "parse_terminals.hh"

namespace common_parsing{
static Log PARSE_FUN_LOGGER{false,"[PARSE_FUN_INSTRUCTIONS_LOGGER]"};
static const auto LEFT_BRACKET=parse_terminal("(");
static const auto RIGHT_BRACKET=parse_terminal(")");
static const auto COMMA=parse_terminal(",");
template<typename Res, typename Arg, typename Context, typename NoParseInfo>
struct FunCallParseInstruction{
    Parse<Fun<Res,Arg,Context>,Context,NoParseInfo> parse_fn;
    Parse<Arg,Context,NoParseInfo> parse_arg;
    ParseResult<vector<Arg>,Context,TerminalNoParseInfo> parse_args_part(Text target, Pos start)const{
        PARSE_FUN_LOGGER.print("parse_args_part");
        auto brackets_fail=std::make_shared<std::vector<NotParseNode<TerminalNoParseInfo>>>(
            vector<NotParseNode<TerminalNoParseInfo>>{});
        auto brackets_success=std::make_shared<std::vector<ParseNode<None,None>>>(std::vector<ParseNode<None,None>>{});
        auto arg_fail=std::make_shared<std::vector<NotParseNode<NoParseInfo>>>(
            vector<NotParseNode<NoParseInfo>>{});
        auto arg_succes=std::make_shared<vector<ParseNode<Arg,Context>>>(vector<ParseNode<Arg,Context>>{});
        auto left=wrap_outparse_2_lists(brackets_success,brackets_fail,LEFT_BRACKET);
        auto arg=wrap_outparse_2_lists(arg_succes,arg_fail,parse_arg);
        auto right=wrap_outparse_2_lists(brackets_success,brackets_fail,RIGHT_BRACKET);
        auto comma=wrap_skip(COMMA);
        auto do_parse=skip_seq({left,maybe_outparse(skip_seq({arg,outparse_rep(skip_seq({comma,arg}))})),right});
        OutParseRes outres=do_parse(target,start);
        if(brackets_fail->size()!=0){
            return brackets_fail->back();
        }else{
            return compress_parse_vector(*arg_succes,target,(*brackets_success)[0].start,(*brackets_success)[1].end);
        }
    }
    Parse<vector<Arg>,Context,TerminalNoParseInfo> get_parse_args_part()const{
        FunCallParseInstruction<Res,Arg,Context,NoParseInfo> copy=*this;
        return [=](Text target, Pos start){
            return copy.parse_args_part(target,start);
        };
    }
    ParseResult<Res,Context,NoParseInfo> parse_fun_call(Text target, Pos start) const{
        PARSE_FUN_LOGGER.print("parse_fun_call");
        auto fun_succes=make_shared<vector<ParseNode<Fun<Res,Arg,Context>,Context>>>(vector<ParseNode<Fun<Res,Arg,Context>,Context>>{});
        auto fun_fail=make_shared<vector<NotParseNode<NoParseInfo>>>(vector<NotParseNode<NoParseInfo>>{});
        auto brackets_success=std::make_shared<std::vector<ParseNode<vector<Arg>,Context>>>(std::vector<ParseNode<vector<Arg>,Context>>{});
        auto brackets_fail=std::make_shared<std::vector<NotParseNode<TerminalNoParseInfo>>>(
            vector<NotParseNode<TerminalNoParseInfo>>{});
        outparse do_outparse=skip_seq({wrap_outparse_2_lists(fun_succes,fun_fail,parse_fn),
                                            wrap_outparse_2_lists(brackets_success,brackets_fail,get_parse_args_part())});
        OutParseRes outres=do_outparse(target,start);
        if(fun_fail->size()!=0){
            PARSE_FUN_LOGGER.print("parse_fun_call/fun_fail->size()!=0");
            return fun_fail->back();
        }else if(brackets_fail->size()!=0){
            PARSE_FUN_LOGGER.print("parse_fun_call/fun_fail->size()=0/brackets_fail->size()!=0");
            return ParseResult<Res,Context,NoParseInfo>{NotParseNode<NoParseInfo>{brackets_fail->back().start,target,brackets_fail->back().info}};
        }else{
            PARSE_FUN_LOGGER.print("parse_fun_call/fun_fail->size()=0/brackets_fail->size()=0/(SUCCESS)");
            Intreprete<Res,Context> intreprete=[=](Context context){
                return fun_succes->back().intreprete(context)(brackets_success->back().intreprete(context),context);
            };
            ParseNode<Res,Context> res;
            res.start=fun_succes->back().start;
            res.end=brackets_success->back().end;
            res.target=target;
            res.intreprete=intreprete;
            return res;
        }
    }
    Parse<Res,Context,NoParseInfo> get_parse_fun_call()const{
        FunCallParseInstruction<Res,Arg,Context,NoParseInfo> copy=*this;
        return [=](Text target, Pos start){
            return copy.parse_fun_call(target,start);
        };
    }
};
}

#endif /* FUNCTION_PARSE_INSTRUCTIONS_HH_ */
