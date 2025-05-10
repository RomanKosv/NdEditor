#include "model.h"

Model::Model() {}

void Model::add_element(ObjectEntry* entry)
{
    entry->setParent(this);
    objects.push_back(entry);
}

bool Model::removeAt(qsizetype index)
{
    bool success=false;
    if(0<=index&&index<objects.size()){
        objects.removeAt(index);
        success=true;
    }
    return success;
}

void Model::clear()
{
    objects.clear();
}

QVector<ObjectEntry *> Model::get_objects()
{
    return objects;
}

QJsonArray Model::toJson()
{
    QJsonArray json;
    for(auto obj : get_objects()) {
        json.append(obj->toJson());
    }
    return json;
}

void Model::readJson(QJsonArray array)
{
    clear();
    for(auto json:array){
        if(json.isObject()){
            ObjectEntry* entry=new ObjectEntry{};
            entry->setParent(this);
            entry->readJson(json.toObject());
            add_element(entry);
        }
    }
}

bool Model::readJsonFile(QUrl url)
{
    cout<<"try load model from url "<<url.toString().toStdString()<<std::endl;
    if(!url.isLocalFile()){
        cout<<url.toString().toStdString()<<" not local file"<<"\n";
    }else{
        QFile file{url.toLocalFile()};
        if(!file.open(QIODevice::ReadOnly)){
            cout<<"couldnt open file "<<file.fileName().toStdString()<<std::endl;
        }else{
            QByteArray data=file.readAll();
            QJsonDocument doc(QJsonDocument::fromJson(data));
            if(!doc.isArray()){
                cout<<"error, not a array object in json \n";
            }else{
                readJson(doc.array());
                cout<<"read file successfull\n";
                return true;
            }
        }
    }
    return false;
}

bool Model::writeJsonFile(QUrl url)
{
    cout<<"try write model in url "<<url.toString().toStdString()<<std::endl;
    if(!url.isLocalFile()){
        cout<<url.toString().toStdString()<<" not local file"<<"\n";
    }else{
        QFile file{url.toLocalFile()};
        if(!file.open(QIODevice::WriteOnly)){
            cout<<" couldnt open file "<<file.fileName().toStdString()<<std::endl;
        }else{
            file.write(QJsonDocument(toJson()).toJson());
            cout<<"write file successfull\n";
            return true;
        }
    }
    return false;
}

vector<triangle> Model::get_render(Context &context)
{
    vector<triangle> triangles;
    strings_to_screen_version1 pipeline;
    for(ObjectEntry *entry : this->get_objects()) {
        auto intreprete_rez = entry->get_render_figure(context);
        if (intreprete_rez.has_value()){
            Figure figure = intreprete_rez.value();
            vector<tuple<vector<Vector3f>, vector<float> >> f_3d=pipeline.imagine_3d(figure);
            RGBA obj_color{};
            obj_color.r = entry->color().redF();
            obj_color.g = entry->color().greenF();
            obj_color.b = entry->color().blueF();
            obj_color.a = entry->color().alphaF();
            for(tuple<vector<Vector3f>, vector<float> >& p: f_3d){
                for(auto& out_n : pipeline.out_box_normals){
                    get<0>(p).push_back(out_n);
                    get<1>(p).push_back(pipeline.out_box_distance);
                }
                auto tr = pipeline.triangulate(get<0>(p), get<1>(p));
                for(triangle& t:tr){
                    t.a.rgba = (t.b.rgba = (t.c.rgba = obj_color));
                    triangles.push_back(t);
                }
            }
        }
    }
    return triangles;
}
