import QtQuick 1.0
import "qml"  // там лежат необходимые файлы
import "js" as Script
Rectangle {
  id : mainRec  // name
  
  /// prop.
  property variant captionValues : [1,2,3,4]  //
  
  /// obj. prop.
  anchors.fill: parent;  
  color: "lightblue" 
  width : uim.wi+po.wi+2*3+1
  height : 500

  /// child
  // панель вывода свойства входного файла
  PanelOut {
    id : po
    Component.onCompleted: { 
      setCaption("<h4>Входной файл</h4>")
      console.debug(po)
    }
  }
  
  // панель выбора параметров анализа
  PanelOut {
    id : po_out
    //anchors.left: po_conv.right;
	anchors.top: po.bottom
    Component.onCompleted: setCaption("<h4>Выходной файл</h4>")
  }
  
  // player
  /*UiRezPanel {
    id : rp
	anchors.top: po_out.bottom
	width : po_out.wi
	height : 40
	radius : 2
	color : "steelblue";
  }*/

  // Плеер
  /*UiConvertor {
    id : po_conv
    anchors.left: po.right;
    
    // иницаилизация при создании
    Component.onCompleted: console.log("Completed Running!")
  }*/
  
  // боковая часть
  // панель вывода сообщений
  UiMesseger {
    id : uim
    anchors.left: po.right;
	he : parent.height  // во всю высоту окна
    MouseArea {  
      anchors.fill : parent
      onClicked :  {
        po.resetActi("white", "steelblue")
        uim.setMessage("asdf")
      }
    }
  }  // UiMess..

}
