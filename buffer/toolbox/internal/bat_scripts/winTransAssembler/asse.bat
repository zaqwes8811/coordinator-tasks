@echo off
cls
set src_path=%CD%
cd ..
echo %CD%
cd ..
echo %CD%
mkdir asse
set itog='dir /s/b *%1.%2'
echo %itog%
for /F %%i in (%itog%) do  (
  echo %%i
  copy %%i %CD%\asse
  copy %%~pi%%~ni %CD%\asse
)
cd %src_path%


