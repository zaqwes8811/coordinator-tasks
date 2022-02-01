#-*- coding : utf-8 -*-
# openoffice -accept="socket,host=localhost,port=2002;urp;"
# soffice -accept="socket,host=localhost,port=2002;urp;"
''' autor : not i, i - modifer '''
import os
import sys

def import_uno():
    # Add the URE_BOOTSTRAP environment variable       #3
    os.environ['URE_BOOTSTRAP'] = 'vnd.sun.star.pathname:c:\Program Files\OpenOffice.org 3\program\\fundamental.ini'

    # Add the UNO_PATH environment variable               #4
    os.environ['UNO_PATH'] = 'c:\Program Files\OpenOffice.org 3\program\\'

    # Add the PATH environment variable, but weed the duplicates first       #5
    new_paths_string = 'c:\Program Files\OpenOffice.org 3\\URE\\bin;c:\Program Files\OpenOffice.org 3\Basis\program;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\\Wbem;C:\Program Files\Common Files\Intuit\QBPOSSDKRuntime'
    new_paths = new_paths_string.split(';')
    existing_paths = os.environ['PATH'].split(';')
    for path in new_paths:
        if path not in existing_paths:
            existing_paths.append(path)
    os.environ['PATH'] = ';'.join(existing_paths)

    # Add the uno location to PYTHONPATH         #6
    sys.path.append('C:\\Program Files\\OpenOffice.org 3\\Basis\\program')

    return

# Begin Main
import_uno()
import uno
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
resolver = localContext.ServiceManager.createInstanceWithContext(
	"com.sun.star.bridge.UnoUrlResolver", localContext )
ctx = resolver.resolve( "uno:socket,host=localhost,port=2002;urp;StarOffice.ComponentContext" )
smgr = ctx.ServiceManager
desktop = smgr.createInstanceWithContext( "com.sun.star.frame.Desktop",ctx)
model = desktop.getCurrentComponent()
text = model.Text
cursor = text.createTextCursor()
 
text.insertString( cursor, "Hello World", 0 )
 
''' Do a nasty thing before exiting the python process. In case the
 last call is a one-way call (e.g. see idl-spec of insertString),
 it must be forced out of the remote-bridge caches before python
 exits the process. Otherwise, the one-way call may or may not reach
 the target object.
 I do this here by calling a cheap synchronous call (getPropertyValue).'''
ctx.ServiceManager

document = desktop.getCurrentComponent()
fname = "/letter2.html"
fileSysPath = os.getcwd()
fileSysPath = fileSysPath.replace('\\', '/')
fullSaveFileName = "file:///"+fileSysPath+fname
document.storeAsURL( fullSaveFileName,())
# End Main