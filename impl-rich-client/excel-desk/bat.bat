rem все окружение
call bin\pathes.bat

rem оболочка
doskey pull=gi\pull
doskey push=gi\push
doskey cmt=gi\cmt $*
cmd &
