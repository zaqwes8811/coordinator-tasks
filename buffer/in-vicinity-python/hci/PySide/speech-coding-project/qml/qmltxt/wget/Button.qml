import QtQuick 1.0

Rectangle {
     id: button
     transformOrigin: Item.Center

     property int buttonHeight: 75
         property int buttonWidth: 150
     property string label : "button label"
     Text{
         id: buttonLabel
         anchors.centerIn: parent
         text: label
     }

     property color buttonColor: "lightblue"
     property color onHoverColor: "gold"
     property color borderColor: "white"

     /// детализация изображения
     radius: 10
     smooth: true

     border {   // граница
         width: 2; color: borderColor }
     width: buttonWidth; height: buttonHeight
     ///

     // как будто автоматически совмещаются, нет connect()
     signal buttonClick()
     onButtonClick: {  // on<signal_name>
         console.log(buttonLabel.text + " clicked" )
     }
     // как быть если нужно вызвать несколько слотов?

     MouseArea{
         // active place
         anchors.fill: parent //anchor all sides of the mouse area to the rectangle's anchors
         //onClicked handles valid mouse button clicks

         onClicked: buttonClick()

         // зависаем над кнопкой
         hoverEnabled: true  // hover(нависать)
         onEntered: parent.border.color = onHoverColor
         onExited:  parent.border.color = borderColor
     }

     //determines the color of the button by using the conditional operator
     color: button.MouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
 }
