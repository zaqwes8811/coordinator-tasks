g_calcMouseCoordEna = false
function checkCoordCalkEna(){
	return	g_calcMouseCoordEna;
}

function mouseCoordCalcEna(){
	g_calcMouseCoordEna = true;
	air.trace('X=')
}
function mouseCoordCalcDisa(){
	g_calcMouseCoordEna = false;
	air.trace('X=')
}

function SetValues(){
	// Если расчет координат разрешен
	if( checkCoordCalkEna() ){
		air.trace('X=' + window.event.clientX + ' Y=' + window.event.clientY);
	}
}

function closeApp(){
alert('asfd')
window.nativeWindow.close();
}

