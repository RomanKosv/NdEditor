#include "strings_to_screen_version1.h"

// float strings_to_screen_version1::out_box_distance = 10;
vector<Vector3f> strings_to_screen_version1::out_box_normals{
    {1, 0, 0},
    {-1, 0, 0},
    {0, 1, 0},
    {0, -1, 0},
    {0, 0, 1},
    {0, 0, -1}
};

strings_to_screen_version1::strings_to_screen_version1() {
    // string dims="wabc";
    // for(char c:dims){
    //     auto id=default_context.space.get_next();
    //     auto one=default_context.space.get_one(id);
    //     default_context.vars[string{c}]=ExprResSucces{one};
    //     default_context.dim_names[id]=string{c};
    // }
}
/*
vector<triangle> strings_to_screen_version1::get_render(Model & m, Context & c)
{
    // vector<Object> res=intreprete(m,c);
    // vector<Figure> figures=search_figures(res);
    vector<triangle> triangles;
    // for(Figure& f:figures){
    //     vector<tuple<vector<Vector3f>, vector<float> >> f_3d=imagine_3d(f);
    //     for(tuple<vector<Vector3f>, vector<float> >& p:f_3d){
    //         for(auto& out_n : out_box_normals){
    //             get<0>(p).push_back(out_n);
    //             get<1>(p).push_back(out_box_distance);
    //         }
    //         for(triangle& t:triangulate(get<0>(p),get<1>(p))){
    //             triangles.push_back(t);
    //         }
    //     }
    // }
    for(ObjectEntry *entry : m.get_objects()) {
        auto intreprete_rez = intreprete(entry, c);
        if (intreprete_rez.has_value() && intreprete_rez.value().getOk().is_logic()){
            Figure figure = intreprete_rez.value().getOk().get_figure();
            vector<tuple<vector<Vector3f>, vector<float> >> f_3d=imagine_3d(figure);
            RGBA obj_color{};
            obj_color.r = entry->color().redF();
            obj_color.g = entry->color().greenF();
            obj_color.b = entry->color().blueF();
            obj_color.a = entry->color().alphaF();
            for(tuple<vector<Vector3f>, vector<float> >& p: f_3d){
                for(auto& out_n : out_box_normals){
                    get<0>(p).push_back(out_n);
                    get<1>(p).push_back(out_box_distance);
                }
                auto tr = triangulate(get<0>(p), get<1>(p));
                for(triangle& t:tr){
                    t.a.rgba = (t.b.rgba = (t.c.rgba = obj_color));
                    triangles.push_back(t);
                }
            }
        }
    }
    return triangles;
}
*/
vector<triangle> strings_to_screen_version1::triangulate(vector<Vector3f> normals, vector<float> distances, float epsilon)
{
    auto t=easy_triangulate(normals,distances,epsilon);
    vector<triangle> triangles={};
    for(auto i:t){
        triangles.push_back({
            {get<0>(i).x(),get<0>(i).y(),get<0>(i).z()},
            {get<1>(i).x(),get<1>(i).y(),get<1>(i).z()},
            {get<2>(i).x(),get<2>(i).y(),get<2>(i).z()}
        });
    }
    return triangles;
}

tuple<vector<Vector3f>, vector<float> > strings_to_screen_version1::imagine_3d(Polyhedra & figure, float strong_inequality_align)
{
    vector<Vector3f> normals;
    vector<float> distanses;
    typedef nd_geometry::HalfSpace<concrete_parsing_ver_2::NumExpr> HalfSpace;
    for(HalfSpace& hs:*figure.get_faces()){
        concrete_parsing_ver_2::NumExpr expr=*hs.get_upper_bound();
        Vector3f vec;
        vec.x()=linear_algebra_utilites::project(default_context.algebra,expr,default_context.space.get_one(default_context.space.x));
        vec.y()=linear_algebra_utilites::project(default_context.algebra,expr,default_context.space.get_one(default_context.space.y));
        vec.z()=linear_algebra_utilites::project(default_context.algebra,expr,default_context.space.get_one(default_context.space.z));
        normals.push_back(vec);
        distanses.push_back(-linear_algebra_utilites::project(default_context.algebra,expr,default_context.space.get_scale()));
        distanses.back()+=hs.is_strong()?strong_inequality_align:0;
    }
    return {normals,distanses};
}

vector<tuple<vector<Vector3f>, vector<float> > > strings_to_screen_version1::imagine_3d(Figure & figure)
{
    vector<tuple<vector<Vector3f>, vector<float> > > res;
    vector<NumExpr> space={
        default_context.space.get_scale(),
        default_context.space.get_one(default_context.space.x),
        default_context.space.get_one(default_context.space.y),
        default_context.space.get_one(default_context.space.z)
    };
    //auto projected=default_context.gs.project_in(figure,space);
    string s=default_context.to_str(figure);
    for(Polyhedra& p:*figure.get_components()){
        res.push_back(imagine_3d(p));
    }
    return res;
}

Context strings_to_screen_version1::get_standart_context()
{
    return default_context;
}
/*
std::optional<strings_to_screen_version1::Object> strings_to_screen_version1::intreprete(ObjectEntry *entry, Context &context)
{
    QString name=entry->name();
    QString data=entry->expression();
    Text t_name=make_shared<string>(name.toStdString());
    Text t_expr=make_shared<string>(data.toStdString());
    auto p_name=to_end(concrete_parsing_ver_2::p_name(t_name,0));
    auto p_expr=to_end(concrete_parsing_ver_2::p_eval_layer(t_expr,0));
    if(!common_parsing::isOk(p_name)){
        cout<<"no name: "+*t_name<<"\n";
        return std::nullopt;
    }else if(!common_parsing::isOk(p_expr)){
        cout<<"no expr: "+*t_expr<<"\n";
        return std::nullopt;
    }else{
        auto n_name=get_node(p_name);
        auto n_expr=get_node(p_expr);
        string v_name=n_name.intreprete(context);
        Object v_expr=n_expr.intreprete(context);
        if(!context.vars.contains(v_name)){
            if(v_expr.isOk()){
                context.vars[v_name]=v_expr.getOk();
                return v_expr;
            }else{
                cout<<"expr err: "+*t_expr<<"\n";
                return std::nullopt;
            }
        }else{
            cout<<"name already exist: "+*t_name<<"\n";
            return std::nullopt;
        }
    }
}
*/

/*
vector<strings_to_screen_version1::Object> strings_to_screen_version1::intreprete(Model & m, Context & c)
{
    vector<strings_to_screen_version1::Object> res;
    for(ObjectEntry* entry: m.get_objects()){
        QString name=entry->name();
        QString data=entry->expression();
        Text t_name=make_shared<string>(name.toStdString());
        Text t_expr=make_shared<string>(data.toStdString());
        auto p_name=to_end(concrete_parsing_ver_2::p_name(t_name,0));
        auto p_expr=to_end(concrete_parsing_ver_2::p_eval_layer(t_expr,0));
        if(!common_parsing::isOk(p_name)){
            cout<<"no name: "+*t_name<<"\n";
        }else if(!common_parsing::isOk(p_expr)){
            cout<<"no expr: "+*t_expr<<"\n";
        }else{
            auto n_name=get_node(p_name);
            auto n_expr=get_node(p_expr);
            string v_name=n_name.intreprete(c);
            Object v_expr=n_expr.intreprete(c);
            if(!c.vars.contains(v_name)){
                if(v_expr.isOk()){
                    c.vars[v_name]=v_expr.getOk();
                    res.push_back(v_expr);
                }else{
                    cout<<"expr err: "+*t_expr<<"\n";
                }
            }else{
                cout<<"name already exist: "+*t_name<<"\n";
            }
        }
    }
    return res;
}
*/
vector<Figure> strings_to_screen_version1::search_figures(vector<Object> & obj)
{
    vector<Figure> res;
    for(Object& o:obj){
        if(o.isOk()){
            if(o.getOk().is_logic()){
                res.push_back(o.getOk().get_figure());
            }
        }
    }
    return res;
}

