
/**
  Интерфейс к базе данных
  	При отладке используем совойства html5 по хранению
  		данных не стороне клиента. Отлаживаем в chrome
  	Для настольного приложения
  	? здесь нет стогого контроля типов, поэтому полиморфизм уже по умолчанию
  		и похоже целесообразнее наследовать реализацию
  	переписывать все равно придется
  		но в класс завернуть нужно бы, чтобы потом тупо поменять 
  		в созданнии название конструкора. лучше функции присоединить к объекту
		
	Таблицы будут две
		Основная база данных
		Состояние программы
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
	// дескриптор базы данных
	this.db = null
	//this.tblNotes = ''
	
	// открывает базу данных
	this.open_db = function(dbName){}
	
	// создает таблицу
	this.create_tbl = function(tblName){}
	
	// вставка содержимого заметки
	this.insert = function(tblName,key, strNoteContent){}
	
	// чтение
	this.select = function(tblName, key){}
	
	// обновление
	//this.deleteFile = f
}

// Доступ к локальной базе данных через chrome
// Проблема с именем файла с базой данных - имя файла - цифорка просто
//   и с вызовом функции открытия никак не связано. Можно попытаться совой файл 
//   подсунуть
// Вопросы:
//   1. как обновлять если не создано и тп, хотя считано все равно будет!
function SQLiteHTML5(){
	SQLiteHTML5.superclass.constructor.call(this)
	this.open_db = function(dbName){
		// тип обмена какой? синхронный или асинхронный
		try {
			if (window.openDatabase) {
			db = openDatabase(dbName, "1.0", "HTML5 Database API example", 200000);
			if (!db)
				alert("Failed to open the database on disk.  This is probably because the version was bad or there is not enough space left in this domain's quota");
			} else
				alert("Couldn't open the database.  Please try with a WebKit nightly with this feature enabled");
		} catch(err) {
			db = null;
			alert("Couldn't open the database.  Please try with a WebKit nightly with this feature enabled");
		}  // try {..
	}
	// пока асинхронный режим
	// NTEGER PRIMARY KEY AUTOINCREMENT - создает еще одну таблицу
	this.create_tbl = function(tblName){
		db.transaction(
			function(tx, error) { 
				tx.executeSql(
					"CREATE TABLE IF NOT EXISTS "+tblName+ " ( _ID_NOTE INTEGER PRIMARY KEY AUTOINCREMENT, TimePoint TEXT,  TextContent TEXT )", 
					[], 
					function(result) { 
						tblNotes = tblName
						//alert(this._tblNotes)
					},
					function(){alert("error create")}
				); // tx.execute..
			}
		);  // tx.execu..
	}
	
	// 
	this.insert = function(tblNotes, key, strNoteContent){
		var command = "INSERT INTO "+tblNotes+ " (TimePoint,  TextContent) VALUES ('"+key+"', '"+strNoteContent+"')"
		db.transaction(
			function(tx, error) { 
				tx.executeSql(
					command, 
					[], 
					function(result) { },
					function(){alert("error insert")}
				); // tx.execute..
			}
		);  // tx.execu..
		
	}
	
	// Выборка значений
	this.select = function(tblName, key) {
		db.transaction(function(tx) {
			tx.executeSql("SELECT TextContent FROM "+tblName+" WHERE TimePoint='01'", 
				[], function(tx, result) {
				for (var i = 0; i < result.rows.length; ++i) {
					var row = result.rows.item(i);
					alert( unescape(row['TimePoint']));
				}
			}, function(tx, error) {
				alert('Failed to retrieve notes from database - ' + error.message);
				return;
			});
		});
	}

	// обновление записи 
	/*
	 "UPDATE " + _TableName + " " +  // Имя таблицы
				"SET " + //column + " = '', " + 
				"TextContent = 'xxx' " + 
			  "WHERE " + column + " LIKE 'Bob%'";
	 */
}
// наследуем - скорее для наследования реализации, что нам особо не нужно
//   писать все равно скорее всего придется заново
extend(SQLiteHTML5, ISQLite)

// Создаем модуль для работы с базой данных
function Test_sqlHtml5(){
	var sqlHtml5 = new SQLiteHTML5()
	sqlHtml5.open_db('NotesTable')
	sqlHtml5.create_tbl('Notes')
	
	// просмоторщику базы данных безразличен символ перевода строки
	//sqlHtml5.insert('Notes', '01', 'first \nnote2')
	sqlHtml5.select('Notes', '01')
}
//Test_sqlHtml5()

// Событие загузки формы
// Проверяем существует ли база данных для
//   хранения настроек
// Если ее нет создадим

// События выхода - пока проблема, т.к. как его поймать 


/*
// Air Доступ к локальной базе данных
function SQLiteAIR(){
	SQLiteAIR.superclass.constructor.call(this)
	this.open_db = function(){
		alert("AIR")
	}
}*/