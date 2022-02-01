import Qt 4.7
Rectangle {
  id : mainRec  // name
  anchors.fill: parent;  // position
  color: "lightblue"  // 
  
  //Описание сигнала
  signal wantquit 
  property int qwX: 0;  property int qwY: 0
  property int owX: 0;    property int owY: 0 
  property bool first: true	
 
  /* Функция передающая текст для вывода текстовым виджетом*/
  function updateMessage(text) {
    messageText.text = text
  }
  /*Button{
             id: loadButton
             buttonColor: "lightgrey"
             label: "Load"
         }
    */

  //Текстовый виджет
  Text {
    id: messageText; 
	anchors.centerIn: parent; color: "white"
  }

  //Обработка событий вызванных мышью
  MouseArea {
    anchors.fill: parent
    onClicked: 
    {
      // Взятие текста методом класса файла python
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
		
		//Перемещение окна
		main.form_move(qwX, qwY)
	    }
	    
                  onDoubleClicked:
	    {
	    //Отправка сигнала
	    wantquit()
	    }*/
    }
}