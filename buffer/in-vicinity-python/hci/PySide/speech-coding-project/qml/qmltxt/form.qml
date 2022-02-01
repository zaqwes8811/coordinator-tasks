import Qt 4.7
Rectangle {
  id : mainRec  // name
  anchors.fill: parent;  // position
  color: "lightblue"  // 
  property  int a: 0
  property  int b: 0
  property  int c: 0
  property  int d: 0
  property string s : "asdf"
  // ��������� ������
  Row {
    anchors.centerIn: parent
    spacing: parent.width/12
	
	// �������� ����� � ����������� ��������
    Button {  //
      id: loadButton
      buttonColor: "lightgrey"
      label: "Load"
	  MouseArea {
         anchors.fill: parent
         onClicked: {
		   con.outputStr(s)
		 }
	  }
    }
	// �������� ���������
	Button {
	  id: saveButton
      buttonColor: "grey"
      label: "Show"
	  MouseArea {
        anchors.fill: parent
        onClicked: {
		    
		   a = con.showParam()
		   console.log(a)
		}
	  }
    }
  }
  
  // function
  function updateMessage(text) {
    messageText.text = text
  }
  // ��������� ������
  Text {
    id: messageText; 
	anchors.centerIn: parent; color: "white"
  }
  
  // menu
  /*ListView{
         id: menuListView

         //����� (anchors) ��������� � �������� ������������� ����������
         anchors.fill:parent
         anchors.bottom: parent.bottom
         width:parent.width
         height: parent.height

         //model �������� ������
         model: menuListModel

         //������������ ����� ������ ��� ����� ����
         snapMode: ListView.SnapOneItem
         orientation: ListView.Horizontal
         boundsBehavior: Flickable.StopAtBounds
         flickDeceleration: 5000
         highlightFollowsCurrentItem: true
         highlightMoveDuration:240
         highlightRangeMode: ListView.StrictlyEnforceRange
     }
  VisualItemModel{
    id: menuListModel
    FileMenu{
      width: menuListView.width
      meight: menuBar.height
      color: fileColor
    }
    EditMenu{
      color: editColor
      width:  menuListView.width
      height: menuBar.height
    }
  }*/
  TextEdit {
    id: textEditor
    anchors.fill:parent
    width:parent.width; 
	height:parent.height
    color:"midnightblue"
    focus: true
    wrapMode: TextEdit.Wrap
    //onCursorRectangleChanged: flickArea.ensureVisible(cursorRectangle)  // ��� �� javascript
  }

}