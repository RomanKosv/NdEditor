#include "TriangleGeometry.h"

TriangleGeometry::TriangleGeometry(){
    updateData();
}

void TriangleGeometry::updateData(){
    QByteArray vertexData(triangles.size()*sizeof(triangle),Qt::Initialization::Uninitialized);
    std::memcpy(vertexData.data(),triangles.data(),triangles.size()*sizeof(triangle));
    cout<<triangles.size()<<endl;
    for(float* num = (float*)(vertexData.data()); (char*)num != vertexData.end(); num++) {
        cout << *num<<endl;
    }
    setVertexData(vertexData);
    setStride(sizeof(triangle));
    setBounds(QVector3D(-50.0f, -50.0f, -50.0f), QVector3D(+50.0f, +50.0f, +50.0f));
    setPrimitiveType(QQuick3DGeometry::PrimitiveType::Triangles);
    addAttribute(QQuick3DGeometry::Attribute::PositionSemantic,
                 0,
                 QQuick3DGeometry::Attribute::F32Type);
}
