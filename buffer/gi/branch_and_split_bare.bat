rem Ответвляет ветку от текущей, отправляет в голый репозиторий, переходит в новую
git branch %1
touch tmp.tmp

call git add .
call git commit -a -m "save"
rm tmp.tmp 

call git add .
call git commit -a -m "save"

rem Добавляем
call git checkout %1
call git push origin %1
touch tmp.tmp

call git add .
call git commit -a -m "save"
rm tmp.tmp 

call git add .
call git commit -a -m "save"
call git push

git push


