/*
 * parse_instructions.hh
 *
 *  Created on: Mar 30, 2025
 *      Author: aleksandr
 */

#ifndef PARSE_INSTRUCTIONS_HH_
#define PARSE_INSTRUCTIONS_HH_

#include <vector>
#include "function_parse_architect.hh"
#include "outparse.hh"
namespace common_parsing{
template<typename NoParseInfo>
struct VariantError{
	std::vector<NotParseNode<NoParseInfo>> vars_info;
};
template<typename Res, typename Context, typename NoParseInfo>
Parse<Res,Context,VariantError<NoParseInfo>> parse_variants(vector<Parse<Res,Context,NoParseInfo>> vars){
	return [=](Text target, Pos start){
		vector<outparse> outparsers;
		OutParseTarget<Res,Context,NoParseInfo> out_target=
                std::make_shared<std::vector<ParseResult<Res,Context,NoParseInfo>>>(std::vector<ParseResult<Res,Context,NoParseInfo>>{});
        for(Pos i=0; i<vars.size(); i++){
			outparsers.push_back(wrap_outparse(out_target,vars[i]));
		}
		OutParseRes res=outparse_variants(outparsers)(target,start);
		if(res.isEmpty()){
			VariantError<NoParseInfo> err;
			for(auto i:*out_target){
                err.vars_info.push_back(get<NotParseNode<NoParseInfo>>(i));
			}
            return ParseResult<Res,Context,VariantError<NoParseInfo>>{NotParseNode<VariantError<NoParseInfo>>{start,target,err}};
		}
        return ParseResult<Res,Context,VariantError<NoParseInfo>>{get<ParseNode<Res,Context>>(out_target->back())};
	};
}

template<typename Res, typename Context>
vector<Res> intreprete_vector(vector<ParseNode<Res,Context>> nodes, Context context){
    vector<Res> res;
    for(ParseNode<Res,Context> i:nodes){
        res.push_back(i.intreprete(context));
    }
    return res;
}
template<typename Res, typename Context>
ParseNode<vector<Res>,Context> compress_parse_vector(vector<ParseNode<Res,Context>> nodes,Text target,Pos start,Pos end){
    Intreprete<vector<Res>,Context> intreprete=[=](Context context){
        return intreprete_vector(nodes,context);
    };
    ParseNode<vector<Res>,Context> res;
    res.start=start;
    res.end=end;
    res.target=target;
    res.intreprete=intreprete;
    return res;
}

}

#endif /* PARSE_INSTRUCTIONS_HH_ */
