// file : UiMesseger.qml
import QtQuick 1.0
Rectangle {
  property int wi : 200
  property int he : 100
  property int rad : 4
  id : uiMesseger
  width : wi;  height : he; 
  color : "lightblue"
  radius : rad
  
  // поле с сообещением
  property string msgs : ""  // сообщение
  
  // настройка расположения
  property double a : 0.4  // панель действий
  property double b : 0.4  // панель сообщений
  
  Column {
    id : colRect
    anchors.centerIn : parent
    anchors.fill : parent
    // заголовок
    Rectangle {
      width : wi; height : he*(1-b-a)
      color : "steelblue"; radius : rad
      Text {
        anchors.centerIn : parent
        text : "<h4>Сообщения</h4>"
        color : "white"
      }
    }

    // сами сообщения
    Rectangle {
	//anchors.centerIn : parent
    //anchors.fill : parent
      width : wi; height : he*b
      color : "steelblue"; radius : rad
      Text {  // поддерживает ли многострочный вывод
        text : msgs  // ! важно, нужна прокрутка
        color : "white"
      }
    }
    
    // actions
    Rectangle {
      width : wi; height : he*a
      color : "steelblue"; radius : rad
      Text {
        text : "asdf"
        color : "white"
      }
    }
  }  // ..Column {..
  
  // вывод сообщения
  function setMessage(msg) {
    uiMesseger.msgs = msg
  }
}