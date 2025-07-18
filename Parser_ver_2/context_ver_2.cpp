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
    gs.algebra=algebra;
    gs.obj_factory=nd_geometry::wrap_factory<NumExpr>();
    gs.order=linear_algebra_utilites::make_order<Scalar>();
    gs.polyhedron_gs=make_shared<nd_parser_realisation_1::PolyhedronGeomSys<NumExpr,Scalar>>(
        nd_parser_realisation_1::PolyhedronGeomSys<NumExpr,Scalar>{});
    gs.polyhedron_gs->algebra=gs.algebra;
    gs.polyhedron_gs->obj_factory=gs.obj_factory;
    gs.polyhedron_gs->order=gs.order;
    gs.polyhedron_gs->zero=space.get_zero();
    gs.polyhedron_gs->epsilon=epsilon;
    initalize_vars_and_functions();
}

StdContext::StdContext(StdSpaceFactory<Scalar> _space, LinearAlgebra<NumExpr, Scalar> _algebra, GroupGeomSys<Scalar, NumExpr> _gs)
{
    space=_space;
    algebra=_algebra;
    gs=_gs;
    gs.algebra=algebra;
    initalize_vars_and_functions();
}

void StdContext::initalize_vars_and_functions(){
    vars["x"]=ExprResSucces{space.get_one(space.x)};
    vars["y"]=ExprResSucces{space.get_one(space.y)};
    vars["z"]=ExprResSucces{space.get_one(space.z)};
    dim_names[space.x]="x";
    dim_names[space.y]="y";
    dim_names[space.z]="z";
    funs["move"]=EvalFun{
        BoolFun{
            [](vector<EvalMaybe<ExprResSucces>> args, StdContext c){
                return c.move_figure(args);
            }
        }
    };
};

bool StdContext::is_scalar(NumExpr expr){
    NumExpr perpendicular=linear_algebra_utilites::perpendicular_component(
        algebra,expr,space.get_one(space.scale));
    return linear_algebra_utilites::is_zero(algebra,perpendicular);
}

bool StdContext::is_zero(NumExpr expr)
{
    return algebra.like_dot_product(expr,expr)==0;
}

bool StdContext::is_vector(NumExpr expr)
{
    return algebra.like_dot_product(expr,space.get_scale())==0;
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

string StdContext::to_str(EvalTypes type)
{
    switch (type) {
    case concrete_parsing_ver_2::EvalTypes::figure:
        return "fugure";
        break;
    case concrete_parsing_ver_2::EvalTypes::numexpr:
        return "numexpr";
        break;
    case concrete_parsing_ver_2::EvalTypes::scalar:
        return "scalar";
        break;
    default:
        throw "oops";
        break;
    }
}

EvalMaybe<Figure> StdContext::move_figure(Figure figure, NumExpr dim, Scalar distance)
{
    if(is_zero(dim)){
        return EvalMaybe<Figure>{EvalError{"dim [arg 2] is zero in function move"}};
    }else if(!is_vector(dim)){
        return EvalMaybe<Figure>{EvalError{"dim [arg 2] is not vector in function move"}};
    }else{
        Transform t=get_empty_transform();
        auto id=space.get_next();
        auto norm=dim/dim.norm();
        auto new_dim=space.get_one(id);
        t.pairs.push_back(Transform::DimPair{norm,new_dim});
        EvalMaybe<Figure> cond=comp(
            EvalMaybe<NumExpr>{new_dim},
            EvalMaybe<NumExpr>{norm+space.get_scale()*distance},
            CompOp::eq);
        Figure res=t.transform_accurate(figure,cond.getOk());
        return EvalMaybe<Figure>{res};
    }
}

EvalMaybe<Figure> StdContext::move_figure(EvalMaybe<ExprResSucces> figure, EvalMaybe<ExprResSucces> dim, EvalMaybe<ExprResSucces> distance)
{
    if(!figure.isOk()){
        return EvalMaybe<Figure>{figure.getErr()};
    }else if(!figure.getOk().is_logic()){
        return EvalMaybe<Figure>{EvalError{"figure [1 arg] is not figure in move()"}};
    }else if(!dim.isOk()){
        return EvalMaybe<Figure>{dim.getErr()};
    }else if(!dim.getOk().is_num()){
        return EvalMaybe<Figure>{EvalError{"dim [2 arg] is not numexpr in move()"}};
    }else if(!distance.isOk()){
        return EvalMaybe<Figure>{distance.getErr()};
    }else if(!distance.getOk().is_num()||!is_scalar(distance.getOk().get_num())){
        return EvalMaybe<Figure>{EvalError{"distance [3 arg] is not scalar in move()"}};
    }else{
        return move_figure(
            figure.getOk().get_figure(),
            dim.getOk().get_num(),
            get_scalar(distance.getOk().get_num()).get_ok());
    }
}

EvalMaybe<Figure> StdContext::move_figure(std::vector<EvalMaybe<ExprResSucces> > args)
{
    if(args.size()!=3){
        return EvalMaybe<Figure>{EvalError{"move() need 3 args, recieve"+to_string(args.size())}};
    }else{
        return move_figure(args[0],args[1],args[2]);
    }
}

Transform StdContext::get_empty_transform()
{
    return Transform{
        shared_ptr<GeometrySystem<NumExpr,Figure>>{
                new GroupGeomSys<Scalar,NumExpr>{gs}},
        algebra,
        gs.obj_factory,
        {}
    };
}

Transform StdContext::get_transform_with_static_scale()
{
    auto t=get_empty_transform();
    t.pairs.push_back(Transform::DimPair{space.get_scale(),space.get_scale()});
    return t;
}

Transform StdContext::get_full_transform_static_scale()
{
    auto t=get_transform_with_static_scale();
    map<std::size_t,string> mp=dim_names;
    for(auto pair:dim_names){
        auto id=space.get_next();
        t.pairs.push_back(Transform::DimPair{space.get_one(pair.first),space.get_one(id)});
        string str=pair.second+"'";
        mp[id]=str;
    }
    return t;
}

optional<EvalError> StdContext::check_match_types(vector<EvalMaybe<ExprResSucces>> &args, vector<EvalTypes> types, string funname)
{
    auto no_ok=EvalMaybe<ExprResSucces>::get_first_no_ok(args);
    if(!no_ok.isEmpty()){
        return no_ok.get_ok();
    }else{
        string pref="Error in fun "+funname+" : ";
        if(args.size()!=types.size()){
            return EvalError{pref+"need "+to_string(types.size())+" recieve "+to_string(args.size())};
        }else{
            for(int i=0;i<args.size();i++){
                if(!is_match_type(args[i].getOk(),types[i])){
                    return EvalError{pref+"arg "+to_string(i+1)+" excepted "+to_str(types[i])};
                }
            }
            return nullopt;
        }
    }
}

EvalMaybe<ExprResSucces> StdContext::rotate_figure(vector<EvalMaybe<ExprResSucces> > &args)
{
    optional<EvalError> err_types=check_match_types(args,{EvalTypes::figure,EvalTypes::numexpr,EvalTypes::numexpr,EvalTypes::scalar}, "rotate()");
    if(err_types.has_value()){
        return EvalMaybe<ExprResSucces>{err_types.value()};
    }else{
        auto figure=args[0].getOk().get_figure();
        auto dim1=args[1].getOk().get_num();
        auto dim2=args[2].getOk().get_num();
        auto angle=get_scalar(args[3].getOk().get_num()).get_ok();
        Eigen::Rotation2D<Scalar> rot(angle);
        auto t=get_empty_transform();
        if(is_zero(dim1)){
            return EvalMaybe<ExprResSucces>{EvalError{"rotate() arg 2 is zero"}};
        }else if(is_zero(dim2)){
            return EvalMaybe<ExprResSucces>{EvalError{"rotate() arg 3 is zero"}};
        }else{
            dim2=linear_algebra_utilites::perpendicular_component(algebra,dim2,dim1);
            dim1=dim1/dim1.norm();
            dim2=dim2/dim2.norm();
            t.pairs.push_back(Transform::DimPair{dim1,dim1*rot.matrix()(0,0)+dim2*rot.matrix()(0,1)});
            t.pairs.push_back(Transform::DimPair{dim2,dim1*rot.matrix()(1,0)+dim2*rot.matrix()(1,1)});
            return EvalMaybe<ExprResSucces>{ExprResSucces{t.set_dim(t.pairs,figure)}};
        }
    }
}

vector<NumExpr> StdContext::get_named_space()
{
    vector<NumExpr> res;
    res.push_back(space.get_scale());
    for(auto dim:dim_names){
        res.push_back(space.get_one(dim.first));
    }
    return res;
}

vector<NumExpr> StdContext::get_3d_space()
{
    return vector<NumExpr>{
        space.get_scale(),
        space.get_one(space.x),
        space.get_one(space.y),
        space.get_one(space.z)
    };
}

}
