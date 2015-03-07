#-*- coding : utf-8 -*-
# you need run oofice from command line so: 
""" 

soffice -accept="socket,host=localhost,port=2002;urp;

"""
# еще можно при запущенном квикстартере работать
''' 
	Autor : cp from uno examples 
	
	Question:
		Позиционирование курсора?
		Настройки прочих элементов существующих файлов
'''
import os
import uinit
import uno

# Begin Main
''' Here is the sequence of things the lines do:
1.  Get the uno component context from the PyUNO runtime
2.  Create the UnoUrlResolver
3.  Get the central desktop object
4.  Declare the ServiceManager
5.  Get the central desktop object
6.  Access the current writer document
7.  Access the document's text property
8.  Create a cursor
9.  Insert the text into the document '''
 
localContext = uno.getComponentContext()
resolver = localContext.ServiceManager.createInstanceWithContext("com.sun.star.bridge.UnoUrlResolver", localContext )
ctx = resolver.resolve( "uno:socket,host=localhost,port=2002;urp;StarOffice.ComponentContext" )

# Теперь можно модифицировать содержимое
smgr = ctx.ServiceManager
desktop = smgr.createInstanceWithContext( "com.sun.star.frame.Desktop",ctx)
'''
# Похоже что-то извлекаем
model = desktop.getCurrentComponent()
text = model.Text
cursor = text.createTextCursor()
 
# Форматируем
text.insertString( cursor, "Hello World", 0 )'''
 
# Загружаем существующий документ
fname = "/exa.java"
fileSysPath = os.getcwd()
fileSysPath = fileSysPath.replace('\\', '/')
fullSaveFileName = fileSysPath+fname
doc_java = desktop.loadComponentFromURL( uno.systemPathToFileUrl(fullSaveFileName) ,"_blank", 0, ())
 
# Сохряняем документ
''' Do a nasty thing before exiting the python process. In case the
 last call is a one-way call (e.g. see idl-spec of insertString),
 it must be forced out of the remote-bridge caches before python
 exits the process. Otherwise, the one-way call may or may not reach
 the target object.
 I do this here by calling a cheap synchronous call (getPropertyValue).'''
ctx.ServiceManager	# зачем интересно этот вызов

# Получаем текущий документ?
'''doc = desktop.getCurrentComponent()

fname = "/letter.html"
fileSysPath = os.getcwd()
fileSysPath = fileSysPath.replace('\\', '/')
fullSaveFileName = fileSysPath+fname'''


# to html
doc_java.storeToURL( uno.systemPathToFileUrl(fullSaveFileName), uinit.getHtmlProps())
# to pdf
#doc.storeToURL( uno.systemPathToFileUrl(fullSaveFileName), getPdfProps())

doc_java.dispose()

# End Main
def saveAsHtml( fname ):
	pass