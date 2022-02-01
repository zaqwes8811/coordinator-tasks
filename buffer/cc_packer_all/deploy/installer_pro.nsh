# http://www.seas.gwu.edu/~drum/java/lectures/appendix/installer/install.html
# define installer name

# OS detection 
# http://stackoverflow.com/questions/14409801/nsis-detect-windows-version

# Summary!!
# http://nsis.sourceforge.net/Docs/Chapter4.html

!include "MUI2.nsh"
!include WinVer.nsh
!include wver.nsh

OutFile "installer.exe"
 
# set desktop as install directory
InstallDir $PROGRAMFILES32\App

; The default installation directory
#InstallDir $PROGRAMFILES\Example1

# Deps
Var QtBinPath ;Declare the variable
Var OpenSSLBinPath ;
Var ExeFileDir ;
var osVersionVar

!define MUI_LANGDLL_ALLLANGUAGES
!insertmacro MUI_LANGUAGE "Russian"
!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_RESERVEFILE_LANGDLL

Function .onInit
	; Target: 32 on win xp/7/8 32/64 ru/en
	SetRegView 32

	!insertmacro MUI_LANGDLL_DISPLAY ;This has to come after the language macros
FunctionEnd

; The text to prompt the user to enter a directory
DirText "This will install My Cool Program on your computer. Choose a directory"

# http://www.rhyous.com/2010/06/10/detecting-if-the-install-is-occuring-on-a-64-bit-or-32-bit-machine/

# default section start
Section
	; http://www.rhyous.com/2010/06/10/detecting-if-the-install-is-occuring-on-a-64-bit-or-32-bit-machine/
	; Install to the correct directory on 32 bit or 64 bit machines
	; http://nsis.sourceforge.net/Reference/SetRegView
	# http://stackoverflow.com/questions/14089647/install-to-program-files-x86-on-64bit-host
	IfFileExists $WINDIR\SYSWOW64\*.* Is64bit Is32bit
	Is32bit:
		MessageBox MB_OK "32 bit"
		SetRegView 32
		#StrCpy $INSTDIR "$PROGRAMFILES32\LANDesk\Health Check"
		GOTO End32Bitvs64BitCheck
	Is64bit:
		MessageBox MB_OK "64 bit"
		#SetRegView 64 ; NO!
		#StrCpy $INSTDIR "$PROGRAMFILES64\LANDesk\Health Check"
	End32Bitvs64BitCheck:
	
	${GetWindowsVersion} $osVersionVar
	${If} $osVersionVar == "XP"
		CreateDirectory "$SMPROGRAMS\Rizwan Inc."
		CreateShortCut "$SMPROGRAMS\Rizwan Inc.\Open Notepad.lnk" "Notepad.exe" 
	${Else}
		# 7 >=
    ${EndIf}
	
	# Cross work
	CreateDirectory $INSTDIR
	 
	# define output path
	SetOutPath $INSTDIR
	 
	# specify file to go in output path
	File "c:\\packer_release.exe"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\Qt5Core.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\Qt5Widgets.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\Qt5Gui.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\libgcc_s_dw2-1.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\libwinpthread-1.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\libstdc++-6.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\icudt53.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\icuin53.dll"
	File "C:\\Qt\\5.4\\mingw491_32\\bin\\icuuc53.dll"
	
	File "/oname=tar.exe" "tar.exe__" 
		 
	# define uninstaller name
	WriteUninstaller $INSTDIR\uninstaller.exe

	# FIXME: make lnk
SectionEnd
 
# create a section to define what the uninstaller does.
# the section will always be named "Uninstall"
Section "Uninstall"
	${un.GetWindowsVersion} $osVersionVar
	${If} $osVersionVar == "XP"
		#Delete "$SMPROGRAMS\Rizwan Inc.\Run Rizwan's Program.lnk"
		Delete "$SMPROGRAMS\Rizwan Inc.\Open Notepad.lnk"
		RMDIR "$SMPROGRAMS\Rizwan Inc."
	${Else}
		# 7 >=
    ${EndIf}
	 
	# Always delete uninstaller first
	Delete $INSTDIR\uninstaller.exe
	 
	# now delete installed file
	Delete $INSTDIR\packer_release.exe
	Delete $INSTDIR\tar.exe
	Delete $INSTDIR\*.dll

	# App folder
	RMDir $INSTDIR
SectionEnd