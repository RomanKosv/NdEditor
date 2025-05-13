#ifndef EXAMPLEGEOMETRY_H
#define EXAMPLEGEOMETRY_H

#include <QQuick3DGeometry>
#include "model.h"
#include "triangles.h"
#include <QRandomGenerator>
#include <QVector3D>

//! [triangle geometry]
class TriangleGeometry : public QQuick3DGeometry
{
    Q_OBJECT
    QML_NAMED_ELEMENT(TriangleGeometry)
    // Q_PROPERTY(bool normals READ normals WRITE setNormals NOTIFY normalsChanged)
    // Q_PROPERTY(float normalXY READ normalXY WRITE setNormalXY NOTIFY normalXYChanged)
    // Q_PROPERTY(bool uv READ uv WRITE setUV NOTIFY uvChanged)
    // Q_PROPERTY(float uvAdjust READ uvAdjust WRITE setUVAdjust NOTIFY uvAdjustChanged)

public:
    TriangleGeometry();
    Q_INVOKABLE void setModel(Model*);
    Q_PROPERTY(float boxDist READ boxDist WRITE setBoxDist NOTIFY boxDistChanged FINAL);

    // bool normals() const { return m_hasNormals; }
    // void setNormals(bool enable);

    // float normalXY() const { return m_normalXY; }
    // void setNormalXY(float xy);

    // bool uv() const { return m_hasUV; }
    // void setUV(bool enable);

    // float uvAdjust() const { return m_uvAdjust; }
    // void setUVAdjust(float f);

    // signals:
    //     void normalsChanged();
    //     void normalXYChanged();
    //     void uvChanged();
    //     void uvAdjustChanged();
    void setBoxDist(float);
    float boxDist();
signals:
    void boxDistChanged(float);

private:
    void updateData();

    std::vector<triangle> triangles={
        {
            {-1.0f, -1.0f, 1.0f},
            {1.0f, -1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}
        }
    };
    float _boxDist = 50;
    // bool m_hasNormals = false;
    // float m_normalXY = 0.0f;
    // bool m_hasUV = false;
    // float m_uvAdjust = 0.0f;
};
//! [triangle geometry]

// class ExamplePointGeometry : public QQuick3DGeometry
// {
//     Q_OBJECT
//     QML_NAMED_ELEMENT(ExamplePointGeometry)

// public:
//     ExamplePointGeometry();

// private:
//     void updateData();
// };

#endif
