
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D
import QtQuick3D.Helpers
import QtQuickProject
import QtQuick.Dialogs

ApplicationWindow {
    id: window
    width: 640
    height: 360
    visible: true
    title: "Custom Geometry Example"

    NdModel{
        id: model
    }

    Component{
        id: entry_pattern
        ObjectEntry{
            name: ""
            expression: ""
            color: Qt.color("green")
        }
    }

    ListModel{
        id: object_list
    }
    Item{
        id: dispathcer
        property double color_indent: 20
        property double flagsPostfix : 30
        property double name_width: name_header.width
        property double expression_width: width - color_indent - name_width - flagsPostfix

        width: 250
        anchors{
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        SplitView{
            id: headers
            anchors{
                leftMargin : dispathcer.color_indent
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 30
            Text{
                id: name_header
                SplitView.preferredWidth: 40
                SplitView.minimumWidth: 20
                height: parent.height
                text: "name"
            }
            Text{
                id: expression_header
                SplitView.preferredWidth: 100
                SplitView.minimumWidth: 20
                height: parent.height
                text: "expression"
            }
        }

        ScrollView{
            id : list_scroll
            anchors{
                left: parent.left
                right: parent.right
                top: headers.bottom
                bottom: addButton.top
            }
            clip: true
            ListView{
                id: object_list_view
                model: object_list
                anchors.fill : parent

                delegate: Item{
                    id: listItem
                    width : parent.width
                    property double additionalHeight: 8
                    height: Math.max(name_field.contentHeight, exp_field.contentHeight) + additionalHeight
                    // onHeightChanged: {
                    //     console.log(entry.name)
                    //     console.log(entry.expression)
                    // }
                    // required property string name
                    // required property string expression
                    required property var entry
                    // property bool isNew: true
                    // Component.onCompleted: {
                    //     isNew = false
                    // }

                    ColorDialog{
                        id: colorSelector
                        onAccepted: {
                            // console.log(entry.color, colorSelector.color)
                            entry.color = selectedColor
                        }
                    }

                    Row{
                        id: fields
                        anchors.fill: parent
                        Button{
                            id: colorButton
                            width: dispathcer.color_indent
                            height: parent.height
                            background: Rectangle{
                                anchors.fill: colorButton
                                color: entry.color
                            }

                            onClicked: {
                                colorSelector.open()
                            }
                        }

                        TextArea {
                            id: name_field
                            text: entry.name
                            width: dispathcer.name_width
                            wrapMode: TextArea.WrapAtWordBoundaryOrAnywhere
                            // onWidthChanged: {
                            //     console.log("new width:", width)
                            //     if(!listItem.isNew) dispathcer.name_width = width
                            // }
                            onTextChanged: {
                                console.log("change")
                                entry.name = text
                            }
                        }
                        TextArea {
                            id: exp_field
                            text: entry.expression
                            width: dispathcer.expression_width
                            wrapMode: TextArea.WrapAtWordBoundaryOrAnywhere
                            // onWidthChanged: {
                            //     console.log("new width:", width)
                            //     if(!listItem.isNew) dispathcer.expression_width = width
                            // }
                            onTextChanged: {
                                console.log("change")
                                entry.expression = text
                            }
                        }
                        CheckBox {
                                id: is_visible_check_box
                                text: "visible"
                                checked: true
                                width: dispathcer.flagsPostfix/2
                                onCheckedChanged: {
                                    entry.visible = checked
                                }
                        }
                        CheckBox {
                                id: is_project_check_box
                                text: "project"
                                checked: false
                                width: dispathcer.flagsPostfix/2
                                onCheckedChanged: {
                                    entry.project = checked
                                }
                        }
                    }
                }
            }
        }
        Button {
            id : addButton
            height : 30
            width : parent.width
            anchors{
                left:parent.left
                bottom:runButton.top
            }
            onClicked: {
                object_list.append({entry : entry_pattern.createObject()/*name: "aaaa", expression : "bbbbb"*/})
            }
        }
        Button{
            id: runButton
            height : 30
            width : parent.width
            anchors{
                left: parent.left
                bottom: parent.bottom
            }
            onClicked: {
                for(var j = 0; j < model.get_objects().count; j++) {
                    console.log(model.get_objects()[j].name, model.get_objects()[j].entry)
                }

                model.clear()
                for(var i = 0; i < object_list.count; i++){
                    model.add_element(object_list.get(i).entry)
                    console.log(object_list.get(i).entry.name, object_list.get(i).entry.expression)
                }
                geometry.setModel(model)
            }
        }
    }

    // property bool isLandscape: width > height

    View3D {
        id: v3d
        anchors.left: dispathcer.right
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
            color: Qt.rgba(1, 1, 1, 1.0)
            ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
        }

        PointLight {
            id: pointLight
            position: cameraNode.position
            color: Qt.rgba(1, 1, 1, 1)
            ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)

        }
        Binding{pointLight.position : cameraNode.position}

        //! [model triangle]
        Model {
            id: triangleModel
            visible: true
            scale: Qt.vector3d(100, 100, 100)
            geometry: TriangleGeometry {
                id: geometry
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
                    // baseColor: Qt.green
                    cullMode: PrincipledMaterial.NoCulling
                    // baseColorMap: cbTexture.checked ? baseColorMap : null
                    specularAmount: 0.5
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
