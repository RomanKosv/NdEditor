#include "objectentry.h"

// std::vector<triangle> ObjectEntry::triangulate()
// {

// }

QJsonObject ObjectEntry::toJson()
{
    QJsonObject json;
    json["name"] = name();
    json["expression"] = expression();
    json["visible"] = visible();
    json["project"] = project();
    json["r"] = color().redF();
    json["g"] = color().greenF();
    json["b"] = color().blueF();
    json["a"] = color().alphaF();
    return json;
}

void ObjectEntry::readJson(QJsonObject json)
{
    setName(json.contains("name") && json["name"].isString() ? json["name"].toString() : name());
    setExpression(json.contains("expression") && json["expression"].isString() ? json["expression"].toString() : expression());
    setVisible(json.contains("visible") && json["visible"].isBool() ? json["visible"].toBool() : visible());
    setProject(json.contains("project") && json["project"].isBool() ? json["project"].toBool() : project());
    if (json.contains("r") && json["r"].isDouble()
        && json.contains("g") && json["g"].isDouble()
        && json.contains("b") && json["b"].isDouble()
        && json.contains("a") && json["a"].isDouble()){
        setColor(QColor::fromRgbF(
            json["r"].toDouble(),
            json["g"].toDouble(),
            json["b"].toDouble(),
            json["a"].toDouble()));
    }
}

ObjectEntry::ObjectEntry() {
    setColor(QColor::fromRgbF(1, 0, 0, 1));
}

void ObjectEntry::setName(QString s){
    _name=s;
    emit nameChanged(s);
}

QString ObjectEntry::name(){
    return _name;
}

void ObjectEntry::setExpression(QString exp) {
    _expression = exp;
    emit expressionChanged(exp);
}

QString ObjectEntry::expression() {
    return _expression;
}

void ObjectEntry::setColor(QColor color_)
{
    _color = color_;
    emit colorChanged(_color);
}

QColor ObjectEntry::color()
{
    return _color;
}

void ObjectEntry::setVisible(bool val)
{
    _visible=val;
}

bool ObjectEntry::visible()
{
    return _visible;
}

void ObjectEntry::setProject(bool new_project)
{
    _project=new_project;
}

bool ObjectEntry::project()
{
    return _project;
}

std::optional<Object> ObjectEntry::intreprete(Context & context)
{
    QString name=this->name();
    QString data=this->expression();
    Text t_name=make_shared<string>(name.toStdString());
    Text t_expr=make_shared<string>(data.toStdString());
    auto p_name=to_end(concrete_parsing_ver_2::p_name(t_name,0));
    auto p_expr=to_end(concrete_parsing_ver_2::p_eval_layer(t_expr,0));
    std::regex r(R"(\s*)");
    bool empty_name=std::regex_match(*t_name,r);//если имя не указано
    std::regex new_var_pattern(R"(\s*\[\s*new\s*var\s*\]\s*)");
    bool new_var_expr=std::regex_match(*t_expr, new_var_pattern);
    if(new_var_expr){
        if(common_parsing::isOk(p_name)){
            auto n_name=get_node(p_name);
            string v_name=n_name.intreprete(context);
            if(!context.vars.contains(v_name)){
                auto id=context.space.get_next();
                context.dim_names[id]=v_name;
                context.vars[v_name]=ExprResSucces{context.space.get_one(id)};
                return EvalMaybe<ExprResSucces>{context.vars[v_name]};
            }else{
                cout<<"name already exist: "+*t_name<<"\n";
                return std::nullopt;
            }
        }else{
            cout<<"no name: "+*t_name<<"\n";
            return std::nullopt;
        }
    }else if(!(common_parsing::isOk(p_name)||empty_name)){
        cout<<"no name: "+*t_name<<"\n";
        return std::nullopt;
    }else if(!common_parsing::isOk(p_expr)){
        cout<<"no expr: "+*t_expr<<"\n";
        return std::nullopt;
    }else{
        //auto n_name=get_node(p_name);
        auto n_expr=get_node(p_expr);
        //string v_name=n_name.intreprete(context);
        Object v_expr=n_expr.intreprete(context);
        if(empty_name){
            return v_expr;
        }else{
            auto n_name=get_node(p_name);
            string v_name=n_name.intreprete(context);
            if(!context.vars.contains(v_name)){
                if(v_expr.isOk()){
                    context.vars[v_name]=v_expr.getOk();
                    return v_expr;
                }else{
                    cout<<"expr err: "+*t_expr<<" try set name: "<<v_name<<"\n";
                    return std::nullopt;
                }
            }else{
                cout<<"name already exist: "+*t_name<<"\n";
                return std::nullopt;
            }
        }
    }
}

std::optional<Figure> ObjectEntry::get_render_figure(Context &context)
{
    auto object=intreprete(context);
    if(object.has_value()){
        if(object.value().isOk()&&object.value().getOk().is_logic()){
            if(visible()){
                if(project()){
                    //strings_to_screen_version1 pipeline;
                    vector<NumExpr> space={
                        context.space.get_scale(),
                        context.space.get_one(context.space.x),
                        context.space.get_one(context.space.y),
                        context.space.get_one(context.space.z)
                    };
                    return context.gs.project_in(object.value().getOk().get_figure(),space);
                }else{
                    return object.value().getOk().get_figure();
                }
            }
        }
    }
    return std::nullopt;
}
