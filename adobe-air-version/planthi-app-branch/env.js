// Получаем текущий каталог
//D:\\Public\\do-small\\Dropbox\\bitbucket\\air-planner\\Planthi-app-branch
var g_pwd;
air.NativeApplication.nativeApplication.addEventListener(air.InvokeEvent.INVOKE, onInvokeEvent);
function onInvokeEvent(invokeEvent) { 
    arguments = invokeEvent.arguments; 
    currentDir = invokeEvent.currentDirectory; 
	g_pwd = invokeEvent.currentDirectory.nativePath;
}

//
function reverseStr(str) {
    return str.split("").reverse().join("");
}

//
function closeApp(){
window.nativeWindow.close();
}

