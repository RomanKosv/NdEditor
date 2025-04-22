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
