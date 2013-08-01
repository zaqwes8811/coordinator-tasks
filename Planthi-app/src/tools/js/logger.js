g_Log = ''
function toLog( string ){
	// запись в лог
	setLogContent( string );
	air.trace( getLogContent() );
}
function toLogApp( string ){
	logAppend( string );
	air.trace( getLogContent() );
}

/** Function for job with global wars */
function logAppend( string ){
	g_Log += string;
}
function getLogContent(){
	return g_Log;
}

function setLogContent( string ){
	g_Log = string;
}

function printLog(){
	air.trace( getLogContent() );
}
