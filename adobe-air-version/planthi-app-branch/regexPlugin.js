// обработк полученной из файла строки
function processOneType(typeString, srcString) {
	//var i = 0;
	var result = srcString
	var head = typeString+"[^\:]*?:"
	var bottom = "[^\:]*?\:"
	var reReplace = new RegExp(head,"g")
	result = result.replace(reReplace, ':'+typeString+' :')
	var myRe = new RegExp(head+bottom,"gms")
	var arrayResults = new Array();
	while ((myArray = myRe.exec(result)) != null) {
		var msg = myArray[0];
		
		// удаляем обрывок - пока просто переворачиваем строку
		msg = reverseStr(msg) 
		msg = msg.replace(/:[^\:]*?\t*?\n\r/gms,'')
		msg = reverseStr(msg) 
		
		// вот это выражение нужи заменить
		//var msgNew = "<:"+msg+"</"+typeString+i.toString()+">"
		var msgNew = "<:"+msg+"</"+typeString+">"
		msgNew = msgNew.replace(' :',"\>")
		//msgNew = msgNew.replace( typeString,typeString+i.toString())
		result = result.replace(":"+msg,msgNew)
		//i++;
	}
	return result;

} 

