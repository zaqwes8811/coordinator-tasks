/**
  file : UiPrseTop.qml

  Abstract : блок полей ввода-вывода
  
  Notes : 
    var c = repRect.itemAt(index)  
    пока отмена, т.к. похоже проблемы с Quick 1.1
    Repeater {  // пока не работает в linux
      //id : repRect
      //model : num //}  // Rep..
      
  Autor : Igot Lugansky 
      
  Mod. history : 
    26-oct IL добавил свич 
*/

import QtQuick 1.0

Rectangle {
  id: uiParseTop
  
  /// prop.
  property int wi : 80  // некоторые по умолчанию
  property int he : 120
  property int num : 4
  property variant labels : ["", "", "", ""]
  
  /// js-fun.
  // функция заполнения
  function setEditText(captionValues, index) {
      colRect.getRow(index).setText(captionValues)
  }
  function getEditText(index) {
      return colRect.getRow(index).getText()
  }
  
  /// obj. prop.
  color : "steelblue"
  radius : 4
  width: wi;  height: he
  
  /// child
  Column {
    id : colRect
    
    /// js-fun
    // нужно вернуть объект, чтобы к нему был доступ
    function getRow(index) {
      var c  
      switch(index) {
        case 0 : c = rg0; break
        case 1 : c = rg1; break
        case 2 : c = rg2; break
        case 3 : c = rg3; break
        default : c = rg0; break
      }
      return c
    }
    
    /// obj. prop.
    spacing : 6
    anchors.centerIn : parent
    
    /// child
    Rectangle {  /// ///
      id : rg0
      
      /// js-fun.
      function setText(val) { tval0.text = val }
      function getText() { return tval0.text }
      
      /// obj. prop.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// child
      TextEdit {
        id : tval0
        anchors.centerIn : parent;  text : labels[0]
      }
    }  // поле вывод а i
    Rectangle {  /// ///
      id : rg1
      
      /// js-fun. 
      function setText(val) { tval1.text = val }
      function getText() { return tval0.text }
      
      /// obj. prop.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// child
      TextEdit {
        id : tval1
        anchors.centerIn : parent;  text : labels[1]
      }
    }  // поле вывода i
    Rectangle {
      id : rg2
      
      /// js-fun.
      function setText(val) { tval2.text = val }
      function getText() { return tval0.text }
      
      /// obj. prop.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// child
      TextEdit {
        id : tval2
        anchors.centerIn : parent;  text : labels[2]
      }
    }  // поле вывода i
    Rectangle {  /// ///
      id : rg3
      
      /// js-fun.
      function setText(val) { tval3.text = val }
      function getText() { return tval0.text }
      
      /// obj. prop.
      color: "white";  width: wi-colRect.spacing
      height: (he-(num-1)*(colRect.spacing+2))/num;  radius : 4
      
      /// child
      TextEdit {
        id : tval3
        anchors.centerIn : parent;  text : labels[3]
      }
    }  // поле вывода i
  }  // Colu..
}
