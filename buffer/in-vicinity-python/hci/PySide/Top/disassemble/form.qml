import QtQuick 1.0
import "qmlsrc"
Rectangle {
  id : mainRec  // name
  anchors.fill: parent;  // position
  
  color: "lightblue"  // 

  // data
  property variant captionValues : [1,2,3,4]  //
  width : 400
  height : 250
  // 
  Rectangle {
    id : butRect
    width : 300
    height : 40
    color : "steelblue"
    radius : 4
    anchors.horizontalCenter: parent.horizontalCenter
    //anchors.left: parent.left
    anchors.bottom : parent.bottom
    Row {
      spacing: 4
      anchors.centerIn : parent
      // 
      Button {  //
        id: loadButton
        buttonColor: "lightgrey"
        label: "Show"
        MouseArea {
          anchors.fill: parent
          onClicked: {
            wProc.showWFandSG()
          }
        }
      }
    
      // временная кнопка, пусть пока будет
      Button {
        id : filler
        buttonColor: "lightgrey"
        label: "FillFilds"
        MouseArea {
          anchors.fill : parent
          onClicked : {
            wProc.play()
          }
        }
      }
      // Открывает диалог
      Button {  //
        id: loadFile
        buttonColor: "lightgrey"
        label: "LoadFile"
        MouseArea {
          anchors.fill: parent
          onClicked: {
            wProc.loadFile()
console.log(wProc.getNumChannals())
            po.setEdits(wProc.getNumChannals(), 0)
            po.setEdits(8*wProc.getSampWidth(), 1)
            po.setEdits(wProc.getFrameRate(), 2)
            po.setEdits(wProc.getNumFrames(), 3)
          }
        }
      }  // Butto.. id: loadFile..
    }
  }
  // панель вывода
  PanelOut {
    id : po
    anchors.centerIn : parent
  }

  // панель ввода

}
