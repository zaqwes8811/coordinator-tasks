 import QtQuick 1.0

 Rectangle {
     id: myRect

     property string text

     width: 75; height: 50
     radius: 6  // на бегающий кубик не влияет почему-то
     color: "#646464"
     border.width: 4; border.color: "white"

	 /// что это значитнаверное что то связанное с движением
     MouseArea {
         anchors.fill: parent  // что значит fill?
         hoverEnabled: true
         onEntered: {
             focusRect.x = myRect.x;
             focusRect.y = myRect.y;
             focusRect.text = myRect.text;
         }
     }
 }