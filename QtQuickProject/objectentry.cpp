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
    emit visibleChanged(val);
}

bool ObjectEntry::visible()
{
    return _visible;
}

void ObjectEntry::setProject(bool new_project)
{
    _project=new_project;
    emit projectChanged(_project);
}

bool ObjectEntry::project()
{
    return _project;
}

void ObjectEntry::setResult(QString new_result)
{
    _result=new_result;
    emit resultChanged(_result);
}

QString ObjectEntry::result()
{
    return _result;
}

void ObjectEntry::setIsError(bool new_err)
{
    _is_error=new_err;
    emit isErrorChanged(_is_error);
}

bool ObjectEntry::isError()
{
    return _is_error;
}

tuple<std::optional<Figure>, QString, bool> ObjectEntry::intreprete_with_string_res(Context & context)
{
    QString name=this->name();
    QString data=this->expression();
    Text t_name=make_shared<string>(name.toStdString());
    Text t_expr=make_shared<string>(data.toStdString());
    auto p_name=to_end(concrete_parsing_ver_2::p_name(t_name,0));
    auto p_expr=to_end(concrete_parsing_ver_2::p_eval_layer(t_expr,0));
    auto p_transform=to_end(concrete_parsing_ver_2::p_transform(t_expr,0));
    std::regex r(R"(\s*)");
    bool empty_name=std::regex_match(*t_name,r);//если имя не указано
    bool empty_expr=std::regex_match(*t_expr,r);//если пустое поле
    std::regex new_var_pattern(R"(\s*\[\s*new\s*var\s*\]\s*)");
    bool new_var_expr=std::regex_match(*t_expr, new_var_pattern);
    QString message="";
    bool is_err;
    std::optional<Figure> res_to_display;
    if(!(common_parsing::isOk(p_name)||empty_name)){//name incorrect, maybe comment
        is_err=!std::regex_match(*t_name,regex(R"(\s*//\s*)"));
        if(is_err){
            message+=" no name: "+*t_name;
        }else{
            message+=" comment ";
        }
    }else if(empty_expr){
        is_err=false;
        message+="empty field";
    }else{
        optional<string> save_name=nullopt;
        is_err=false;
        if(!empty_name){
            auto v_name=get_node(p_name).intreprete(context);
            if(context.vars.contains(v_name)||context.funs.contains(v_name)){
                is_err=true;
                message+=" error: name alredy exists ";
            }else{
                save_name=v_name;
            }
        }
        if(!is_err){
            if(isOk(p_transform)){//transform
                auto v_transform=get_node(p_transform).intreprete(context);
                if(v_transform.isOk()){
                    is_err=false;
                    message+=" intreprete transform success ";
                    if(save_name.has_value()){
                        context.funs[save_name.value()]=concrete_parsing_ver_2::EvalFun{
                            v_transform.getOk()
                        };
                    }else{
                        //pass, just not write
                    }
                }else{
                    is_err=true;
                    message+=" error intreprete trensform ";
                }
            }else if(new_var_expr){
                is_err=false;
                message+=" new var ";
                if(save_name.has_value()){
                    auto id=context.space.get_next();
                    context.dim_names[id]=save_name.value();
                    context.vars[save_name.value()]=ExprResSucces{context.space.get_one(id)};
                }else{
                    //no write
                }
            }else if(common_parsing::isOk(p_expr)){
                auto v_expr=get_node(p_expr).intreprete(context);
                std::tie(message,is_err)=process_intreprete_obj_with_string_res(context,v_expr);
                if((!is_err)&&save_name.has_value()){
                    context.vars[save_name.value()]=v_expr.getOk();
                }else{
                    //no write var
                };
                if((!is_err)&&v_expr.getOk().is_logic()){
                    res_to_display=v_expr.getOk().get_figure();
                }else{
                    //no write display
                };
            }else{
                is_err=true;
                message+=" error: can not read expression part ";
            }
        }else{
            //pass
            //it just err with exist name
            //message and is_err setted
        }
    };
    return {res_to_display,message,is_err};
}

tuple<QString, bool> ObjectEntry::process_intreprete_obj_with_string_res(Context & context, Object & obj)
{
    bool is_err=!obj.isOk();
    QString message=QString::fromStdString(context.to_str(obj));
    if(obj.isOk()&&obj.getOk().is_logic()){
        Figure display=context.gs.project_in(obj.getOk().get_figure(), context.get_named_space());
        message=QString::fromStdString(context.to_str(display));
    }else{
        message=QString::fromStdString(context.to_str(obj));
    }
    return {message,is_err};
}

std::optional<Figure> ObjectEntry::intreprete(Context & context)
{
    std::optional<Figure> res;
    QString message;
    bool is_err;
    std::tie(res,message,is_err)=intreprete_with_string_res(context);
    setResult(message);
    setIsError(is_err);
    return res;
}

std::optional<Figure> ObjectEntry::get_render_figure(Context &context)
{
    auto object=intreprete(context);
    if(object.has_value()){
        if(visible()){
            vector<NumExpr> space;
            if(project()){
                space=context.get_3d_space();
            }else{
                space=context.get_named_space();
            }
            string s=context.to_str(object.value());
            cout<<"get_render_figure "<<s<<"\n";
            auto proj = context.gs.project_in(object.value(),space);
            string s_proj=context.to_str(proj);
            string sm=s+s_proj;
            return proj;
        }else{
            //to return nullopt
        }
    }else{
        //to return nullopt
    }
    return std::nullopt;
}
