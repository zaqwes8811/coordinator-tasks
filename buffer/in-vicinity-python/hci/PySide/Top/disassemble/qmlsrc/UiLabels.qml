// file : UiLabels.qml
import QtQuick 1.0

Rectangle {
  id: uiLabels
// панель куда кладем лэйблы
  property int wi : 80
  property int he : 120
  property int num : 4
  property variant labels : ["nchannels: ", "sampwidth, bit: ", "framerate, Hz: ", "nframes: "]

  //
  color : "steelblue"
  radius : 4
  //color: "lightblue"
  width: wi
  height: he
  
  // поля ввода
  Column {
    id : colRect
	spacing : 6
	Repeater {
	  id : repRect
	  model : num
	  Rectangle {
	    color: uiLabels.color
		width: wi
		height: (he-(num-1)*colRect.spacing)/num
		radius : 4
		Text {
		  anchors.verticalCenter: parent.verticalCenter
		  anchors.right: parent.right
		  text : labels[index]
		  color : "white"
		}
	  }
	}  // Rep..
  }  // Colu..
}