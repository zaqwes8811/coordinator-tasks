import QtQuick 1.0

 Rectangle {
     id: simplebutton
     color: "grey"
     transformOrigin: Item.Center
     width: 150; height: 75

     Text{
         id: buttonLabel
         anchors.centerIn: parent
         text: "button label"
     }
     // events
     MouseArea{
       id: buttonMouseArea
       anchors.fill: parent //anchor all sides of the mouse area to the rectangle's anchors
       //onClicked handles valid mouse button clicks
       onClicked: console.log(buttonLabel.text + " clicked" )
     }
 }
