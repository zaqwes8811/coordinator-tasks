@echo off

rem Where placed launcher
set PATH_TO_LAUNCHER_FOLDER=%~dp0

rem Need set python path. Deployed if first.
set PYTHONPATH=%PYTHONPATH%;%PATH_TO_LAUNCHER_FOLDER%common;%PATH_TO_LAUNCHER_FOLDER%third_party^
  ;%PATH_TO_LAUNCHER_FOLDER%..\third_party;%PATH_TO_LAUNCHER_FOLDER%..\common

rem If file exist run it.
set EXE_FILE_NAME=%1 
echo Run bin file: %EXE_FILE_NAME%
if exist %EXE_FILE_NAME% (
   c:\\python26\\python.exe %PATH_TO_LAUNCHER_FOLDER%mdl-launcher.py %EXE_FILE_NAME% %2
) else (
  echo "No found" %EXE_FILE_NAME%
)


