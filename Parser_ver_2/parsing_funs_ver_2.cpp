#include "parsing_funs_ver_2.h"
namespace concrete_parsing_ver_2{
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_layer(Text target, Pos start){
    return p_add_layer(target,start);
};
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_add_layer(Text target, Pos start){
    common_parsing::OperatorParseInstruction<EvalMaybe<NumExpr>,EvalMaybe<NumExpr>,SumOp,StdContext,NoParseSintaxInfo> instruction{
        p_mult_layer,
        p_add_op,
        [](vector<EvalMaybe<NumExpr>> operands,vector<SumOp> operators,StdContext context){
            return context.eval_sum_op(operands,operators);
        }
    };
    return common_parsing::UseOperatorParseInstruction(target,start,instruction);
};
ParseResult<SumOp,StdContext,NoParseSintaxInfo> p_add_op(Text target, Pos start){
    Maybe<Pos> res_add=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal("+"))})(target,start);
    Maybe<Pos> res_rem=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal("-"))})(target,start);
    if(!res_add.isEmpty()){
        return ParseResult<SumOp,StdContext,NoParseSintaxInfo>{
            ParseNode<SumOp,StdContext>{start,res_add.get_ok(),target,[](StdContext){return SumOp::add;}}
        };
    }else if(!res_rem.isEmpty()){
        return ParseResult<SumOp,StdContext,NoParseSintaxInfo>{
            ParseNode<SumOp,StdContext>{start,res_rem.get_ok(),target,[](StdContext){return SumOp::rem;}}
        };
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant parse +|-"}};
    }
};
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_mult_layer(Text target, Pos start){
    common_parsing::OperatorParseInstruction<EvalMaybe<NumExpr>,EvalMaybe<NumExpr>,MultOp,StdContext,NoParseSintaxInfo> instruction{
        p_num_brackets_layer,
        p_mult_op,
        [](vector<EvalMaybe<NumExpr>> operands,vector<MultOp> operators,StdContext context){
            return context.eval_mult_op(operands,operators);
        }
    };
    return common_parsing::UseOperatorParseInstruction(target,start,instruction);
};
ParseResult<MultOp,StdContext,NoParseSintaxInfo> p_mult_op(Text target, Pos start){
    Maybe<Pos> res_mult=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal("*"))})(target,start);
    Maybe<Pos> res_div=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal("/"))})(target,start);
    if(!res_mult.isEmpty()){
        return ParseResult<MultOp,StdContext,NoParseSintaxInfo>{
            ParseNode<MultOp,StdContext>{start,res_mult.get_ok(),target,[](StdContext){return MultOp::mult;}}
        };
    }else if(!res_div.isEmpty()){
        return ParseResult<MultOp,StdContext,NoParseSintaxInfo>{
            ParseNode<MultOp,StdContext>{start,res_div.get_ok(),target,[](StdContext){return MultOp::div;}}
        };
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant parse *|/"}};
    }
};
template<typename Res>
ParseResult<Res,StdContext,NoParseSintaxInfo> translate(ParseResult<Res,StdContext,VariantError<NoParseSintaxInfo>> res){
    if(common_parsing::isOk(res)){
        return ParseResult<Res,StdContext,NoParseSintaxInfo>{get<ParseNode<Res,StdContext>>(res)};
    }else{
        NotParseNode<VariantError<NoParseSintaxInfo>> node=get<NotParseNode<VariantError<NoParseSintaxInfo>>>(res);
        return ParseResult<Res,StdContext,NoParseSintaxInfo>{NotParseNode<NoParseSintaxInfo>{node.start,node.target,node.info}};
    }
}
template<typename Res>
Parse<Res,StdContext,NoParseSintaxInfo> translate_parse(Parse<Res,StdContext,VariantError<NoParseSintaxInfo>> parse){
    return [parse](Text target, Pos start){
        return translate(parse(target,start));
    };
}
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_brackets_layer(Text target, Pos start){
    return translate(common_parsing::parse_variants<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo>(
        {p_num_brackets,p_num_fun,p_num_literal,p_num_var,p_unary_minus})(target,start));
};
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_brackets(Text target, Pos start){
    common_parsing::outparse parse_left=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal("("))});
    common_parsing::outparse parse_right=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal(")"))});
    Maybe<Pos> left_res=parse_left(target,start);
    if(!left_res.isEmpty()){
        ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> res=p_num_layer(target,left_res.get_ok());
        if(isOk(res)){
            Maybe<Pos> right_res=parse_right(target,get<ParseNode<EvalMaybe<NumExpr>,StdContext>>(res).end);
            if(!right_res.isEmpty()){
                return ParseNode<EvalMaybe<NumExpr>,StdContext>{start,right_res.get_ok(),target,get<ParseNode<EvalMaybe<NumExpr>,StdContext>>(res).intreprete};
            }else{
                return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read ')"}};//не верно
            }
        }else{
            return NotParseNode<NoParseSintaxInfo>{start,target,get<NotParseNode<NoParseSintaxInfo>>(res).info};//не верно
        }
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read '("}};
    }
};
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_fun(Text target, Pos start){
    common_parsing::FunCallParseInstruction<EvalMaybe<NumExpr>,EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> call{
        p_num_fun_name,
        p_eval_layer
    };
    return call.parse_fun_call(target,start);
};

ParseResult<Fun<EvalMaybe<NumExpr>,EvalMaybe<ExprResSucces>,StdContext>,StdContext,NoParseSintaxInfo>
p_num_fun_name(Text target, Pos start){
    ParseResult<string,StdContext,NoParseSintaxInfo> name=p_name(target,start);
    if(!isOk(name)){
        return get<NotParseNode<NoParseSintaxInfo>>(name);
    }else{
        ParseNode<string,StdContext> node=get<ParseNode<string,StdContext>>(name);
        return ParseNode<Fun<EvalMaybe<NumExpr>,EvalMaybe<ExprResSucces>,StdContext>,StdContext>{
            start,
            node.end,
            target,
            [node](StdContext c)->Fun<EvalMaybe<NumExpr>,EvalMaybe<ExprResSucces>,StdContext>{
                EvalMaybe<NumFun> fun=c.get_num_fun(node.intreprete(c));
                if(fun.isOk()){
                    return fun.getOk().fun;
                }else{
                    EvalError err=fun.getErr();
                    return [err](vector<EvalMaybe<ExprResSucces>>,StdContext){
                        return EvalMaybe<NumExpr>(err);
                    };
                }
            }
        };
    }
};
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_literal(Text target, Pos start){
    common_parsing::outparse parse_num=common_parsing::wrap_skip(common_parsing::parse_variants<None,None,TerminalNoParseInfo>({
        common_parsing::parse_terminal("0"),
        common_parsing::parse_terminal("1"),
        common_parsing::parse_terminal("2"),
        common_parsing::parse_terminal("3"),
        common_parsing::parse_terminal("4"),
        common_parsing::parse_terminal("5"),
        common_parsing::parse_terminal("6"),
        common_parsing::parse_terminal("7"),
        common_parsing::parse_terminal("8"),
        common_parsing::parse_terminal("9")
    }));
    common_parsing::outparse parse_double=common_parsing::outparse_seq(
        {
            common_parsing::skip_spaces,
            parse_num,
            common_parsing::outparse_rep(parse_num),
            common_parsing::maybe_outparse(common_parsing::outparse_seq(
                {
                 common_parsing::wrap_skip(common_parsing::parse_terminal(".")),
                 parse_num,
                 common_parsing::outparse_rep(parse_num)})),
            common_parsing::skip_spaces
        });
    Maybe<Pos> pres=parse_double(target,start);
    if(pres.isEmpty()){
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read num"}};
    }else{
        return ParseNode<EvalMaybe<NumExpr>,StdContext>{
                                                         start,
                                                         pres.get_ok(),
                                                         target,
                                                         [start,target,pres](StdContext context)->EvalMaybe<NumExpr>{
                                                             return EvalMaybe<NumExpr>(context.space.get_scale()*stod(target->substr(start,pres.get_ok()-start)));}};
    }
};
ParseResult<string,StdContext,NoParseSintaxInfo> p_name(Text target, Pos start){
    std::regex r(R"(\w|\d)");
    start=common_parsing::skip_spaces(target,start).get_ok();
    if(!std::regex_match(target->substr(start,1),std::regex(R"([a-z]|[A-Z]|_)"))){
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cannot read var name"}};
    }
    else{
        Pos i=start;
        while(std::regex_match(target->substr(i,1),r)){
            i++;
        }
        string str=target->substr(start,i-start);
        return ParseNode<string,StdContext>{start,i,target,[str](StdContext){return str;}};
    }
};
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_num_var(Text target, Pos start){
    ParseResult<string,StdContext,NoParseSintaxInfo> name=p_name(target,start);
    if(common_parsing::isOk(name)){
        ParseNode<string,StdContext> node=get<ParseNode<string,StdContext>>(name);
        return ParseNode<EvalMaybe<NumExpr>,StdContext>{start,node.end,target,
                                                         [node](StdContext context){return context.get_numvar(node.intreprete(context));}};
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read var name"}};
    }
};
ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> p_unary_minus(Text target, Pos start){
    ParseResult<SumOp,StdContext,NoParseSintaxInfo> sum=p_add_op(target,start);
    if(common_parsing::isOk(sum)){
        Pos sum_end=get<ParseNode<SumOp,StdContext>>(sum).end;
        ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> num=p_num_brackets_layer(target,sum_end);
        if(common_parsing::isOk(num)){
            ParseNode<SumOp,StdContext> sum_node=get<ParseNode<SumOp,StdContext>>(sum);
            ParseNode<EvalMaybe<NumExpr>,StdContext> num_node=get<ParseNode<EvalMaybe<NumExpr>,StdContext>>(num);
            return ParseNode<EvalMaybe<NumExpr>,StdContext>{
                                                             start,
                                                             num_node.end,
                                                             target,
                                                             [num_node,sum_node](StdContext c)->EvalMaybe<NumExpr>{
                                                                 return c.eval_sum_op(
                                                                     {EvalMaybe<NumExpr>{c.space.get_zero()},num_node.intreprete(c)},
                                                                     {sum_node.intreprete(c)});}};
        }else{
            return NotParseNode<NoParseSintaxInfo>{start,target,get<NotParseNode<NoParseSintaxInfo>>(num).info};
        }
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,get<NotParseNode<NoParseSintaxInfo>>(sum).info};
    }
};
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_eval_layer(Text target, Pos start){
    return translate(parse_first_max_variant<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo>({p_common_brackets,p_common_fun,p_common_var,p_common_wrap_logic,p_common_wrap_num})(target,start));
};
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_fun(Text target, Pos start){
    common_parsing::FunCallParseInstruction<EvalMaybe<ExprResSucces>,EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> call{
        p_common_fun_name,
        p_eval_layer
    };
    return call.parse_fun_call(target,start);
};
ParseResult<Fun<EvalMaybe<ExprResSucces>,EvalMaybe<ExprResSucces>,StdContext>,StdContext,NoParseSintaxInfo>
p_common_fun_name(Text target, Pos start){
    ParseResult<string,StdContext,NoParseSintaxInfo> name=p_name(target,start);
    if(!isOk(name)){
        return get<NotParseNode<NoParseSintaxInfo>>(name);
    }else{
        ParseNode<string,StdContext> node=get<ParseNode<string,StdContext>>(name);
        return ParseNode<Fun<EvalMaybe<ExprResSucces>,EvalMaybe<ExprResSucces>,StdContext>,StdContext>{
            start,
            node.end,
            target,
            [node](StdContext c)->Fun<EvalMaybe<ExprResSucces>,EvalMaybe<ExprResSucces>,StdContext>{
                EvalMaybe<EvalFun> fun=c.get_common_fun(node.intreprete(c));
                if(fun.isOk()){
                    return c.wrap_common_fun(fun.getOk());
                }else{
                    EvalError err=fun.getErr();
                    return [err](vector<EvalMaybe<ExprResSucces>>,StdContext){
                        return EvalMaybe<ExprResSucces>(err);
                    };
                }
            }
        };
    }
};
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_brackets(Text target, Pos start){
    common_parsing::outparse parse_left=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal("("))});
    common_parsing::outparse parse_right=common_parsing::skip_seq({common_parsing::wrap_skip(common_parsing::parse_terminal(")"))});
    Maybe<Pos> left_res=parse_left(target,start);
    if(!left_res.isEmpty()){
        ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> res=p_eval_layer(target,left_res.get_ok());
        if(isOk(res)){
            Maybe<Pos> right_res=parse_right(target,get<ParseNode<EvalMaybe<ExprResSucces>,StdContext>>(res).end);
            if(!right_res.isEmpty()){
                return ParseNode<EvalMaybe<ExprResSucces>,StdContext>{start,right_res.get_ok(),target,get<ParseNode<EvalMaybe<ExprResSucces>,StdContext>>(res).intreprete};
            }else{
                return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read ')"}};//не верно
            }
        }else{
            return NotParseNode<NoParseSintaxInfo>{start,target,get<NotParseNode<NoParseSintaxInfo>>(res).info};//не верно
        }
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read '("}};
    }
};
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_var(Text target, Pos start){
    ParseResult<string,StdContext,NoParseSintaxInfo> name=p_name(target,start);
    if(common_parsing::isOk(name)){
        ParseNode<string,StdContext> node=get<ParseNode<string,StdContext>>(name);
        return ParseNode<EvalMaybe<ExprResSucces>,StdContext>{start,node.end,target,
                                                               [node](StdContext context){return context.get_common_var(node.intreprete(context));}};
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read var name"}};
    }
};
template<typename Res, typename T,typename Context, typename Info>
ParseResult<Res,Context,Info> parse_next(ParseResult<T,Context,Info> res, function<ParseResult<Res,Context,Info>(ParseNode<T,Context>)> next){
    if(!isOk(res)){
        return get<NotParseNode<Info>>(res);
    }else{
        return next(get<ParseNode<T,Context>>(res));
    }
}
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_wrap_logic(Text target, Pos start){
    ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> figure=p_logic_layer(target,start);
    if(!isOk(figure)){
        return get<NotParseNode<NoParseSintaxInfo>>(figure);
    }else{
        ParseNode<EvalMaybe<Figure>,StdContext> node=get<ParseNode<EvalMaybe<Figure>,StdContext>>(figure);
        return ParseNode<EvalMaybe<ExprResSucces>,StdContext>{start,node.end,target,[node](StdContext c)->EvalMaybe<ExprResSucces>{
                                                                   EvalMaybe<Figure> res=node.intreprete(c);
                                                                   if(res.isOk()){
                                                                       return EvalMaybe<ExprResSucces>{ExprResSucces{res.getOk()}};
                                                                   }else{
                                                                       return EvalMaybe<ExprResSucces>{res.getErr()};
                                                                   }
                                                               }};
    }
};
ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> p_common_wrap_num(Text target, Pos start){
    ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> num=p_num_layer(target,start);
    if(!isOk(num)){
        return get<NotParseNode<NoParseSintaxInfo>>(num);
    }else{
        ParseNode<EvalMaybe<NumExpr>,StdContext> node=get<ParseNode<EvalMaybe<NumExpr>,StdContext>>(num);
        return ParseNode<EvalMaybe<ExprResSucces>,StdContext>{start,node.end,target,[node](StdContext c)->EvalMaybe<ExprResSucces>{
                                                                   EvalMaybe<NumExpr> res=node.intreprete(c);
                                                                   if(res.isOk()){
                                                                       return EvalMaybe<ExprResSucces>{ExprResSucces{res.getOk()}};
                                                                   }else{
                                                                       return EvalMaybe<ExprResSucces>{res.getErr()};
                                                                   }
                                                               }};
    }
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_layer(Text target, Pos start){
    return p_or_layer(target,start);
};
Parse<None,StdContext,NoParseSintaxInfo> p_term(Parse<None,None,TerminalNoParseInfo> parse){
    return [parse](Text target, Pos start)->ParseResult<None,StdContext,NoParseSintaxInfo>{
        ParseResult<None,None,TerminalNoParseInfo> res=parse(target,start);
        if(isOk(res)){
            ParseNode<None,None> node=get<ParseNode<None,None>>(res);
            return ParseNode<None,StdContext>{node.start,node.end,target,[node](StdContext){return node.intreprete(None{});}};
        }else{
            NotParseNode<TerminalNoParseInfo> node=get<NotParseNode<TerminalNoParseInfo>>(res);
            return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{node}};
        }
    };
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_or_layer(Text target, Pos start){
    OperatorParseInstruction<EvalMaybe<Figure>,EvalMaybe<Figure>,OrOp,StdContext,NoParseSintaxInfo> op{
        p_and_layer,
        p_term(common_parsing::wrap_skip_parse_tarminal("|")),
        [](vector<EvalMaybe<Figure>> args,vector<OrOp> op, StdContext c)->EvalMaybe<Figure>{
            return c.eval_or_op(args,op);
        }
    };
    return common_parsing::UseOperatorParseInstruction(target,start,op);
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_and_layer(Text target, Pos start){
    OperatorParseInstruction<EvalMaybe<Figure>,EvalMaybe<Figure>,AndOp,StdContext,NoParseSintaxInfo> op{
        p_logic_brackets_layer,
        p_term(common_parsing::wrap_skip_parse_tarminal("&")),
        [](vector<EvalMaybe<Figure>> args,vector<AndOp> op, StdContext c)->EvalMaybe<Figure>{
            return c.eval_and_op(args,op);
        }
    };
    return common_parsing::UseOperatorParseInstruction(target,start,op);
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_brackets_layer(Text target, Pos start){
    return translate(common_parsing::parse_variants<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo>({p_logic_brackets,p_comp,p_logic_fun,p_logic_var,p_not})(target,start));
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_brackets(Text target, Pos start){
    auto errs=make_shared<vector<NotParseNode<NoParseSintaxInfo>>>(vector<NotParseNode<NoParseSintaxInfo>>{});
    auto arg=make_shared<vector<ParseNode<EvalMaybe<Figure>,StdContext>>>(vector<ParseNode<EvalMaybe<Figure>,StdContext>>{});
    auto brackets=make_shared<vector<ParseNode<None,StdContext>>>(vector<ParseNode<None,StdContext>>{});
    common_parsing::outparse seq=common_parsing::skip_seq({
        common_parsing::wrap_outparse_2_lists(brackets,errs,p_term(common_parsing::parse_terminal("("))),
        common_parsing::wrap_outparse_2_lists<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo>(arg,errs,p_logic_layer),
        common_parsing::wrap_outparse_2_lists(brackets,errs,p_term(common_parsing::parse_terminal(")")))
    });
    Maybe<Pos> res_seq=seq(target,start);
    if(res_seq.isEmpty()){
        return errs->back();
    }else{
        return ParseNode<EvalMaybe<Figure>,StdContext>{start,brackets->back().end,target,arg->back().intreprete};
    }
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_comp(Text target, Pos start){
    auto errs=make_shared<vector<NotParseNode<NoParseSintaxInfo>>>(vector<NotParseNode<NoParseSintaxInfo>>{});
    auto args=make_shared<vector<ParseNode<EvalMaybe<NumExpr>,StdContext>>>(vector<ParseNode<EvalMaybe<NumExpr>,StdContext>>{});
    auto op=make_shared<vector<ParseNode<CompOp,StdContext>>>(vector<ParseNode<CompOp,StdContext>>{});
    common_parsing::outparse seq=common_parsing::skip_seq({
        common_parsing::wrap_outparse_2_lists<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo>(args,errs,p_num_layer),
        common_parsing::wrap_outparse_2_lists(op,errs,translate_parse(common_parsing::parse_variants<CompOp,StdContext,NoParseSintaxInfo>({p_less_or_eq,p_less,p_more_or_eq,p_more,p_eq,p_no_eq}))),
        common_parsing::wrap_outparse_2_lists<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo>(args,errs,p_num_layer)
    });
    Maybe<Pos> seq_res=seq(target,start);
    if(seq_res.isEmpty()){
        return errs->back();
    }else{
        return ParseNode<EvalMaybe<Figure>,StdContext>{start,args->back().end,target,
            [args,op](StdContext c)->EvalMaybe<Figure>{
                return c.comp((*args)[0].intreprete(c),(*args)[1].intreprete(c),op->back().intreprete(c));
            }
        };
    }
};
ParseResult<CompOp,StdContext,NoParseSintaxInfo> p_less(Text target, Pos start){
    ParseResult<None,StdContext,NoParseSintaxInfo> res=p_term(common_parsing::wrap_skip_parse_tarminal("<"))(target,start);
    if(isOk(res)){
        return ParseNode<CompOp,StdContext>{start,get<ParseNode<None,StdContext>>(res).end,target,[](StdContext){return CompOp::less;}};
    }else{
        return get<NotParseNode<NoParseSintaxInfo>>(res);
    }
};
ParseResult<CompOp,StdContext,NoParseSintaxInfo> p_less_or_eq(Text target, Pos start){
    ParseResult<None,StdContext,NoParseSintaxInfo> res=p_term(common_parsing::wrap_skip_parse_tarminal("<="))(target,start);
    if(isOk(res)){
        return ParseNode<CompOp,StdContext>{start,get<ParseNode<None,StdContext>>(res).end,target,[](StdContext){return CompOp::less_or_eq;}};
    }else{
        return get<NotParseNode<NoParseSintaxInfo>>(res);
    }
};
ParseResult<CompOp,StdContext,NoParseSintaxInfo> p_more(Text target, Pos start){
    ParseResult<None,StdContext,NoParseSintaxInfo> res=p_term(common_parsing::wrap_skip_parse_tarminal(">"))(target,start);
    if(isOk(res)){
        return ParseNode<CompOp,StdContext>{start,get<ParseNode<None,StdContext>>(res).end,target,[](StdContext){return CompOp::more;}};
    }else{
        return get<NotParseNode<NoParseSintaxInfo>>(res);
    }
};
ParseResult<CompOp,StdContext,NoParseSintaxInfo> p_more_or_eq(Text target, Pos start){
    ParseResult<None,StdContext,NoParseSintaxInfo> res=p_term(common_parsing::wrap_skip_parse_tarminal(">="))(target,start);
    if(isOk(res)){
        return ParseNode<CompOp,StdContext>{start,get<ParseNode<None,StdContext>>(res).end,target,[](StdContext){return CompOp::more_or_eq;}};
    }else{
        return get<NotParseNode<NoParseSintaxInfo>>(res);
    }
};
ParseResult<CompOp,StdContext,NoParseSintaxInfo> p_eq(Text target, Pos start){
    ParseResult<None,StdContext,NoParseSintaxInfo> res=p_term(common_parsing::wrap_skip_parse_tarminal("="))(target,start);
    if(isOk(res)){
        return ParseNode<CompOp,StdContext>{start,get<ParseNode<None,StdContext>>(res).end,target,[](StdContext){return CompOp::eq;}};
    }else{
        return get<NotParseNode<NoParseSintaxInfo>>(res);
    }
};
ParseResult<CompOp,StdContext,NoParseSintaxInfo> p_no_eq(Text target, Pos start){
    ParseResult<None,StdContext,NoParseSintaxInfo> res=p_term(common_parsing::wrap_skip_parse_tarminal("!="))(target,start);
    if(isOk(res)){
        return ParseNode<CompOp,StdContext>{start,get<ParseNode<None,StdContext>>(res).end,target,[](StdContext){return CompOp::no_eq;}};
    }else{
        return get<NotParseNode<NoParseSintaxInfo>>(res);
    }
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_var(Text target, Pos start){
    ParseResult<string,StdContext,NoParseSintaxInfo> name=p_name(target,start);
    if(common_parsing::isOk(name)){
        ParseNode<string,StdContext> node=get<ParseNode<string,StdContext>>(name);
        return ParseNode<EvalMaybe<Figure>,StdContext>{start,node.end,target,
                                                        [node](StdContext context){return context.get_logic_var(node.intreprete(context));}};
    }else{
        return NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"cant read var name"}};
    }
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_logic_fun(Text target, Pos start){
    common_parsing::FunCallParseInstruction<EvalMaybe<Figure>,EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> call{
        p_logic_fun_name,
        p_eval_layer
    };
    return call.parse_fun_call(target,start);
};
ParseResult<Fun<EvalMaybe<Figure>,EvalMaybe<ExprResSucces>,StdContext>,StdContext,NoParseSintaxInfo>
p_logic_fun_name(Text target, Pos start){
    ParseResult<string,StdContext,NoParseSintaxInfo> name=p_name(target,start);
    if(!isOk(name)){
        return get<NotParseNode<NoParseSintaxInfo>>(name);
    }else{
        ParseNode<string,StdContext> node=get<ParseNode<string,StdContext>>(name);
        return ParseNode<Fun<EvalMaybe<Figure>,EvalMaybe<ExprResSucces>,StdContext>,StdContext>{
            start,
            node.end,
            target,
            [node](StdContext c)->Fun<EvalMaybe<Figure>,EvalMaybe<ExprResSucces>,StdContext>{
                EvalMaybe<BoolFun> fun=c.get_logic_fun(node.intreprete(c));
                if(fun.isOk()){
                    return fun.getOk().fun;
                }else{
                    EvalError err=fun.getErr();
                    return [err](vector<EvalMaybe<ExprResSucces>>,StdContext){
                        return EvalMaybe<Figure>(err);
                    };
                }
            }
        };
    }
};
ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> p_not(Text target, Pos start){
    ParseResult<None,StdContext,NoParseSintaxInfo> res_not_part=p_term(common_parsing::wrap_skip_parse_tarminal("!"))(target,start);
    if(isOk(res_not_part)){
        ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> body=p_logic_brackets_layer(target,get<ParseNode<None,StdContext>>(res_not_part).end);
        if(isOk(body)){
            ParseNode<EvalMaybe<Figure>,StdContext> node=get<ParseNode<EvalMaybe<Figure>,StdContext>>(body);
            return ParseNode<EvalMaybe<Figure>,StdContext>{start,node.end,target,[node](StdContext c){return c.inverse(node.intreprete(c));}};
        }else{
            return body;
        }
    }else{
        return get<NotParseNode<NoParseSintaxInfo>>(res_not_part);
    }
};
}
