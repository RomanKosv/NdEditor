#include "../parser/outparse.hh"
namespace common_parsing{
//static Log OUTPARSE_LOGGER{true,"[OUTPARSE_LOGGER]"};
/*
template<typename T>
using ptr=shared_ptr<T>;
template<typename Res, typename Context, typename NoParseInfo>
using OutParseTarget=ptr<vector<ParseResult<Res,Context,NoParseInfo>>>;
*/
// template<typename Res, typename Context, typename NoParseInfo>
// OutParseTarget<Res,Context,NoParseInfo> create_outparse_target(){
//     return make_shared<vector<ParseResult<Res,Context,NoParseInfo>>>({});
// }
//typedef Maybe<Pos> OutParseRes;
//typedef std::function<OutParseRes(Text,Pos)> outparse;
//template<typename Res, typename Context, typename NoParseInfo>
// outparse wrap_outparse(OutParseTarget<Res,Context,NoParseInfo> target, Parse<Res,Context,NoParseInfo> parse){
//     OUTPARSE_LOGGER.print("wrap_outparse");
//     return [=](Text text, Pos pos)->OutParseRes{
//         (*target).push_back(parse(text,pos));
//         ParseResult<Res,Context,NoParseInfo> res=(*target).back();
//         if(isOk(res)){
//             return get<ParseNode<Res,Context>>(res).end;
//         }else{
//             return Maybe<Pos>();
//         }
//     };
// }
// template<typename Res, typename Context, typename NoParseInfo>
// outparse wrap_skip(Parse<Res,Context,NoParseInfo> parse){
//     OUTPARSE_LOGGER.print("wrap_skip");
//     return [=](Text text, Pos pos)->OutParseRes{
//         ParseResult<Res,Context,NoParseInfo> res=parse(text,pos);
//         if(isOk(res)){
//             return get<ParseNode<Res,Context>>(res).end;
//         }else{
//             return Maybe<Pos>();
//         }
//     };
// }
// template<typename Res, typename Context, typename NoParseInfo>
// outparse wrap_outparse_2_lists(ptr<vector<ParseNode<Res,Context>>> succes,
//                                ptr<vector<NotParseNode<NoParseInfo>>> fail,
//                                Parse<Res,Context,NoParseInfo> parse){
//     OUTPARSE_LOGGER.print("wrap_outparse_2_lists");
//     return [=](Text text, Pos pos)->OutParseRes{
//         ParseResult<Res,Context,NoParseInfo> res=parse(text,pos);
//         if(isOk(res)){
//             succes->push_back(get<ParseNode<Res,Context>>(res));
//             return get<ParseNode<Res,Context>>(res).end;
//         }else{
//             fail->push_back(get<NotParseNode<NoParseInfo>>(res));
//             return Maybe<Pos>();
//         }
//     };
// }
OutParseRes parse_seq(Text str, Pos start, vector<outparse> seq){
    OUTPARSE_LOGGER.print("parse_seq");
    if(seq.size()==0){
        return start;
    }
    OutParseRes res=seq[0](str,start);
    for(Pos i=1; i<seq.size(); i++){
        if(res.isEmpty()){
            return res;
        }
        res=seq[i](str,get<1>(res.val));
    }
    return res;
}
outparse outparse_seq(vector<outparse> seq){
    OUTPARSE_LOGGER.print("outparse_seq");
    return [=](Text str, Pos start){
        return parse_seq(str,start,seq);
    };
}
//не рекомендуется для вычислений с результатом - будут записаны ошибки
OutParseRes parse_variants(Text str, Pos start, vector<outparse> variants){
    OUTPARSE_LOGGER.print("parse_variants");
    if(variants.size()==0){
        return start;
    };
    OutParseRes res=variants[0](str,start);
    for(Pos i=1; i<variants.size(); i++){
        if(!res.isEmpty()){
            return res;
        }
        res=variants[i](str,start);
    }
    return res;
}
//не рекомендуется для вычислений с результатом - будут записаны ошибки
outparse outparse_variants(vector<outparse> variants){
    OUTPARSE_LOGGER.print("outparse_variants");
    return [=](Text str, Pos start){
        return parse_variants(str,start,variants);
    };
}
//не рекомендуется для вычислений с результатом - будут записаны ошибки
OutParseRes parse_rep(Text str, Pos start, outparse parse){
    OUTPARSE_LOGGER.print("parse_rep");
    OutParseRes res=parse(str,start);
    Pos end=start;
    while(!res.isEmpty()){
        end=get<1>(res.val);
        res=parse(str,get<1>(res.val));
    }
    return end;
}
outparse outparse_rep(outparse parse){
    OUTPARSE_LOGGER.print("outparse_rep");
    return [=](shared_ptr<string> str, int start){
        return parse_rep(str,start,parse);
    };
}
outparse wrap(outparse target, outparse sep){
    OUTPARSE_LOGGER.print("wrap");
    return outparse_seq({sep,target,sep});
}
//static const outparse outparse_epmty=outparse_seq({});
outparse maybe_outparse(outparse parse){
    OUTPARSE_LOGGER.print("maybe_outparse");
    return outparse_variants({parse,outparse_epmty});
}
}
