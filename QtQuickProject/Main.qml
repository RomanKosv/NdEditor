
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D
import QtQuick3D.Helpers
import QtQuickProject

ApplicationWindow {
    id: window
    width: 640
    height: 360
    visible: true
    title: "Custom Geometry Example"

    // property bool isLandscape: width > height

    View3D {
        id: v3d
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        // camera: camera

        environment: SceneEnvironment {
            id: env
            backgroundMode: SceneEnvironment.Color
            clearColor: "black"
        }

        Node {
            id: originNode
            PerspectiveCamera {
                id: cameraNode
                z: 600
            }
            position: Qt.vector3d(0, 0, 0)
            // rotation: Qt.quaternion(1, 0, 0, 0)
        }

        DirectionalLight {
            id: directionalLight
            color: Qt.rgba(0.4, 0.2, 0.6, 1.0)
            ambientColor: Qt.rgba(0.1, 0.1, 0.1, 1.0)
        }

        PointLight {
            id: pointLight
            position: Qt.vector3d(0, 0, 100)
            color: Qt.rgba(0.1, 1.0, 0.1, 1.0)
            ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
        }

        //! [model triangle]
        Model {
            id: triangleModel
            visible: true
            scale: Qt.vector3d(100, 100, 100)
            geometry: TriangleGeometry {
                // normals: cbNorm.checked
                // normalXY: sliderNorm.value
                // uv: cbUV.checked
                // uvAdjust: sliderUV.value
            }
            materials: [
                PrincipledMaterial {
                    // Texture {
                    //     id: baseColorMap
                    //     source: "qt_logo_rect.png"
                    // }
                    cullMode: PrincipledMaterial.NoCulling
                    // baseColorMap: cbTexture.checked ? baseColorMap : null
                    // specularAmount: 0.5
                }
            ]
        }
        //! [model triangle]

        OrbitCameraController {
            origin: originNode
            camera: cameraNode
        }
    }

}
