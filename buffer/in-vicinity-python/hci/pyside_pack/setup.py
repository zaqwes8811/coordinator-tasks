# http://www.qtcentre.org/archive/index.php/t-53822.html?s=c9eb763da946ce5e6fdbe66fd4d805c4
# "setup.py py2exe --includes sip"

# https://freepythontips.wordpress.com/2014/03/03/using-py2exe-the-right-way/

# http://stackoverflow.com/questions/14815173/py2exe-cant-find-msvcp90-dll

from distutils.core import setup
import py2exe

# http://www.py2exe.org/index.cgi/Tutorial
# http://stackoverflow.com/questions/25211893/py2exe-msvcr90-dll-runtime-error

# http://stackoverflow.com/questions/17509088/system-path-error-with-pyqt-and-py2exe

crt = 'C:\Windows\winsxs\x86_microsoft.vc90.crt_1fc8b3b9a1e18e3b_9.0.30729.6161_none_50934f2ebcb7eb57'

from glob import glob

#data_files = [("Microsoft.VC90.CRT", glob(r'C:\Windows\winsxs\x86_microsoft\.vc90\.crt_1fc8b3b9a1e18e3b_9\.0\.30729\.6161_none_50934f2ebcb7eb57'))]


setup(options={'py2exe':{'bundle_files':1}},
	#data_files=data_files,
	zipfile = None,
	windows=['main.pyw'])