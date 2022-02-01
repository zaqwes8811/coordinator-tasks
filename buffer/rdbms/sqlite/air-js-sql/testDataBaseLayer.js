/**
	abs. : test
	
	thk : как тестировать асинхронные вызовы
*/

// form site QUnit
$(document).ready(function(){
	// separate test into module
	module("Open DB modele");
	
	// открываем-закрываем
	function asynCall() {
		equal( false, false, "Right path" );
	}
	function testOpenDataBase() {
		// Вля отладки будем хранить файлы не так далеко, как 
		//   хранят air-приложения. Но потом нужно все вернуть
		//var file = air.File.applicationDirectory.resolvePath('crm.db' );
		var urlStr = g_pwd+"\\idat\\testAir.db"; 
		var file = new air.File(urlStr); 
		var resultArray = doLoad( file )
		equal( resultArray["Error"], false, "Right path" );
		var db = resultArray["hDB"];	// !! отсутствие var делает перем. глобальной 
		db.close();
		db = undefined;
		
		// тест непревильного пути
		var urlStr = "фаd:/testAir2.db"; 
		var file = new air.File(urlStr); 
		var resultArray = doLoad( file )
		equal( resultArray["Error"], true, "Wrong path" );
		
		// 
		closeDataBase( db )
		
		asynCall()
	}
		
	// запуск теста?
	//test( "Open and close database",testOpenDataBase );
	
	// Тест асинхронных вызовов
	/*test('asynchronous test', function() {
		// Pause the test first
		stop();
	
		setTimeout(function() {
			ok(true);

			// After the assertion has been called,
			// continue the test
			start();
		}, 100)
	})*/
});







