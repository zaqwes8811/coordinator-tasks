// file : PanelOut.qml
import QtQuick 1.0
Rectangle {
  id : panelOut
  // pos.
  property int wi : 180
  property int he : 120
  property double part : 0.6
  
  color : "lightblue"
 
  //
  width : wi;  height : he
  
  // labels
  UiLabels { id : labels
    wi : panelOut.wi*part
  }
  // textEdits
  UiParseTop { id : test; x : labels.wi
    wi : panelOut.wi*(1-part)
  }
  //
  function setEdits(captionValues, index) {
    test.setEdit(captionValues, index)
  }
}
/*
  общие переменные нужно бы в отдельный файл, как бы константы

*/
