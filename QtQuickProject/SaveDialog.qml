import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs


Dialog{

    id: saveDialog
    modal: true

    function saveFun(file) {
        console.log("write profonation, file: ", file)
        return true
    }
    property url saveURL: ""
    property bool canDisard: false

    Dialog{
        id: saveFailNotification
        header: Text{text:"Can't save to file"/*; height: 20*/}
        contentItem: Row{
            Button{
                text: "Choose another file"
                onClicked: {
                    chooseSaveFile.open()
                    saveFailNotification.accept()
                }
            }
            Button{
                text: "Cancel"
                onClicked: {
                    saveFailNotification.reject()
                }
            }
        }
    }

    FileDialog {
        id: chooseSaveFile
        fileMode: FileDialog.SaveFile
        onAccepted: {
            var succes = saveFun(selectedFile)
            if(!succes){
                saveFailNotification.open()
            }else{
                console.log("saved objects")
                saveDialog.accept()
            }
        }
        defaultSuffix: "json"
    }
    header: Text{text: "You have unsaved changes"}
    contentItem: Row{
        id: saveDialogButtons
        Button{
            text: "Save"
            onClicked: {
                if (saveURL == "") {
                    chooseSaveFile.open()
                }
                else{
                    if(saveFun(saveURL)){
                        saveDialog.accept()
                        console.log("saved to file", saveURL)
                    }else{
                        saveFailNotification.open()
                    }
                }
            }
        }
        Button{
            text: "Discard"
            visible: canDisard
            onClicked: {
                saveDialog.accept()
            }
        }
        Button{
            text: "Cancel"
            onClicked: {
                saveDialog.reject()
            }
        }
    }
}
