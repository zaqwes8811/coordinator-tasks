#-*- coding : utf-8 -*-
# file : import uinit
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

''' функции-конветроры '''

def getPdfProps():
	props = []
	prop = PropertyValue()
	prop.Name = "FilterName"
	prop.Value = "writer_pdf_Export"
	props.append(prop)
	return tuple(props)
	
def getHtmlProps():
	props = []	# это передается
	
	# набираем порции
	prop = PropertyValue()
	prop.Name = "FilterName"
	prop.Value = "HTML (StarWriter)"
	
	# добавляем
	props.append(prop)
	
	# набираем порции
	prop = PropertyValue()
	prop.Name = "FilterName"
	prop.Value = "writer_html_Export"
	
	# добавляем
	props.append(prop)
	return tuple(props)
	
import_uno()
import uno
from com.sun.star.beans import PropertyValue