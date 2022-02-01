// file : uipButtons.qml
import QtQuick 1.1
Rectangle {
  id : uipButtons
  //width : 300; height : 40
  color : "steelblue"; radius : 4
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
        /* тестовая */
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
          //console.log(wProc.parseParameters())  // не понятно, что с
          // возвращаемым значением
          po.setEdits(wProc.getNumChannals(), 0)
          po.setEdits(8*wProc.getSampWidth(), 1)
          po.setEdits(wProc.getFrameRate(), 2)
          po.setEdits(wProc.getNumFrames(), 3)
        }
      }
    }  // Butto.. id: loadFile..
  }
}