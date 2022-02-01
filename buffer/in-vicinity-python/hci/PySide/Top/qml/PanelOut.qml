// file : PanelOut.qml
import QtQuick 1.0
Rectangle {
  id : panelOut

  /// prop.
  property int wi : 200
  property int he : 200
  property double part : 0.6  // соотношения ширин полей вывода метки и сами поля
  property double he_part_a: 0.7  // под поле вывода
  property double he_part_b: 0.2  // под кнопки
  property double boDick : 1  // наверное насколько выступает граница
  property string c : "white" 
  property string c_g : "steelblue"
  property string caption : "<h4>Входной файл</h4>"
  
  /// js-functions
  function setEdits(captionValues, index) { 
    col.getPtr().setEditText(captionValues, index) 
  }
  // установка активности
  function setActi(iniCColor) {
    c = iniCColor; c_g = iniCColor
  }
  // снятие активности
  function resetActi(iniBColor, iniCColor) {
    c = iniBColor; c_g = iniCColor
  }
  // установка заголовка - будет вызваться в конструкторе
  function setCaption(ext_caption) { caption = ext_caption }
  
  /// object prop.
  color : c
  border {   // граница
    width: boDick; color: "white"
  }
  width : wi+3;  height : he+3
  radius : 4
  
  /// child
  Column {
    id : col
    
    /// js-function
    function getPtr() {
      return wl.getPtr()
    }
    ///
    anchors.centerIn : parent
    width: wi
    height : he
    // заголовок
    Rectangle {
      width : panelOut.wi; height : he*(1-he_part_b-he_part_a)
      color : c_g; radius : 4
      Text {
        anchors.centerIn : parent
        text : caption
        color : "white"
      }

      // активизация окна
      MouseArea {
        anchors.fill : parent
        onClicked : {
          panelOut.setActi("gold")
          // здесь будет генерироваться сигнал о том что окно выбрано
        }
      }  // MouseA..
    }
  
    // поля ввода вывода
    Rectangle {
      id : wl
      
      /// js-fun.
      function getPtr() {
        return test
      }
      
      ///
      
      width : panelOut.wi
      height : panelOut.he*he_part_a
      color : panelOut.color
      UiLabels { id : labels
        wi : panelOut.wi*part
        he : panelOut.he*he_part_a
      }
      // textEdits
      UiParseTopIFile { 
        id : test; x : labels.wi
        wi : panelOut.wi*(1-part)
        he : panelOut.he*he_part_a
      }
    }
  
    // buttons
    UipButtons {
      id : uipb
      width : panelOut.wi; 
      height : panelOut.he*(panelOut.he_part_b)
    }
  }
  
  /// states
  
  /// transitions
}
/*
  общие переменные нужно бы в отдельный файл, как бы константы

*/