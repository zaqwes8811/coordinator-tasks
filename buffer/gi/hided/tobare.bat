set SRC_PRJ_NAME=%1
rem input
cd %SRC_PRJ_NAME%

rem init
git init

rem commit
git add .
git commit -a -m "auto"
cd ..

rem ����� ����������� .git
xcopy /s/e %SRC_PRJ_NAME%\.git\** %SRC_PRJ_NAME%.git\

rem ������� ����� �����������
cd %SRC_PRJ_NAME%.git
git config --bool core.bare true
rem git config --bool core.bare true
cd ..

rem ������� �������� ������
move %SRC_PRJ_NAME% Trash



