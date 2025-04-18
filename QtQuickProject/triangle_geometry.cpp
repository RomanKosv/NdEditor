
#include "triangle_geometry.h"
#include <QRandomGenerator>
#include <QVector3D>

TriangleGeometry::TriangleGeometry()
{
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
