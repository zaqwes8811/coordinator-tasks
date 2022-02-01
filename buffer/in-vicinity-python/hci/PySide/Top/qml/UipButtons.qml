// file : uipButtons.qml
import QtQuick 1.0
Rectangle {
  id : uipButtons
  
  /// prop.
  property int he : 30
  property int wi : 60
  
  /// obj. prop.
  color : "steelblue"; radius : 4
  
  /// child
  Row {
    spacing: 4
    anchors.centerIn : parent
    /// child
    Button {  //
      id: loadButton
	  ///
      buttonColor: "lightgrey"; label: "Show"
	  width : wi; height : he
	  ///
      MouseArea {
        anchors.fill: parent
        onClicked: {
          wProc.showWFandSG()
        }
      }
    }
    
    // временная кнопка, пусть пока будет
    // Открывает диалог
    Button {  //
      id: loadFile
      buttonColor: "lightgrey"; label: "LoadFile"
	  ///
	  width : wi; height : he
      ///
      MouseArea {
        anchors.fill: parent
        onClicked: {
          wProc.loadFile()
		  /// лучше то в массив собрать и одной функцией отправить
          panelOut.setEdits(wProc.getNumChannals(), 0)
          panelOut.setEdits(8*wProc.getSampWidth(), 1)
          panelOut.setEdits(wProc.getFrameRate(), 2)
          panelOut.setEdits(wProc.getNumFrames(), 3)
        }
      }
    }  // Butto.. id: loadFile..
  }
}