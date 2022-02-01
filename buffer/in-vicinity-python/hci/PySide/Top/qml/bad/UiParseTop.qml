/**
  file : UiParseTop.qml
  abst. : 
*/
import QtQuick 1.0
Rectangle {
  id: uiParseTop
  
  /// prop.
  property int wi : 80
  property int he : 120
  property int num : 4
  property variant labels : ["", "", "", ""]
  
  /// js-fun.
  // функция заполнения
  function setEdit(captionValues, index) {
    // заполняем поля
    //for(var i = 0; i < 4; i++) {  // удобно, но как из через слот передать кортеж
      colRect.getRow(index).setText(captionValues)
    //}
  }
  
  /// obj. prop.
  color : "steelblue"
  radius : 4
  width: wi;  height: he
  
  /// child
  // поля ввода
  Column {
    id : colRect
    /// js-fun.
    // достоп к полям
    function getRow(index) {
      var c  // "указатель на объект"
      if(index == 0) {c = rg0;} 
      else if(index == 1) {c = rg1;} else if(index == 2) {c = rg2;}
      else if(index == 3) {c = rg3;} //switch()  // как реализоваy
      //var c = repRect.itemAt(index)  // пока отмена, т.к. проблемы с Quick 1.1
      return c
    }

    /// obj. prop.
    spacing : 6
    anchors.centerIn : parent
    
    /// child
    //Repeater {  // пока не работает в linux
      //id : repRect
      //model : num //}  // Rep..
    // белое поле для вида
    Rectangle {
      id : rg0
      /// js-fun.
      function setText(val) { tval0.text = val }
      
      /// obj. porp.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// child
      Text { id : tval0
        anchors.centerIn : parent;  text : labels[0] }
    }  // поле вывода i

    // белое поле для вида
    Rectangle {
      id : rg1
      /// js-fun.
      function setText(val) { tval1.text = val }
      
      /// obj. prop.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// child
      Text { id : tval1
        anchors.centerIn : parent;  text : labels[1] }
    }  // поле вывода i

    // белое поле для вида
    Rectangle {
      id : rg2
      /// js-fun.
      function setText(val) { tval2.text = val }
      
      /// obj. prop.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// 
      Text { id : tval2
        anchors.centerIn : parent;  text : labels[2] }
      
    }  // поле вывода i

    // белое поле для вида
    Rectangle {
      id : rg3
      /// js-fun.
      function setText(val) { tval3.text = val }
      
      /// obj. prop.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// child
      Text { id : tval3
        anchors.centerIn : parent;  text : labels[3] }
    }  // поле вывода i
  }  // Colu..
}
