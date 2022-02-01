/** 
  file : UiLabels.qml
  abstract : панель куда кладем лэйблы

*/
import QtQuick 1.0
Rectangle {
  id: uiConvertor

  /// property declarations
  property int wi : 200
  property int he : 200
  property double boDick : 1  // ширина выступа 
  
  
  /// js-function
  function iniComponent() {
    he = he+30
  }

  /// object properties
  color : "steelblue"
  radius : 4
  //color: "lightblue"
  width: wi+3; height: he+3
  border {   // граница
    width: boDick; color: "white"
  }
  
  /// child 
}