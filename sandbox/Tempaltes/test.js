/*$(document).ready(function(){
	$(".btn-slide").click(function(){
		$("#panel").slideToggle("high");
		$(this).toggleClass("active"); return false;
	});
});*/

function options(str){
	createTable() 
}

// создает таблицу
function createTable() {
	// создаем
	var newTable = document.createElement('table')
	newTable.border = "1"
	newTable.cellspacing = "0"
	for( var i=0; i<9; i++ ) {
		// создаем новый ряд
		var newRow = document.createElement('tr')
		
		// новый столбец - конечной элемент
		for (var j = 0; j < 7; j++) {
			// новая ячейка
			var newCell = document.createElement('td')
			
			// добавление обработчика
			newCell.onclick = onClickCell
			
			// офформление поля
			newCell.innerHTML = j.toString()
			
			// добавления поля
			newRow.appendChild(newCell)
		}
		
		// добавляем строку в таблицу
		newTable.appendChild(newRow)
	}
	
	// добавляем в телодокумента
	document.getElementById("boxCentral").appendChild(newTable)//innerHTML=newValue;
}







