#include "mainwindow.h"

#include <QApplication>
#include <Qt3DCore/QEntity>
#include <Qt3DExtras/QOrbitCameraController>
#include <Qt3DRender/QCamera>
#include <Qt3DRender/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QAspectEngine>

#include <Qt3DInput/QInputAspect>

#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/QGeometryRenderer>
#include <Qt3DExtras/QForwardRenderer>
#include <Qt3DExtras/QPhongMaterial>
#include <Qt3DExtras/QSphereMesh>
#include <Qt3DExtras/QTorusMesh>
#include <Qt3DExtras/Qt3DWindow>

using namespace std;

typedef double Scalar;
const Qt3DCore::QAttribute::VertexBaseType scalar_base_type = Qt3DCore::QAttribute::VertexBaseType::Double;
typedef tuple<Scalar, Scalar, Scalar> Vec3d;
typedef tuple<Vec3d, Vec3d, Vec3d> Triangle;



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
