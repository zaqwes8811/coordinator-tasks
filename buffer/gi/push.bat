rem оболочка
call cmt "save and push"

rem Отправить накопленные коммиты в центральный репозиторий
git push %1 %2
