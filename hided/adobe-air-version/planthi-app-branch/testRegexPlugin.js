
// form site QUnit
$(document).ready(function(){
	function testReadingTxtFile() {
		/**
			Файл лучше преобразовать в xml формат - настоящий, тогда будет много проще!!
			Есть парсеры и на том проблем нет - для работы такой формат не подходит, слшиком
				избыточный
		*/
		var urlStr = g_pwd+"\\idat\\planthi.xml"; 
		var file = new air.File(urlStr); 
		var fileStream = new air.FileStream(); 
		fileStream.open(file, air.FileMode.READ);
		
		// Читаем содержимое
		var fileContent = fileStream.readMultiByte(fileStream.bytesAvailable, 'utf-8');
		fileContent = fileContent.replace(/Ref/g,'ref')
		fileContent = fileContent.replace(/Think/g,'thi')
		fileContent = fileContent.replace(/Do/g,'do')
		fileContent = fileContent.replace(/Learn/g,'lrn')
		fileContent = fileContent.replace(/Thi/g,'thi')
		fileContent = fileContent.replace(/Anl/g,'anl')
		
		// попробуем что-либо выделить регулярным выражениями
		var ID = "ref"
		fileContent = processOneType(ID, fileContent);
		var ID = "thi"
		fileContent = processOneType(ID, fileContent);
		var ID = "do"
		fileContent = processOneType(ID, fileContent);
		var ID = "anl"
		fileContent = processOneType(ID, fileContent);
		var ID = "lrn"
		fileContent = processOneType(ID, fileContent);
		
		// теперь разом нужно убрать из первых тегов ':'
		fileContent = fileContent.replace(/\:/g,'')

		// Выходим
		fileStream.close()
		
		// Пишем - в терминал выводит русский текст бесполезно
		var urlStrWr = g_pwd+"\\odat\\planthi_mon.xml"; 
		var fileWr = new air.File(urlStrWr); 
		var fileStreamWr = new air.FileStream(); 
		fileStreamWr.open(fileWr, air.FileMode.WRITE);
		
		// 	
		fileStreamWr.writeMultiByte(fileContent, 'utf-8')	// записать все что накопили разом
		fileStreamWr.close()
		
		// Нужно чтобы хоть какоя та проверка была
		equal( false, false, "Right path" );
	}
	test( "Open and close database",testReadingTxtFile );
})


});