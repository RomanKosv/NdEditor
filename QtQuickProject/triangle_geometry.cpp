
#include "triangle_geometry.h"
#include <QRandomGenerator>
#include <QVector3D>
#include "triangulation_with_eigen_1.h"

TriangleGeometry::TriangleGeometry()
{
    vector<Vector3f> normals={
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1},
        {-1, 0, 0},
        {0, -1, 0},
        {0, 0, -1},
        {1, 1, 1}
    };
    vector<float> distances = {1, 1, 1, 1, 1, 1, 2};
    auto t=easy_triangulate(normals,distances);
    triangles={};
    for(auto i:t){
        triangles.push_back({
                             {get<0>(i).x(),get<0>(i).y(),get<0>(i).z()},
                             {get<1>(i).x(),get<1>(i).y(),get<1>(i).z()},
                             {get<2>(i).x(),get<2>(i).y(),get<2>(i).z()}
        });
    }
    updateData();
}

//! [update data]
void TriangleGeometry::updateData()
{
    clear();

    static_assert(sizeof(triangle) ==3*3*sizeof(float));
    // int stride = 3 * sizeof(float);
    // assert(stride == sizeof(vec3));

    QByteArray vertexData(sizeof(triangle)*triangles.size(), Qt::Initialization::Uninitialized);
    // assert(sizeof(triangle)*triangles.size() == 3 * stride);
    memcpy(vertexData.data(), triangles.data(), sizeof(triangle)*triangles.size());

    setVertexData(vertexData);
    setStride(sizeof(vec3));
    // setBounds(QVector3D(-1.0f, -1.0f, 0.0f), QVector3D(+1.0f, +1.0f, 0.0f));

    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);

    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);

}
//! [update data]
