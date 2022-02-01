rem Ответвляет ветку от текущей переходит в новую
rem Новая ветка локальна!
git branch %1
touch tmp.tmp

call git add .
call git commit -a -m "save"
rm tmp.tmp 

call git add .
call git commit -a -m "save"

rem Добавляем
call git checkout %1
touch tmp.tmp

call git add .
call git commit -a -m "save"
rm tmp.tmp 

call git add .
call git commit -a -m "save"


