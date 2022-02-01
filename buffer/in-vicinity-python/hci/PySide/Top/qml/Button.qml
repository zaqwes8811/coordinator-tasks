import QtQuick 1.0

Rectangle {
  /*color: "lightblue";
  border.color: "steelblue"
  border.width: 2
  radius: 10		
  width: wi; height: he 
		smooth : true
		gradient: Gradient {
          GradientStop { position: 0.0; color: "lightsteelblue" }
          GradientStop { position: 1.0; color: "steelblue" }
        }*/


     id: button
     transformOrigin: Item.Center

     property int buttonHeight: 30
         property int buttonWidth: 60
     property string label : "Test"
     Text {
         id: buttonLabel
         anchors.centerIn: parent
         text: label
     }

     property color buttonColor: "lightblue"
     property color onHoverColor: "gold"
     property color borderColor: "white"
	 property string valf : "none"

     /// детализация изображения
     radius: 10
     smooth: true

     border {   // граница
         width: 2; color: borderColor }
     width: buttonWidth; height: buttonHeight
     ///

     // как будто автоматически совмещаются, нет connect()
     //signal buttonClick()
     //onButtonClick: {  // on<signal_name>
       //  console.log(buttonLabel.text + " clicked" )
	//	 con.outputStr("asdfasdf")
     //}
     // как быть если нужно вызвать несколько слотов?

	 
    MouseArea{
         // active place
         anchors.fill: parent //anchor all sides of the mouse area to the rectangle's anchors

         // зависаем над кнопкой
         hoverEnabled: true  // hover(нависать)
         onEntered: parent.border.color = onHoverColor
         onExited:  parent.border.color = borderColor
     }
	 
     //determines the color of the button by using the conditional operator
     color: button.MouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
 }
