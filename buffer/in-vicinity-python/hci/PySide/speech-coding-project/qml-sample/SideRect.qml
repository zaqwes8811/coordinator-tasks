 import QtQuick 1.0

 Rectangle {
     id: myRect

     property string text

     width: 75; height: 50
     radius: 6  // �� �������� ����� �� ������ ������-��
     color: "#646464"
     border.width: 4; border.color: "white"

	 /// ��� ��� �������������� ��� �� ��������� � ���������
     MouseArea {
         anchors.fill: parent  // ��� ������ fill?
         hoverEnabled: true
         onEntered: {
             focusRect.x = myRect.x;
             focusRect.y = myRect.y;
             focusRect.text = myRect.text;
         }
     }
 }