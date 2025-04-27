#ifndef SOME_MORE_PARSING_UTILITES_H
#define SOME_MORE_PARSING_UTILITES_H
#include "parser/outparse.hh"
#include "parser/parse_instructions.hh"
#include "parse_errors_ver_2.h"
using common_parsing::Parse;
using common_parsing::ParseResult;
using common_parsing::Text;
using common_parsing::Pos;
using common_parsing::OutParseTarget;
using common_parsing::VariantError;
using common_parsing::outparse;
using common_parsing::OutParseRes;
using common_parsing::NotParseNode;
using common_parsing::ParseNode;
using common_parsing::Maybe;
namespace concrete_parsing_ver_2 {
template<typename Res, typename Context, typename Info>
ParseNode<Res,Context> get_node(ParseResult<Res,Context,Info> res){
    return get<ParseNode<Res,Context>>(res);
}
template<typename Res, typename Context, typename Info>
NotParseNode<Info> get_error(ParseResult<Res,Context,Info> res){
    return get<NotParseNode<Info>>(res);
}
template<typename Res, typename Context>
ParseResult<Res,Context,NoParseSintaxInfo> to_end(ParseResult<Res,Context,NoParseSintaxInfo> res){
    if(common_parsing::isOk(res)){
        auto node=get_node(res);
        if(node.end!=node.target->length()){
            return NotParseNode<NoParseSintaxInfo>{node.start,node.target,NoParseSintaxInfo{"not parse to end"}};
        }
    }
    return res;
}
template<typename Res, typename Context, typename NoParseInfo>
Parse<Res,Context,VariantError<NoParseInfo>> parse_first_max_variant(vector<Parse<Res,Context,NoParseInfo>> vars){
    return [=](Text target, Pos start){
        int max=0;
        bool success=false;
        NotParseNode<VariantError<NoParseInfo>> error{start,target,VariantError<NoParseInfo>{}};
        Maybe<ParseNode<Res,Context>> ok;
        for(auto it:vars){
            auto res=it(target,start);
            if(isOk(res)){
                auto node=get_node(res);
                if(ok.isEmpty()||ok.get_ok().end<node.end){
                    ok=Maybe{node};
                }
            }else{
                auto err=get_error(res);
                error.info.vars_info.push_back(err);
            }
        }
        if(!ok.isEmpty()){
            return ParseResult<Res,Context,VariantError<NoParseInfo>>{ok.get_ok()};
        }else{
            return ParseResult<Res,Context,VariantError<NoParseInfo>>{error};
        }
    };
}
}
#endif // SOME_MORE_PARSING_UTILITES_H
