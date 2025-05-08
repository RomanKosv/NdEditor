#include "context_ver_2.h"
namespace concrete_parsing_ver_2 {

NumExpr ExprResSucces::get_num(){
    return get<NumExpr>(res);
}

Figure ExprResSucces::get_figure(){
    return get<Figure>(res);
}

bool ExprResSucces::is_num() const{
    return res.index()==0;
}

bool ExprResSucces::is_logic() const{
    return res.index()==1;
}

bool EvalFun::is_num() const{
    return fun.index()==0;
}

bool EvalFun::is_logic() const{
    return fun.index()==1;
}

NumFun EvalFun::get_num() const{
    return get<NumFun>(fun);
}

BoolFun EvalFun::get_figure() const{
    return get<BoolFun>(fun);
}

StdContext::StdContext(){
    algebra=linear_algebra_utilites::make_algebra<Scalar,NumExpr>([](NumExpr a, NumExpr b)->Scalar{return a.dot(b);});
    function<SparseVector<double>(SparseVector<double>,double)> mult=[](SparseVector<double> a,double c)->SparseVector<double>{
        return a*c;
    };
    function<SparseVector<double>(SparseVector<double>,SparseVector<double>)> add=[](SparseVector<double> a,SparseVector<double> b)->SparseVector<double>{
        return a+b;
    };
    algebra.mult=mult;
    algebra.add=add;
    vars["x"]=ExprResSucces{space.get_one(space.x)};
    vars["y"]=ExprResSucces{space.get_one(space.y)};
    vars["z"]=ExprResSucces{space.get_one(space.z)};
    dim_names[space.x]="x";
    dim_names[space.y]="y";
    dim_names[space.z]="z";
    gs.algebra=algebra;
    gs.obj_factory=nd_geometry::wrap_factory<NumExpr>();
    gs.order=linear_algebra_utilites::make_order<Scalar>();
    gs.polyhedron_gs=make_shared<nd_parser_realisation_1::PolyhedronGeomSys<NumExpr,Scalar>>(
        nd_parser_realisation_1::PolyhedronGeomSys<NumExpr,Scalar>{});
    gs.polyhedron_gs->algebra=gs.algebra;
    gs.polyhedron_gs->obj_factory=gs.obj_factory;
    gs.polyhedron_gs->order=gs.order;
}

bool StdContext::is_scalar(NumExpr expr){
    NumExpr perpendicular=linear_algebra_utilites::perpendicular_component(
        algebra,expr,space.get_one(space.scale));
    return linear_algebra_utilites::is_zero(algebra,perpendicular);
}

bool StdContext::is_match_type(ExprResSucces val, EvalTypes type){
    switch (type) {
    case EvalTypes::figure:
        return val.res.index()==1;
        break;
    case EvalTypes::numexpr:
        return val.res.index()==0;
    case EvalTypes::scalar:
        return is_match_type(val,EvalTypes::numexpr)&&is_scalar(get<NumExpr>(val.res));
    default:
        assert(false);
        break;
    }
}

Maybe<Scalar> StdContext::get_scalar(NumExpr expr){
    if(is_scalar(expr)){
        return Maybe<Scalar>{
                             linear_algebra_utilites::project(algebra,expr,space.get_one(space.scale))};
    }else{
        return Maybe<Scalar>{};
    }
}

string StdContext::get_dim_name(size_t id){
    if(dim_names.contains(id)){
        return dim_names[id];
    }else{
        return "<unnamed dimension "+to_string(id)+" >";
    }
}

EvalMaybe<NumExpr> StdContext::do_div(NumExpr a, NumExpr b){
    if(is_scalar(b)){
        if(get_scalar(b).get_ok()==0){
            return EvalMaybe<NumExpr>{EvalError{"div on 0"}};
        }else{
            return EvalMaybe<NumExpr>{a/get_scalar(b).get_ok()};
        }
    }else{
        return EvalMaybe<NumExpr>{EvalError{"div on linear expr"}};
    }
}

EvalMaybe<NumExpr> StdContext::do_mult(NumExpr a, NumExpr b){
    if(is_scalar(a)){
        return EvalMaybe<NumExpr>{b*get_scalar(a).get_ok()};
    }else if(is_scalar(b)){
        return EvalMaybe<NumExpr>{a*get_scalar(b).get_ok()};
    }else{
        return EvalMaybe<NumExpr>{EvalError{"mult two linears"}};
    }
}

EvalMaybe<NumExpr> StdContext::eval_mult_op(vector<EvalMaybe<NumExpr> > args, vector<MultOp> ops){
    assert(args.size()!=0);
    context_log.print("eval_mult_op");
    Maybe<EvalError> err=EvalMaybe<NumExpr>::get_first_no_ok(args);
    if(!err.isEmpty()){
        return EvalMaybe<NumExpr>{get<EvalError>(err.val)};
    }else{
        vector<NumExpr> numargs=array_utilites::do_map<EvalMaybe<NumExpr>,NumExpr>(args,
                                                                                      [](EvalMaybe<NumExpr> arg){return arg.getOk();});
        NumExpr res=numargs[0];
        for(size_t i=1; i<args.size(); i++){
            if(ops[i-1]==MultOp::div){
                EvalMaybe<NumExpr> divres=do_div(res,numargs[i]);
                if(!divres.isOk()){
                    return divres;
                }else{
                    res=divres.getOk();
                }
            }else{
                EvalMaybe<NumExpr> multres=do_mult(res,numargs[i]);
                if(!multres.isOk()){
                    return multres;
                }else{
                    res=multres.getOk();
                }
            }
        }
        return EvalMaybe<NumExpr>{res};
    }
}

EvalMaybe<NumExpr> StdContext::eval_sum_op(vector<EvalMaybe<NumExpr> > args, vector<SumOp> ops){
    assert(args.size()!=0);
    Maybe<EvalError> err=EvalMaybe<NumExpr>::get_first_no_ok(args);
    if(!err.isEmpty()){
        return EvalMaybe<NumExpr>{get<EvalError>(err.val)};
    }else{
        vector<NumExpr> numargs=array_utilites::do_map<EvalMaybe<NumExpr>,NumExpr>(args,
                                                                                      [](EvalMaybe<NumExpr> arg){return arg.getOk();});
        NumExpr res=numargs[0];
        for(size_t i=1; i<args.size(); i++){
            if(ops[i-1]==SumOp::add){
                res=res+numargs[i];
            }else{
                res=res-numargs[i];
            }
        }
        return EvalMaybe<NumExpr>{res};
    }
}

EvalMaybe<Figure> StdContext::eval_and_op(vector<EvalMaybe<Figure> > args, vector<AndOp>){
    assert(args.size()!=0);
    context_log.print("eval_and_op");
    Maybe<EvalError> err=EvalMaybe<Figure>::get_first_no_ok(args);
    if(!err.isEmpty()){
        return EvalMaybe<Figure>{get<EvalError>(err.val)};
    }else{
        vector<Figure> boolargs=array_utilites::do_map<EvalMaybe<Figure>,Figure>(args,
                                                                                    [](EvalMaybe<Figure> arg){return arg.getOk();});
        context_log.print("boolargs:");
        Figure res=boolargs[0];
        context_log.print(to_str(res));
        for(size_t i=1; i<boolargs.size(); i++){
            context_log.print(to_str(boolargs[i]));
            res=gs.intersect_of(res,boolargs[i]);
            context_log.print("res: "+to_str(res));
        }
        return EvalMaybe<Figure>{res};
    }
}

EvalMaybe<Figure> StdContext::eval_or_op(vector<EvalMaybe<Figure> > args, vector<OrOp>){
    assert(args.size()!=0);
    context_log.print("eval_or_op");
    Maybe<EvalError> err=EvalMaybe<Figure>::get_first_no_ok(args);
    if(!err.isEmpty()){
        return EvalMaybe<Figure>{get<EvalError>(err.val)};
    }else{
        vector<Figure> boolargs=array_utilites::do_map<EvalMaybe<Figure>,Figure>(args,
                                                                                    [](EvalMaybe<Figure> arg){return arg.getOk();});\
            context_log.print("boolargs:");
        Figure res=boolargs[0];
        context_log.print(to_str(res));
        for(size_t i=1; i<args.size(); i++){
            context_log.print(to_str(boolargs[i]));
            res=gs.union_of(res,boolargs[i]);
            context_log.print("res: "+to_str(res));
        }
        return EvalMaybe<Figure>{res};
    }
}

EvalMaybe<ExprResSucces> StdContext::get_common_var(string name){
    if(vars.contains(name)){
        return EvalMaybe<ExprResSucces>{vars[name]};
    }else{
        return EvalMaybe<ExprResSucces>{EvalError{"cant find common var "+name}};
    }
}

EvalMaybe<NumExpr> StdContext::get_numvar(string name){
    if(vars.contains(name)&&is_match_type(vars[name],EvalTypes::numexpr)){
        return EvalMaybe<NumExpr>{vars[name].get_num()};
    }else{
        return EvalMaybe<NumExpr>{EvalError{"cant find num var "+name}};
    }
}

EvalMaybe<Figure> StdContext::get_logic_var(string name){
    if(vars.contains(name)&&is_match_type(vars[name],EvalTypes::figure)){
        return EvalMaybe<Figure>{vars[name].get_figure()};
    }else{
        return EvalMaybe<Figure>{EvalError{"cant find logic var "+name}};
    }
}

EvalMaybe<NumFun> StdContext::get_num_fun(string name){
    if(funs.contains(name)&&funs[name].is_num()){
        return EvalMaybe<NumFun>{funs[name].get_num()};
    }else{
        return EvalMaybe<NumFun>{EvalError{"cant find num fun "+name}};
    }
}

EvalMaybe<BoolFun> StdContext::get_logic_fun(string name){
    if(funs.contains(name)&&funs[name].is_logic()){
        return EvalMaybe<BoolFun>{funs[name].get_figure()};
    }else{
        return EvalMaybe<BoolFun>{EvalError{"cant find logic fun "+name}};
    }
}

EvalMaybe<EvalFun> StdContext::get_common_fun(string name){
    if(funs.contains(name)&&funs[name].is_logic()){
        return EvalMaybe<EvalFun>{funs[name]};
    }else{
        return EvalMaybe<EvalFun>{EvalError{"cant find fun "+name}};
    }
}

common_parsing::Fun<EvalMaybe<ExprResSucces>, EvalMaybe<ExprResSucces>, StdContext> StdContext::wrap_common_fun(EvalFun fun){
    return [fun](vector<EvalMaybe<ExprResSucces>> vec,StdContext c){
        if(fun.is_logic()){
            EvalMaybe<Figure> f=fun.get_figure().fun(vec,c);
            if(f.isOk()){
                return EvalMaybe<ExprResSucces>{ExprResSucces{f.getOk()}};
            }else{
                return EvalMaybe<ExprResSucces>{f.getErr()};
            }
        }else{
            EvalMaybe<NumExpr> n=fun.get_num().fun(vec,c);
            if(n.isOk()){
                return EvalMaybe<ExprResSucces>{ExprResSucces{n.getOk()}};
            }else{
                return EvalMaybe<ExprResSucces>{n.getErr()};
            }
        }
    };
}

EvalMaybe<Figure> StdContext::inverse(EvalMaybe<Figure> figure){
    context_log.print("inverse"+to_str(figure));
    if(figure.isOk()){
        Figure f=gs.inversion_of(figure.getOk());
        context_log.print("inversed: "+to_str(f));
        return EvalMaybe<Figure>{gs.inversion_of(figure.getOk())};
    }else{
        return EvalMaybe<Figure>{figure.getErr()};
    }
}

EvalMaybe<Figure> StdContext::comp(EvalMaybe<NumExpr> left, EvalMaybe<NumExpr> right, CompOp op){
    Maybe<EvalError> err=EvalMaybe<NumExpr>::get_first_no_ok({left,right});
    if(!err.isEmpty()){
        return EvalMaybe<Figure>{err.get_ok()};
    }else{
        NumExpr l=left.getOk();
        NumExpr r=right.getOk();
        switch (op) {
        case CompOp::less:
            return EvalMaybe<Figure>{
                                     gs.obj_factory.make_group({
                                                                gs.obj_factory.make_polyhedron({
                                                                                                gs.obj_factory.make_halfspace(l-r,true)})})};
            break;
        case CompOp::less_or_eq:
            return EvalMaybe<Figure>{
                                     gs.obj_factory.make_group({
                                                                gs.obj_factory.make_polyhedron({
                                                                                                gs.obj_factory.make_halfspace(l-r,false)})})};
            break;
        case CompOp::more:
            return inverse(comp(left,right,CompOp::less_or_eq));
            break;
        case CompOp::more_or_eq:
            return inverse(comp(left,right,CompOp::less));
            break;
        case CompOp::eq:
            return eval_and_op({comp(left,right,CompOp::less_or_eq),comp(left,right,CompOp::more_or_eq)},{AndOp{}});
            break;
        case CompOp::no_eq:
            return inverse(comp(left,right,CompOp::eq));
            break;
        default:
            assert(false);
            break;
        }
    }
}

string StdContext::to_str(NumExpr expr){
    string res="(";
    for(NumExpr::InnerIterator it(expr);it;++it){
        res+=to_string(it.value())+'*'+get_dim_name(it.index())+";";
    }
    res+=")";
    return res;
}

string StdContext::to_str(nd_geometry::HalfSpace<NumExpr> hs){
    string res="{"+to_str(*hs.get_upper_bound());
    if(hs.is_strong()){
        res+="<0";
    }else{
        res+="<=0";
    }
    res+="}";
    return res;
}

string StdContext::to_str(nd_geometry::Polyhedron<NumExpr> p){
    string res="{";
    for(auto hs:*p.get_faces()){
        res+=to_str(hs)+",";
    }
    res+="}";
    return res;
}

string StdContext::to_str(Figure g){
    string res="[";
    for(auto p:*g.get_components()){
        res+=to_str(p)+",";
    }
    res+="]";
    //string a="";
    return res;
}

string StdContext::to_str(ExprResSucces res){
    if(is_match_type(res,EvalTypes::figure)){
        return to_str(res.get_figure());
    }else{
        return to_str(res.get_num());
    }
}

}
