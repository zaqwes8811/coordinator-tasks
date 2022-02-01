import QtQuick 1.0

Rectangle {
  id: uiParseTop
// панель куда кладем лэйблы
  property int wi : 80
  property int he : 120
  property int num : 4
  property variant labels : ['a', 'b', 'c', 'd']
  color : "steelblue"
  radius : 4
  width: wi;  height: he

  
  // поля ввода
  Column {
    id : colRect
    spacing : 6
	anchors.centerIn : parent
	Repeater {
	  id : repRect
	  model : num
	  // белое поле для вида
	  Rectangle {
        property string s : ""

	    color: "white"
		width: wi-colRect.spacing
		height: (he-(num-1)*(colRect.spacing+2))/num
		radius : 4
		Text {
		  id : tval
		  anchors.centerIn : parent
		  text : labels[index]
		}
      }
	}  // Rep..
    // 
    //function gettt(index) {
      //repRect.itemAt(index).s = "asdf"
    //}
  }  // Colu..
  // signals 
  
  // функция заполнения
  function setEdit(captionValues, index) {
  //    labels.a 
 // colRect.gettt(0)
    labels[index] = captionValues
    console.log(labels[index])
  }
}
