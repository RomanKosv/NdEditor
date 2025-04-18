import QtQuick
import QtQuick3D
import QtQuickProject

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    View3D {

        anchors.fill: parent
        environment: SceneEnvironment {
            backgroundMode: SceneEnvironment.Color
            clearColor: "black"
        }

        PerspectiveCamera {
            position: Qt.vector3d(0, 200, 300)
            lookAtNode: triangleModel
        }

        PointLight {
            position: Qt.vector3d(0, 200, 300)
            scale : Qt.vector3d(100,100,100)
        }

        Model {
            id: triangleModel
            position : Qt.vector3d(0, 0, 0)
            scale : Qt.vector3d(100, 100, 100)
            geometry : TriangleGeometry{

            }

            materials: [
                PrincipledMaterial {
                }
            ]
        }
        Model {
            position: Qt.vector3d(0,0,0)
            source : "#Cube"
            materials: [
                PrincipledMaterial {
                }
            ]
        }


    }
}


