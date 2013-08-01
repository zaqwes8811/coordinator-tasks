/**
	1. Открывать(создавать)/закрывать базу данных
	2. Делать запись - на входе новый объект, но без перв. ключа
		на выходе тот же объект, но с первичным ключем
		! если писать массив, то сделать отдельную функция
			лучше цикл в функции чем функция в цикле!
	3. делать выборки - на выходе массив объектов!
	
	[ сливать базы, сохр. где-то ]
	thi : если баз будет несколько, то лучше следать класс, а не глобалные функции
	Предполагается что данные прочитаны
		для точного обновления нужен prim key
		для удаления похоже тоже
	вставка..? - вот тут создается совсем новое
		с ui нужно сразу считать, чтобы узать первичный ключ иначе при создании
			запись будет неуправляемой с ui
	такое ощущение что выборку можно делать по первичному ключу
*/
// функция для реализации наследования
function extend(Child, Parent) {
	var F = function() { }
	F.prototype = Parent.prototype
	Child.prototype = new F()
	Child.prototype.constructor = Child
	Child.superclass = Parent.prototype
}
// собственно он. как такового смысла нет, но
//   для порядка сделают так, а потом просто методы перенесу
function ISQLite(){
	this.open = function( url ){}
	this.createTbl = function( tblName ){}
	
	// вставка содержимого заметки - только одной 
	this.insertRecordInto = function( record, tblName ){}
	
	// чтение возвращает массив заметок с первичными ключами
	this.selectFromLike = function(tblName, key){}
	
	//
	//this.resetLayer =
	//this.handleValid
}
/**
	abs. : синхронные операции с базой данных
*/
function AirSQLiteSync() {
	this._db = null
	this.open = function( url ){
		// ассоциируем с файлом
		var file = new air.File( url ); 

		// создаем соединение
		this._db = new air.SQLConnection();
		
		// возвращаем ошибку и дескриптор базы данных
		var result = false;

		// добавляем слушателя события открытия
		try {
			// пусть пока в синхронном режиме
			this._db.open( file, air.SQLMode.CREATE );
			result = true;
		}
		catch(SQLError) {
			air.trace( 'SQLError : check path to url' );
		}
		return result;
	}
	this.close = function() {
		try {
			// пусть пока в синхронном режиме
			this._db.close(  );
		}
		catch(SQLError) {
			air.trace( 'SQLError : check path to file' );
		}
	}
	this.createTbl = function ( tbl ) {
		// Date[str = 00 00 00 00 00] // возможно еще бы добавить номер для одновр. добавленных
		// Type[do/thi/] 
		// TextContent[str] 
		// LinkOut ( пока нисходящаа связь )
		var stmtCreate = new air.SQLStatement();
		stmtCreate.sqlConnection = this._db;
		stmtCreate.text = "CREATE TABLE IF NOT EXISTS "+tbl+" ( " +
			'id INTEGER PRIMARY KEY AUTOINCREMENT, ' +
			'Date TEXT, ' +
			'Type TEXT, ' +
			'TextContent TEXT, ' +
			'LinkOut TEXT )';
		try {
			stmtCreate.execute();
		}
		catch(SQLError){
			air.trace('SQLError : CREATE TABLE')
		}
	}
	this.insertRecordInto = function ( tbl, record ) {
		var stmt = new air.SQLStatement();
		stmt.sqlConnection = this._db;
		
		// делаем запрос
		stmt.text = 
			"INSERT INTO "+tbl+" (Date, Type, TextContent, LinkOut) " +  
			"VALUES"+record.getInsertString();
		try {
			stmt.execute();
			var result = stmt.getResult(); 
			record.id = result.lastInsertRowID; 
		}
		catch(SQLError){
			air.trace('SQLError : INSERT '+SQLError.message)
		}
	}
	this.selectFromLike = function ( tbl, likeString ) {
		var stmt = new air.SQLStatement();
		stmt.sqlConnection = this._db;
		stmt.text = "SELECT * FROM "+tbl+" "+"WHERE Date LIKE '"+likeString+"'";
		var resultList = new Array();
		try {
			stmt.execute();
			var result = stmt.getResult(); 
			var numResults = result.data.length; 
			for (var i = 0; i < numResults; i++) {
				var row = result.data[i]; 
				var record = new SimpleRecord();
				record.id = row.id; 
				record.Date = row.Date; 
				record.Type = row.Type; 
				record.TextContent = row.TextContent; 
				record.LinkOut = row.LinkOut; 
				resultList.push( record );
			}
		}
		catch(SQLError){
			// сюда же вылетает если таблица была пустой
			air.trace( 'SQLError : SELECT '+SQLError.message)
		}
		return resultList;
	}
}
extend(AirSQLiteSync, ISQLite)

// Run()
function Run(){
	// событие вызывается позже и g_pwd еще не определена
	//var g_pwd = "D:\\Public\\do-small\\Dropbox\\bitbucket\\air-planner\\Planthi-app-branch"
	var g_pwd = "D:\\";
	var url = g_pwd+"testAir.db";
	var airBase = new AirSQLiteSync();
	
	// открываем базу данных
	var err = airBase.open( url );
	
	// создаем таблицу если ее нет
	var tblName = 'AllRecords';
	airBase.createTbl( tblName );
	
	// ! читаем
	var Date = "%1208311500";
	var Type = "do";
	var resultList = airBase.selectFromLike( tblName, Type );
	// прорисовываем на виде если есть результать чтения
	//createCalendarGrid( recordsList )
	//plotGraph( recordsList );
	
	// создаем запись
	var record = new SimpleRecord();
	record.Date = "1208311500";
	record.Type = "do";
	record.TextContent = "testRecord";
	record.LinkOut = "1208311501";

	// Сама вставка записи в базу
	airBase.insertRecordInto( tblName, record );
	air.trace( record.id );	// Это важно
	
	// RAII не получается здесь!
	airBase.close();
}
var g_callsArray

// Возможны проблемы с прокруткой
function createCalendarGrid( recordsList ) {
	// удаляем таблицу если она была
	var mydiv = document.getElementById("boxCentral");
	while ( mydiv.firstChild ) mydiv.removeChild( mydiv.firstChild );

	// создаем
	var arrayOfCells = new Array()
	var len = recordsList.length

	// заполняем записями
	// Как быть если не влезает, может стить менять! схлопнуть и дело с концом - нет не вышло
	for( var i = 0; i < len; i++ ) {
		// создаем новый ряд
		var newCell = document.createElement('div')

		// стилизуем
		newCell.align="middle"
		newCell.id = "it_Div"	// лучше чтобы Id был уникальный!!
		newCell.innerHTML = recordsList[i].Date

		// попытка прикрепить данные
		newCell.linkObj = recordsList[i];

		// добавление обработчика
		newCell.onclick = onClickCell

		// для управления добавляем сюба
		arrayOfCells.push(newCell)  
	}

	// Добавление (может отличаться)
	for( var i = 0; i < len; i++ ) {
		// добавляем в телодокумента
		document.getElementById("boxCentral").appendChild( arrayOfCells[i] )
	}

	//document.getElementById("boxCentral").appendChild(newTable)
	// возвращаем массив ссылок для оперирования с ячейками
	return arrayOfCells
}
/**
	Обработчик клика по элементу
*/
function onClickCell(event){
	event = event || window.event
	// кросс-браузерно получить target
 	var t = event.target || event.srcElement
	 
	// Узнаем какая ячейка нажата ( пока просто пишем в лог
	air.trace( t.linkObj.id )
}

// запуск после загружки приложения, иначе путь к текущей
//   директории нельзя будет использовать
$(window).load(function () {
	Run();
});
$(document).ready(function(){
	$("#btClose").mouseenter(function(){
		$(this).css('background-image','-webkit-gradient(radial, 50% 50%, 0, 50% 50%,20, from(#ffffff), to(red))');
		// background-image: -webkit-gradient(radial, 50% 50%, 0, 50% 50%,100, from(#cde6f9), to(#6B86A6));
	});
	$("#btClose").mouseout(function(){
		//
		$(this).css('background-image','-webkit-gradient(radial, 50% 50%, 0, 50% 50%,20, from(#red), to(red))');
	});
});
