// file : month_engine.js
// набор констант
var DAY_IN_WEEK = 7
var NUM_SHOWED_WEEK = 9
var CENTER_POS = 3+1	// положение "горизонта"
/**
thinks : может нумерацию всего сделать с нуля
  сумма по модулю прох. через ноль, нумерация массивов с нуля, 
    многовато ошибок на границах
*/
var numDaysMap 
numDaysMap = {
	n00 : 31-1, // янв. 0
	n10 : 28-1, // фев. 1
	 
	n20 : 31-1, // мар. 
	n30 : 30-1, // апр. 
	n40 : 31-1, // май 
	
	n50 : 30-1, // . 
	n60 : 31-1, // . 
	n70 : 31-1, // . 
	
	n80 : 30-1, // . 
	n90 : 31-1, // . 
	n100 : 30-1, // . 
	
	n110 : 31-1, // дек. 11 ! а не 12
	
	n11 : 29-1, // февр. вис. 
} 


// массив ссылок на ячейки таблицы с календарем
var g_callsArray

var day = 2-1
var mon = 6-1
var year = 2012

var gNumDayInCurrMonth = getNumDaysInMonth((mon+12)%12, year)

var g_colorMap = new Array()
g_colorMap[0] = '#FFB'
g_colorMap[1] = '#FFC'
g_colorMap[2] = '#FFB'
g_colorMap[3] = '#FFC'

// раскарска месяцев
function routeg_colorMapUp(  ){
	var tmp = g_colorMap[3] 
	g_colorMap[3] = g_colorMap[2]
	g_colorMap[2] = g_colorMap[1]
	g_colorMap[1] = g_colorMap[0]
	g_colorMap[0] = tmp
}
function routeg_colorMapDown(  ){
	var tmp = g_colorMap[0]
	g_colorMap[0] = g_colorMap[1]
	g_colorMap[1] = g_colorMap[2]
	g_colorMap[2] = g_colorMap[3]
	g_colorMap[3] = tmp
}

/** 
  создает и стилизует сетку с месяцами
*/
function createCalendarGrid( ) {
	// создаем
	var arrayOfCells = new Array()
	var newTable = document.createElement('table')
	newTable.border = "0"
	newTable.cellspacing = "0"
	newTable.id = "month_view"
	for( var i=0; i< NUM_SHOWED_WEEK; i++ ) {
		// создаем новый ряд
		var newRow = document.createElement('tr')
		
		// новый столбец
		for(var j = 0; j < DAY_IN_WEEK; j++) {
			// новая ячейка
			var newCell = document.createElement('td')
			
			// стилизуем
			newCell.align="middle"
			newCell.id = "itd"
			
			// добавление обработчика
			newCell.onclick = onClickCell
			
			// для управления добавляем сюба
			arrayOfCells.push(newCell)  
			
			// добавления поля
			newRow.appendChild(newCell)
		}
		
		// добавляем строку в таблицу
		newTable.appendChild(newRow)
	}

	// добавляем в телодокумента
	document.getElementById("boxCentral").appendChild(newTable)
	
	//
	return arrayOfCells
}

/**
 * abs.: заполнение календаря при нажатии вверх
 */
function positionCalendarContent( day, mon, year ){
	strLog = ''
	// сбрасываем все ячейки(нужно ли?)
	for(var i=0; i<DAY_IN_WEEK*NUM_SHOWED_WEEK; i++) {
		g_callsArray[ i ].innerHTML = ' ' 
		g_callsArray[ i ].style.backgroundColor = '#FFF'//gold'	// для отладки
	};

	// выводим метку
	var weekDay = getDay( day, mon, year )	// пн - 0
	var corrValue = DAY_IN_WEEK*CENTER_POS	// поправка для вывода на горизонт
	posInArray = corrValue+weekDay		// положение центрального дня в общем массиве
	var string = (day+1).toString()
	g_callsArray[ posInArray ].innerHTML = string
	g_callsArray[ posInArray ].style.backgroundColor = 'gold'	// для отладки
	
	// Рисуем окружение центарльной даты, пока один месяц
	numDaysInPrevMon = getNumDaysInMonth((mon+12-1)%12, year)
	numDaysInCurrMon = getNumDaysInMonth((mon+12)%12, year)
	numDaysInNextMon = getNumDaysInMonth((mon+12+1)%12, year)
	
	
	// заполняем на понижение
	jumpMul = 1
	down = 1
	var i = 0
	currenMonColor = '#FFC'
	while (  posInArray-down+1 > 0 ) {	// ограничение по общему объему вниз
		if(down < day+1) {	// заполняем текущий месяц
			g_callsArray[ posInArray-down ].innerHTML = ((day+1)-down).toString() 
			g_callsArray[ posInArray-down ].style.backgroundColor = g_colorMap[1]	// для отладки
		}
		else {
			position = posInArray-down-7*jumpMul	// пусть с разрывом
			if(position+1 > 0){	// числа могут подходит вплотную
				g_callsArray[ position ].innerHTML = (numDaysInPrevMon+1-i).toString()
				g_callsArray[ position ].style.backgroundColor = g_colorMap[0]	// для отладки
				i++
			}
		}
		down++
	}
	
	// заполняем на повышение
	down = 1
	i = 1
	while ( posInArray+down < DAY_IN_WEEK*NUM_SHOWED_WEEK ) {	// до конца видимого массива
		if( down+day-1 < numDaysInCurrMon ){
			position = posInArray+down
			g_callsArray[ position ].innerHTML = (day+down+1).toString()
			g_callsArray[ position ].style.backgroundColor = g_colorMap[1]	// для отладки
		}
		/////*
		else{
			position = posInArray+down+7*jumpMul
			if( position < DAY_IN_WEEK*NUM_SHOWED_WEEK && i < numDaysInNextMon) 
			{
				g_callsArray[ position ].innerHTML = (i).toString()
				g_callsArray[ position ].style.backgroundColor = g_colorMap[2]	// для отладки
				i++
			}
		}
		down++
	}
	// набираем лог
	logAppend(' '+(numDaysInPrevMon+1).toString() )
	logAppend(' '+(numDaysInCurrMon+1).toString())
	logAppend(' '+(numDaysInNextMon+1).toString())
	logAppend(' = '+year.toString()+' : '+(mon+1).toString())
	printLog()
}

/**
 * получить число дней в месяце с учетом висок. годов
 * 
 */
function getNumDaysInMonth( mon, year ) {
	// проверка на високосность
	var key = 'n'+mon.toString()
	var keyYear = '0'
	
	// февраль и високосный
	if( year%4 == 0 && mon == 1 ){ keyYear = '1' } 
	key += keyYear
	return numDaysMap[key] 
}
/**
	получить день недели с вики. Нумерация с нуля
*/
function getDay( day, mon, year ){
	day += 1
	mon += 1
	//var days = new Array("воскресенье","понедельник","вторник","среда","четверг","пятница","суббота");
	var days = new Array(7-1, 1-1, 2-1, 3-1, 4-1, 5-1, 6-1);
	day=parseInt(day, 10); //если день двухсимвольный и <10 
	mon=parseInt(mon, 10); //если месяц двухсимвольный и <10 
	var a=parseInt((14-mon)/12);
	var y=year-a;
	var m=mon+12*a-2;
	var d=(7000+parseInt(day+y+parseInt(y/4)-parseInt(y/100)+parseInt(y/400)+(31*m)/12))%7;
	return days[d];
}

// // //
//
// Обработчики событий
//
// // //

/** обработчик нажатия на ячейку в таблице
 вроде бы можно вызвать через обработчик верхнего уровня DOM
   чтобы каждой ячейке не выделять обработчик */
/**
* My method description.  Like other pieces of your comment blocks, 
* this can span multiple lines.
*
* @method onClickCell
* @param {event} foo Argument 1
*/
function onClickCell(event){
	event = event || window.event
	// кросс-браузерно получить target
 	var t = event.target || event.srcElement
	 
	// Узнаем какая ячейка нажата ( пока просто пишем в лог
	toLog( t.innerHTML )
}

/**
	можно сказать, что конструктор интерфейса
*/
$(window).load(function () {
	// Создаем сетку календаря
	g_callsArray = createCalendarGrid()
	
  // Получаем из базы данные сохраненную дату
  //   и если она пустая ставим по умолчанию
  
	
	// Установка позиции
	positionCalendarContent(day, mon, year)
});

/***/
function onClickUp() {
	day += 7
	numDayCurr = getNumDaysInMonth( mon, year )
	if( day > numDayCurr ) {
		if( mon == 11 ) {	year += 1	}
		mon = (mon+1+12)%12
		day = day%numDayCurr-1
		// 
		routeg_colorMapUp()
	}
	setLogContent( ' < ')
	//
	positionCalendarContent(day, mon, year)
}
/***/
function onClickDown() {
	day -= 7
	if(day < 0) {	// пошел следующий месяц
		// проверяем перекинулся ли год
		if( mon == 0 ) {	year -= 1	}
		// прокручиваем месяц
		mon = (mon-1+12)%12
		day = getNumDaysInMonth( mon, year )+day+1
		routeg_colorMapDown()
	}
	setLogContent( ' > ')
	positionCalendarContent(day, mon, year)
}

