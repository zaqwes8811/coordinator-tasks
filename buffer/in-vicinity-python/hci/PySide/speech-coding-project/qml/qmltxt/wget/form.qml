import Qt 4.7
Rectangle {
  id : mainRec  // name
  anchors.fill: parent;  // position
  color: "lightblue"  // 
  
  //�������� �������
  signal wantquit 
  property int qwX: 0;  property int qwY: 0
  property int owX: 0;    property int owY: 0 
  property bool first: true	
 
  /* ������� ���������� ����� ��� ������ ��������� ��������*/
  function updateMessage(text) {
    messageText.text = text
  }
  /*Button{
             id: loadButton
             buttonColor: "lightgrey"
             label: "Load"
         }
    */

  //��������� ������
  Text {
    id: messageText; 
	anchors.centerIn: parent; color: "white"
  }

  //��������� ������� ��������� �����
  MouseArea {
    anchors.fill: parent
    onClicked: 
    {
      // ������ ������ ������� ������ ����� python
        messageText.text = "asdf"
		updateMessage(messageText.text)
        con.outputStr("Hello, world!")
        first = true
        }
        /*
        onPositionChanged: 
        {
		owX = first? mouseX : owX
		owY = first? mouseY : owY
		first = false
		qwX+=mouseX-owX
		qwY+=mouseY-owY
		
		//����������� ����
		main.form_move(qwX, qwY)
	    }
	    
                  onDoubleClicked:
	    {
	    //�������� �������
	    wantquit()
	    }*/
    }
}