#-*- coding : utf-8 -*-

''' 

Notes:
	soffice -accept="socket,host=localhost,port=2002;urp;
	еще можно при запущенном квикстартере работать


Autor : cp from uno examples 
	
Question:
	Позиционирование курсора?
	Настройки прочих элементов существующих файлов
	
Jython Support:
	None, поэтому лучше вытаскивать текст через Tika, но с русским там проблеммы
	
'''

import os
import uinit
import uno

def odt_to_pdf(ifile, ofile):
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
	resolver = localContext.ServiceManager.createInstanceWithContext("com.sun.star.bridge.UnoUrlResolver", localContext)
	ctx = resolver.resolve("uno:socket,host=localhost,port=2002;urp;StarOffice.ComponentContext")
	
	# Теперь можно модифицировать содержимое
	smgr = ctx.ServiceManager
	desktop = smgr.createInstanceWithContext("com.sun.star.frame.Desktop",ctx)
	
	 
	# Загружаем существующий документ
	doc_java = desktop.loadComponentFromURL(uno.systemPathToFileUrl(ifile) ,"_blank", 0, ())
	 
	# Сохряняем документ
	ctx.ServiceManager	# зачем интересно этот вызов
	
	# Получаем текущий документ?
	doc = desktop.getCurrentComponent()
	
	
	# to pdf
	doc.storeToURL(uno.systemPathToFileUrl(ofile), uinit.getPdfProps())
	
	doc_java.dispose()
	
	return 0, ''

# End Main
def saveAsHtml(fname):
	pass

if __name__=='__main__':
	ifile = "/exa.java"  # utf-8 превращает в крокозяблы - похоже для преобразования используется текущая локаль
	ifile = "/t.odt"
	ifile = "/t.doc"
	ofile = "/letter.pdf"
	file_sys_path = os.getcwd()
	file_sys_path = file_sys_path.replace('\\', '/')
	full_save_fname = file_sys_path+ifile
	file_sys_path = os.getcwd()
	file_sys_path = file_sys_path.replace('\\', '/')
	full_save_fname = file_sys_path+ofile
	err_code, err_msg = odt_to_pdf(full_save_fname, full_save_fname)