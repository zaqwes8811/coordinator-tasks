import QtQuick 1.0
//module
 //import "folderName"            //import the contents of the folder
 //import "script.js" as Script   // import a Javascript file and name it as Script
Rectangle {
  id : content
  width: 600; height: 200
Row{
         anchors.centerIn: parent
         spacing: parent.width/12

         Button{
             id: loadButton
             buttonColor: "lightgrey"
             label: "Load"
         }
         Button{
             buttonColor: "grey"
             id: saveButton
             label: "Save"
         }
         Button{
             id: exitButton
             label: "Exit"
             buttonColor: "darkgrey"

             onButtonClick: Qt.quit()
         }
     }
}
