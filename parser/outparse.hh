/*
 * outparse.hh
 *
 *  Created on: Mar 30, 2025
 *      Author: aleksandr
 */

#ifndef OUTPARSE_HH_
#define OUTPARSE_HH_

#include <memory>
#include <functional>
#include "wrappers.hh"
#include "debug.hh"
#include "function_parse_architect.hh"
namespace common_parsing{
static Log OUTPARSE_LOGGER{false,"[OUTPARSE_LOGGER]"};
template<typename T>
using ptr=shared_ptr<T>;
template<typename Res, typename Context, typename NoParseInfo>
using OutParseTarget=ptr<vector<ParseResult<Res,Context,NoParseInfo>>>;
template<typename Res, typename Context, typename NoParseInfo>
OutParseTarget<Res,Context,NoParseInfo> create_outparse_target(){
    return make_shared<vector<ParseResult<Res,Context,NoParseInfo>>>({});
};
typedef Maybe<Pos> OutParseRes;
typedef std::function<OutParseRes(Text,Pos)> outparse;
template<typename Res, typename Context, typename NoParseInfo>
outparse wrap_outparse(OutParseTarget<Res,Context,NoParseInfo> target, Parse<Res,Context,NoParseInfo> parse);
template<typename Res, typename Context, typename NoParseInfo>
outparse wrap_skip(Parse<Res,Context,NoParseInfo> parse);
template<typename Res, typename Context, typename NoParseInfo>
outparse wrap_outparse_2_lists(ptr<vector<ParseNode<Res,Context>>> succes,
		ptr<vector<NotParseNode<NoParseInfo>>> fail,
                               Parse<Res,Context,NoParseInfo> parse);
template<typename Res, typename Context, typename NoParseInfo>
outparse wrap_outparse(OutParseTarget<Res,Context,NoParseInfo> target, Parse<Res,Context,NoParseInfo> parse){
    OUTPARSE_LOGGER.print("wrap_outparse");
    return [=](Text text, Pos pos)->OutParseRes{
        (*target).push_back(parse(text,pos));
        ParseResult<Res,Context,NoParseInfo> res=(*target).back();
        if(isOk(res)){
            return get<ParseNode<Res,Context>>(res).end;
        }else{
            return Maybe<Pos>();
        }
    };
}
template<typename Res, typename Context, typename NoParseInfo>
outparse wrap_skip(Parse<Res,Context,NoParseInfo> parse){
    OUTPARSE_LOGGER.print("wrap_skip");
    return [=](Text text, Pos pos)->OutParseRes{
        ParseResult<Res,Context,NoParseInfo> res=parse(text,pos);
        if(isOk(res)){
            return get<ParseNode<Res,Context>>(res).end;
        }else{
            return Maybe<Pos>();
        }
    };
}
template<typename Res, typename Context, typename NoParseInfo>
outparse wrap_outparse_2_lists(ptr<vector<ParseNode<Res,Context>>> succes,
                               ptr<vector<NotParseNode<NoParseInfo>>> fail,
                               Parse<Res,Context,NoParseInfo> parse){
    OUTPARSE_LOGGER.print("wrap_outparse_2_lists");
    return [=](Text text, Pos pos)->OutParseRes{
        ParseResult<Res,Context,NoParseInfo> res=parse(text,pos);
        if(isOk(res)){
            succes->push_back(get<ParseNode<Res,Context>>(res));
            return get<ParseNode<Res,Context>>(res).end;
        }else{
            fail->push_back(get<NotParseNode<NoParseInfo>>(res));
            return Maybe<Pos>();
        }
    };
}
OutParseRes parse_seq(Text str, Pos start, vector<outparse> seq);
outparse outparse_seq(vector<outparse> seq);
//не рекомендуется для вычислений с результатом - будут записаны ошибки
OutParseRes parse_variants(Text str, Pos start, vector<outparse> variants);
//не рекомендуется для вычислений с результатом - будут записаны ошибки
outparse outparse_variants(vector<outparse> variants);
//не рекомендуется для вычислений с результатом - будут записаны ошибки
OutParseRes parse_rep(Text str, Pos start, outparse parse);
outparse outparse_rep(outparse parse);
outparse wrap(outparse target, outparse sep);
static const outparse outparse_epmty=outparse_seq({});
outparse maybe_outparse(outparse parse);
}
#endif /* OUTPARSE_HH_ */
