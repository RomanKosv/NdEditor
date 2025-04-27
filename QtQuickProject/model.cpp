#include "model.h"

Model::Model() {}

void Model::add_element(ObjectEntry* entry)
{
    objects.push_back(entry);
}

void Model::clear()
{
    objects.clear();
}

QVector<ObjectEntry *> Model::get_objects()
{
    return objects;
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
