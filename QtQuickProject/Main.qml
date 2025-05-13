
import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick3D
import QtQuick3D.Helpers
import QtQuickProject
import QtQuick.Dialogs
import QtQml

ApplicationWindow {
    id: window
    width: 640
    height: 360
    visible: true
    title: "Nd scene editing: "+(""+save_url).replace(/^file:\/*/, "")+(need_save? "*" : "")
    //*
    property url test_save_url: "file:save_file.json"

    property url save_url: "demo.json"

    property bool need_save: false

    function saveModel(file) {
        model.clear()
        for(var i = 0; i < object_list.count; i++){
            model.add_element(object_list.get(i).entry)
            console.log(object_list.get(i).entry.name, object_list.get(i).entry.expression)
        }
        var succes= model.writeJsonFile(file)
        if (succes) {
            need_save = false
            save_url = file
        }
        return succes
    }

    Component.onCompleted: {
        if (save_url!="") {
            var success=model.readJsonFile(save_url);
            if(!success){
                //some logic
                console.log("failed to load url: "+save_url.toString())
            }else{
                for(var i=0; i<model.get_objects().length; i++){
                    object_list.append({entry: model.get_objects()[i]})
                }
                console.log("loaded objects, count: "+model.get_objects().length)
            }
        }
    }

    onClosing: (close) => {
                   if(need_save) {
                       close.accepted = false;
                       closeSaveDialog.open();
                   }
               }

    SaveDialog{
        id: closeSaveDialog
        anchors.centerIn: Overlay.overlay
        saveURL : save_url
        function saveFun(file){
            return saveModel(file)
        }
        canDisard: true
        onAccepted:{
            need_save = false
            window.close()
        }
    }

    //*/

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


    Row{
        id: topPanel
        height: 20
        anchors{
            top: parent.top
            right: parent.right
            left: parent.left
        }
        Button{
            id: openButton
            text: "Open file"

            SaveDialog{
                id: saveBeforeOpen
                anchors.centerIn: Overlay.overlay
                saveURL: save_url
                function saveFun(file) {
                    return saveModel(file)
                }
                canDisard: true
                onAccepted: {
                    console.log("saved and dont need to save")
                    chooseOpenFile.open()
                }
            }

            FileDialog{
                id: chooseOpenFile
                defaultSuffix: "json"
                fileMode: FileDialog.OpenFile
                onAccepted: {
                    if (model.readJsonFile(selectedFile)) {
                        object_list.clear()
                        for(var i=0; i<model.get_objects().length; i++){
                            object_list.append({entry: model.get_objects()[i]})
                        }
                        save_url = selectedFile
                        need_save = false
                        geometry.setModel(model)
                    }
                    else{
                        openFail.open()
                    }
                }
            }

            Dialog {
                id: openFail
                modal: true
                header: Text{text: "Cant open file"}
                standardButtons: Dialog.Ok | Dialog.Cancel
                anchors.centerIn: Overlay.overlay
                onAccepted: {
                    chooseOpenFile.open()
                }
            }

            onClicked: {
                if (need_save) {
                    saveBeforeOpen.open()
                }
                else {
                    chooseOpenFile.open()
                }
            }
        }
        Button{
            id: newButton
            text: "New"
            SaveDialog{
                id: saveBeforeNew
                saveURL: save_url
                function saveFun(file) {
                    return saveModel(file)
                }
                canDisard: true
                onAccepted: {
                    object_list.clear()
                    model.clear()
                    save_url = ""
                    need_save = false
                }
            }
            onClicked: {
                if (need_save) {
                    saveBeforeNew.open()
                }
                else{
                    object_list.clear()
                    model.clear()
                    save_url = ""
                    need_save = false
                }
            }
        }

        Button{
            id: saveButton
            text: "Save"
            Dialog{
                id: saveFailDialog
                anchors.centerIn: Overlay.overlay
                header: Text{text: "Cant save to "+(""+save_url).replace(/^file:\/*/, "")+"\nChoose another?"}
                standardButtons: Dialog.Ok | Dialog.Cancel
                onAccepted: {
                    saveAsDialog.open()
                }
            }
            onClicked: {
                if(!saveModel(save_url)) {
                    saveFailDialog.open()
                }
            }
        }
        Button{
            id: saveAsButton
            text: "Save as"

            FileDialog{
                id: saveAsDialog
                fileMode: FileDialog.SaveFile
                defaultSuffix: "json"
                onAccepted: {
                    if (!saveModel(selectedFile)) {
                        saveAsFail.open()
                    }
                }
            }
            Dialog{
                id: saveAsFail
                standardButtons: Dialog.Ok | Dialog.Cancel
                header: Text{text: "Cant save to selected file"}
                anchors.centerIn: Overlay.overlay
                onAccepted: {
                    saveAsDialog.open()
                }
            }

            onClicked: {
                saveAsDialog.open()
            }
        }
        Slider{
            id: boxDistanceSlider
            from: 1
            to: 1000
            value: geometry.boxDist
            Binding{boxDistanceSlider.value: geometry.boxDist}
            onValueChanged: {
                if (geometry.boxDist !== value) {
                    geometry.boxDist = value;
                }
            }
            ToolTip.visible: hovered
            ToolTip.text: "Bounding box size"
        }
        TextField{
            id: boxDistText
            text: geometry.boxDist
            onEditingFinished: {
                if (parseInt(text, 10) != NaN) {
                    geometry.boxDist = parseInt(text, 10)
                }
                else{
                    text = geometry.boxDist
                }
            }
            Binding{
                boxDistText.text: geometry.boxDist
                when: !boxDistText.activeFocus
            }
            width: 70
            ToolTip.visible: hovered
            ToolTip.text: "Bounding box size"
        }
    }

    SplitView{
        id: mainPanel
        anchors{
            top: topPanel.bottom
            right: parent.right
            left: parent.left
            bottom: parent.bottom
        }

        Item{
            id: dispathcer
            property double color_indent: 20
            property double flagsPostfix : 30
            property double delButtonPostfix : 15
            property double name_width: name_header.width
            property double scrooller_width: 15
            property double expression_width: width - color_indent - name_width - flagsPostfix - scrooller_width - delButtonPostfix

            SplitView.preferredWidth: 250

            SplitView.minimumWidth: 50

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
                        required property int index

                        ColorDialog{
                            id: colorSelector
                            options: ColorDialog.ShowAlphaChannel
                            onAccepted: {
                                // console.log(entry.color, colorSelector.color)
                                entry.color = selectedColor
                                need_save = true
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
                                    // anchors.fill: colorButton
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
                                    need_save = true
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
                                    need_save = true
                                }
                                background: Rectangle{
                                    color: entry.isError? Qt.color("red") : Qt.color("white")
                                }
                            }
                            CheckBox {
                                id: is_visible_check_box
                                text: "visible"
                                checked: entry.visible
                                width: dispathcer.flagsPostfix/2
                                onCheckedChanged: {
                                    entry.visible = checked
                                    need_save = true
                                }
                                ToolTip.visible: hovered
                                ToolTip.text: "turn on/off visibility"
                            }
                            CheckBox {
                                id: is_project_check_box
                                text: "project"
                                checked: entry.project
                                width: dispathcer.flagsPostfix/2
                                onCheckedChanged: {
                                    entry.project = checked
                                    need_save = true
                                }
                                ToolTip.visible: hovered
                                ToolTip.text: "turn on/off projection"
                            }
                            Button {
                                id : removeButton
                                width : dispathcer.delButtonPostfix
                                onClicked: {
                                    object_list.remove(listItem.index)
                                    need_save = true
                                }
                                text: "Delete object"
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
                    need_save = true
                }
                text: "Add object"
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
                    for(var j = 0; j < model.get_objects().length; j++) {
                        console.log(model.get_objects()[j].name, model.get_objects()[j].entry)
                    }

                    model.clear()
                    for(var i = 0; i < object_list.count; i++){
                        model.add_element(object_list.get(i).entry)
                        console.log(object_list.get(i).entry.name, object_list.get(i).entry.expression)
                    }
                    geometry.setModel(model)
                }
                text: "Update scene"
            }
        }

        // property bool isLandscape: width > height

        View3D {
            id: v3d
            // anchors.left: dispathcer.right
            // anchors.top: parent.top
            // anchors.right: parent.right
            // anchors.bottom: parent.bottom

            SplitView.preferredWidth: 500

            SplitView.minimumWidth: 50

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
                    clipFar: Math.min(Math.max(1000, geometry.boxDist*2), 10000)
                }
                position: Qt.vector3d(0, 0, 0)
                // rotation: Qt.quaternion(1, 0, 0, 0)
            }

            DirectionalLight {
                id: directionalLight1
                color: Qt.rgba(1, 1, 1, 1.0)
                ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
                castsShadow: true
                shadowBias:0.5
                eulerRotation : Qt.vector3d(0, 0, 0)
            }

            DirectionalLight {
                id: directionalLight2
                color: Qt.rgba(1, 1, 1, 1.0)
                ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
                castsShadow: true
                shadowBias:0.5
                eulerRotation : Qt.vector3d(Math.PI/2, 0, 0)
            }

            DirectionalLight {
                id: directionalLight3
                color: Qt.rgba(1, 1, 1, 1.0)
                ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)
                castsShadow: true
                shadowBias:0.5
                eulerRotation : Qt.vector3d(0, Math.PI/2, 0)
            }

            // PointLight {
            //     id: pointLight
            //     position: cameraNode.position
            //     color: Qt.rgba(1, 1, 1, 1)
            //     ambientColor: Qt.rgba(0.2, 0.2, 0.2, 1.0)

            // }
            // Binding{pointLight.position : cameraNode.position}

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
                        // baseColor: Qt.white
                        cullMode: PrincipledMaterial.NoCulling
                        // baseColorMap: null
                        // specularAmount: 0.5
                        // roughness: 0.259091
                        alphaMode: PrincipledMaterial.Mask
                        blendMode: PrincipledMaterial.SourceOver
                        alphaCutoff: 0.9
                    }
                ]
            }

            Model{
                id: additionalModel
                visible: true
                scale: Qt.vector3d(100, 100, 100)
                geometry: geometry
                materials: [
                    PrincipledMaterial {
                        // Texture {
                        //     id: baseColorMap
                        //     source: "qt_logo_rect.png"
                        // }
                        // baseColor: Qt.white
                        cullMode: PrincipledMaterial.NoCulling
                        // baseColorMap: null
                        // specularAmount: 0.5
                        // roughness: 0.259091
                        alphaMode: PrincipledMaterial.Blend
                        blendMode: PrincipledMaterial.SourceOver
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

}
