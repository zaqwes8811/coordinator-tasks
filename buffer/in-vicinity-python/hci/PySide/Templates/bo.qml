// file : bo.qml
import QtQuick 1.0
//import bbutton
Rectangle {
  id : bo
  // pos.
  property int wi : 160
  property int he : 120
  
  // data
  property variant captionValues : [100, 23, 11, 5]  // данные заголовка
  //
  width : wi;  height : he
  
  // labels
  UiLabels { id : labels
    wi : bo.wi/2
  }
  // textEdits
  UiParseTop { id : test; x : labels.wi
    wi : bo.wi/2
  }
  
  //
  Button {
    id : filler
	anchors.centerIn : parent
	MouseArea {
	  anchors.fill : parent
	  onClicked : {
	    test.setEdit(captionValues)
	  
	  }
	}
  }
}
/*
  общие переменные нужно бы в отдельный файл, как бы константы

*/