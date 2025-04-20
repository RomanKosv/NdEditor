#include "../parser/test_inequality_system_parsing_1.h"
#include <iostream>
//#include "../logger/basic_loggers.h"
template<typename Res>
string node_to_str(ParseResult<EvalMaybe<Res>,StdContext,NoParseSintaxInfo> res, StdContext c){
    if(isOk(res)){
        ParseNode<EvalMaybe<Res>,StdContext> node=get<ParseNode<EvalMaybe<Res>,StdContext>>(res);
        return "ParseNode{start:"+to_string(node.start)+",end:"+to_string(node.end)+",res:"+c.to_str(node.intreprete(c))+"}";
    }else{
        NotParseNode<NoParseSintaxInfo> info=get<NotParseNode<NoParseSintaxInfo>>(res);
        return "NotParseNode{<not support to str now>}";
    }
}
void test_inequalyty_systems_and_num_expr_1(){
    //BaseLogSystem log_sys{R"(..\..\..\logs\parsing)"};
    //auto log=AddPrefix<decltype(log_sys.log)&>{log_sys.log, "[test parsing 1]"};
    //auto& inp = log_sys.input;
    bool end=false;
    StdContext c;
    do{
        try{
            cout<<"input command (read/read_num/read_figure/stop)\n";
            string command;
            cin>>command;
            if(command=="read"){
                auto str=make_shared<string>("");
                getline(cin,*str);
                ParseResult<EvalMaybe<ExprResSucces>,StdContext,NoParseSintaxInfo> res=p_eval_layer(str,0);
                cout<<"string lenght: "<<to_string(str->length())<<"\n";
                cout<<node_to_str(res,c)<<"\n";
            }else if(command=="read_num"){
                auto str=make_shared<string>("");
                getline(cin,*str);
                ParseResult<EvalMaybe<NumExpr>,StdContext,NoParseSintaxInfo> res=p_num_layer(str,0);
                cout<<"string lenght: "<<to_string(str->length())<<"\n";
                cout<<node_to_str(res,c)<<"\n";
            }else if(command=="read_figure"){
                auto str=make_shared<string>("");
                getline(cin,*str);
                ParseResult<EvalMaybe<Figure>,StdContext,NoParseSintaxInfo> res=p_logic_layer(str,0);
                cout<<"string lenght: "<<to_string(str->length())<<"\n";
                cout<<node_to_str(res,c)<<"\n";
            }else if(command=="stop"){
                end=true;
            }else{
                cout<<"uncnown command\n";
            }
        }catch(string err){
            cout<<"recieved exception, message:\n";
            //cout<<err.what()<<"\n";
        }
    }while(!end);
}
