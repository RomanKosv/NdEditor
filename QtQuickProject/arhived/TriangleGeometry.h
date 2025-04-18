#ifndef TRIANGLEGEOMETRY_H
#define TRIANGLEGEOMETRY_H

#include <QQuick3DGeometry>
#include <QVector3D>
#include <iostream>
#include <vector>
#include "triangles.h"
class TriangleGeometry : public QQuick3DGeometry{
    Q_OBJECT
public:
    QML_NAMED_ELEMENT(TriangleGeometry)
    TriangleGeometry();
    void updateData();
private:
    std::vector<triangle> triangles{
        {
            {0,0,0},
            {30,30,30},
            {30,0,0}
        }
    };
};

#endif // TRIANGLEGEOMETRY_H
