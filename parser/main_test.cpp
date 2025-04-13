
// #include <iostream>


// #include <iostream>
// using namespace std;
// int main()
// {
//     Log log{true,"[test logger]"};
//     Parse<Res,Context,TestVariantNoParseInfo> addop=
//         parse_variants<Res,Context,NoParseInfo>({test_wrap_parse_terminal(parse_terminal("+")),test_wrap_parse_terminal(parse_terminal("-"))});
//     string s;
//     log.print("+-");
//     getline(cin,s);
//     auto res=addop(make_shared<string>(s),0);
//     if(isOk(res)){
//         cout<<get<ParseNode<Res,Context>>(res).end<<"\n";
//     }else{
//         cout<<"error\n";
//     }
//     Parse<Res,Context,TestVariantNoParseInfo> multop=
//         parse_variants<Res,Context,NoParseInfo>({test_wrap_parse_terminal(parse_terminal("*")),test_wrap_parse_terminal(parse_terminal("/"))});
//     log.print("*/");
//     do{
//         getline(cin,s);
//         auto res=multop(make_shared<string>(s),0);
//         if(isOk(res)){
//             cout<<get<ParseNode<Res,Context>>(res).end<<"\n";
//         }else{
//             cout<<"error\n";
//         }
//     }while(s!="stop");
//     OpFun<Res,Context,Res,Res> add_op_fun=[](vector<Res>,vector<Res>,Context){
//         return Res{};
//     };
//     OpFun<Res,Context,Res,Res> mult_op_fun=add_op_fun;
//     Fun<Res,Res,Context> fun=[](vector<Res>,Context){
//         return Res{};
//     };
//     TestParse parse_fun_name_literal=test_wrap_parse_terminal(parse_terminal("fun"));
//     log.print("fun");
//     do{
//         getline(cin,s);
//         auto res=parse_fun_name_literal(make_shared<string>(s),0);
//         if(isOk(res)){
//             cout<<get<ParseNode<Res,Context>>(res).end<<"\n";
//         }else{
//             cout<<"error\n";
//         }
//     }while(s!="stop");
//     Parse<Fun<Res,Res,Context>,Context,NoParseInfo> parse_fun_name=[&](Text target, Pos start){
//         PARSING_TEST_LOGGER.print("parse_fun_name");
//         auto res=parse_fun_name_literal(target,start);
//         if(!isOk(res)){
//             return ParseResult<Fun<Res,Res,Context>,Context,NoParseInfo>{get<NotParseNode<NoParseInfo>>(res)};
//         }else{
//             Intreprete<Fun<Res,Res,Context>,Context> intreprete=[&](Context){return fun;};
//             return ParseResult<Fun<Res,Res,Context>,Context,NoParseInfo>{ParseNode<Fun<Res,Res,Context>,Context>{start,get<ParseNode<Res,Context>>(res).end,target,intreprete}};
//         }
//     };
//     log.print("parse_fun_name to lambda");
//     do{
//         getline(cin,s);
//         auto res=parse_fun_name(make_shared<string>(s),0);
//         if(isOk(res)){
//             cout<<get<ParseNode<Fun<Res,Res,Context>,Context>>(res).end<<"\n";
//         }else{
//             cout<<"error\n";
//         }
//     }while(s!="stop");
//     Parse<Res,Context,NoParseInfo> parse_num_literal=test_wrap_parse_terminal(parse_terminal("num"));
//     //не буду тестить уже простые терминалы тестились
//     OperatorParseInstruction<Res,Res,Res,Context,NoParseInfo> easy_operator_instructions;
//     easy_operator_instructions.parse_operator=test_wrap_parse_terminal(addop);
//     easy_operator_instructions.parse_operand=parse_num_literal;
//     easy_operator_instructions.intreprete_fun=add_op_fun;
//     auto parse_easy_operator_instructions=parse_operator(easy_operator_instructions);
//     log.print("parse_easy_operator_instructions");
//     do{
//         getline(cin,s);
//         auto res=parse_easy_operator_instructions(make_shared<string>(s),0);
//         if(isOk(res)){
//             cout<<get<ParseNode<Res,Context>>(res).end<<"\n";
//         }else{
//             cout<<"error\n";
//         }
//     }while(s!="stop");
//     FunCallParseInstruction<Res,Res,Context,NoParseInfo> parse_easy_fun_instructions{
//         parse_fun_name,
//         parse_num_literal
//     };
//     log.print("parse_easy_fun_instructions");
//     do{
//         getline(cin,s);
//         auto res=parse_easy_fun_instructions.get_parse_fun_call()
//                    (make_shared<string>(s),0);
//         if(isOk(res)){
//             cout<<get<ParseNode<Res,Context>>(res).end<<"\n";
//         }else{
//             cout<<"error\n";
//         }
//     }while(s!="stop");
//     OperatorParseInstruction<Res,Res,Res,Context,NoParseInfo> mult_instructions{};
//     mult_instructions.parse_operator=test_wrap_parse_terminal(multop);
//     mult_instructions.intreprete_fun=mult_op_fun;
//     OperatorParseInstruction<Res,Res,Res,Context,NoParseInfo> add_instructions{
//         [&](Text text, Pos start){return parse_operator(mult_instructions)(text,start);},
//         test_wrap_parse_terminal(addop),
//         add_op_fun
//     };//недостаток - отсутствие парсинга пробелов
//     FunCallParseInstruction<Res,Res,Context,NoParseInfo> fun_instructions{
//         parse_fun_name,
//         [&](Text text, Pos start){return parse_operator(add_instructions)(text,start);}
//     };
//     mult_instructions.parse_operand=[&](Text target,Pos start){
//         PARSING_TEST_LOGGER.print("parse_fun_or_num");
//         return test_wrap_parse_terminal(
//             parse_variants<Res,Context,NoParseInfo>(
//                 {parse_num_literal,fun_instructions.get_parse_fun_call()}))(target,start);
//     };
//     log.print("no trivial system");
//     do{
//         getline(cin,s);
//         auto res=parse_operator(add_instructions)
//                    (make_shared<string>(s),0);
//         if(isOk(res)){
//             cout<<get<ParseNode<Res,Context>>(res).end<<"\n";
//         }else{
//             cout<<"error\n";
//         }
//     }while(s!="stop");
//     return 0;
// }
