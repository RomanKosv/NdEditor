#include "transformation_support.h"
namespace concrete_parsing_ver_2 {
//image_object(object,dimpairs) - с перегрузками


tuple<Transform, EvalMaybe<Figure> > intreprete_transform(StdContext c, ParseNode<EvalMaybe<Figure>, StdContext> node)
{
    auto tup=make_transform_all_context(c);
    auto trans=get<1>(tup);
    auto context=get<0>(tup);
    return {trans, node.intreprete(context)};
}

tuple<StdContext, Transform> make_transform_all_context(StdContext c)
{
    ExprGeomSys<NumExpr>* sys=new GroupGeomSys<Scalar,NumExpr>{c.gs};
    Transform t{shared_ptr<ExprGeomSys<NumExpr>>(sys),c.algebra,c.gs.obj_factory,{}};
    for(auto pair:c.dim_names){
        auto id=c.space.get_next();
        t.pairs.push_back(Transform::DimPair{c.space.get_one(pair.first),c.space.get_one(id)});
    }
    t.pairs.push_back(Transform::DimPair{c.space.get_scale(),c.space.get_scale()});
    EvalFun transform_num{NumFun{[t](vector<EvalMaybe<ExprResSucces>> args,StdContext c){
        if(args.size()!=1) return EvalMaybe<NumExpr>{EvalError{"need 1 arg"}};
        EvalMaybe<ExprResSucces> eval=args[0];
        if(!eval.isOk()) return EvalMaybe<NumExpr>{eval.getErr()};
        ExprResSucces success=eval.getOk();
        if(success.is_logic()) return EvalMaybe<NumExpr>{EvalError{"need num arg"}};
        NumExpr num=success.get_num();
        return EvalMaybe<NumExpr>{t.set_dim(t.pairs,num)};
    }}};
    c.funs["im"]=transform_num;
    return {c,t};
}

BoolFun make_transform_fun(Transform t, Figure cond, StdContext context)
{
    vector<Transform::DimPair> pairs13;
    //vector<Transform::DimPair> pairs23;
    cout<<"make_transform_fun dims3: ";
    for(auto pair:context.dim_names){
        auto id=context.space.get_next();
        auto dim3=context.space.get_one(id);
        cout<<id<<" ";
        pairs13.push_back(Transform::DimPair{context.space.get_one(pair.first),dim3});
    }
    cout<<"\n";
    pairs13.push_back(Transform::DimPair{
        context.space.get_scale(),
        context.space.get_scale()});
    Figure cond1=t.set_dim(pairs13,cond);
    Figure cond2=t.set_dim(t.reversed_pairs(),cond1);
    cout<<"make_transform_fun cond2: "<<context.to_str(cond2)<<"\n";
    return BoolFun{[t,cond2,pairs13](vector<EvalMaybe<ExprResSucces>> args, StdContext c){
        if(args.size()!=1) return EvalMaybe<Figure>{EvalError{"need 1 arg"}};
        EvalMaybe<ExprResSucces> eval=args[0];
        if(!eval.isOk()) return EvalMaybe<Figure>{eval.getErr()};
        ExprResSucces success=eval.getOk();
        if(success.is_num()) return EvalMaybe<Figure>{EvalError{"need logic arg"}};
        Figure f=success.get_figure();
        return EvalMaybe<Figure>{c.gs.intersect_of(t.set_dim(pairs13,f),cond2)};
    }};
}

common_parsing::ParseResult<EvalMaybe<BoolFun>, StdContext, NoParseSintaxInfo> p_transform(common_parsing::Text target, common_parsing::Pos start)
{
    auto p_pref=common_parsing::skip_seq({
        common_parsing::wrap_skip(common_parsing::parse_terminal("[")),
        common_parsing::wrap_skip(common_parsing::parse_terminal("transform")),
        common_parsing::wrap_skip(common_parsing::parse_terminal("]"))
    });
    Maybe<Pos> end=p_pref(target,start);
    if(!end.isEmpty()){
        auto res=p_logic_layer(target,end.get_ok());
        if(common_parsing::isOk(res)){
            auto node=get_node(res);
            function<EvalMaybe<BoolFun>(StdContext)> intreprete=[node](StdContext c){
                auto tup=intreprete_transform(c,node);
                auto t=get<0>(tup);
                auto cond=get<1>(tup);
                if(cond.isOk()){
                    return EvalMaybe<BoolFun>{make_transform_fun(t,cond.getOk(),c)};
                }else{
                    return EvalMaybe<BoolFun>{cond.getErr()};
                }
            };
            return ParseResult<EvalMaybe<BoolFun>, StdContext, NoParseSintaxInfo>{
                ParseNode<EvalMaybe<BoolFun>, StdContext>{start,node.end,target,intreprete}
            };
        }else{
            return ParseResult<EvalMaybe<BoolFun>, StdContext, NoParseSintaxInfo>{get_error(res)};
        }
    }else{
        return ParseResult<EvalMaybe<BoolFun>, StdContext, NoParseSintaxInfo>{
            NotParseNode<NoParseSintaxInfo>{start,target,NoParseSintaxInfo{"not find prefix"}}
        };
    }
}

}
