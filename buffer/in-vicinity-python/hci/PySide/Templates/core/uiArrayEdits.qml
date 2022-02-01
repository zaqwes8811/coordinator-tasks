// # -*- coding: utf-8 -*-
/**
  file : uiArrayEdits.qml
  
  Description :
    настариваемый массив полей ввода
  
  Autor :
  Lugansky Igor
  
  Changes :
  5-oct-11 LI создан
  6-oct-11 LI поля вывода заменены на поля ввода-вывода

*/
import QtQuick 1.0

Rectangle {
  id: uiParseTop
// панель куда кладем лэйблы
  property int wi : 80
  property int he : 120
  property int num : 4
  property variant labels : ["", "", "", ""]
  //
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
        color: "white"
        width: wi-colRect.spacing
        height: (he-(num-1)*(colRect.spacing+2))/num
        radius : 4
		
		// само поле ввода
        TextEdit {
		  anchors.centerIn : parent
          text: "Hello"
          font.family: "Helvetica"
          font.pointSize: 10
          //color: "blue"
          focus: true
        }
        // доступ к полям
        function setText(val) {
          tval.text = val
        }
		function getText() {
          return tval.text
        }
      }
    }  // Rep..
    
    //
    function getRow(index) {
      var c = repRect.itemAt(index)
      return c
    }
    
  }  // Colu..
  // signals 
  
  // функция заполнения
  function setEdit(captionValues, index) {
    // заполняем поля
    //for(var i = 0; i < 4; i++) {
      colRect.getRow(index).setText(captionValues)
    //}
  }
}
