#!/bin/bash
# Ответвляет ветку от текущей переходит в новую
# Новая ветка локальна!
git branch $1
touch f.fake

git add -A .
git commit -a -m "fake"
rm f.fake

git add -A .
git commit -a -m "fake"

# Добавляем
git checkout $1
touch f.fake

git add -A .
git commit -a -m "fake"
rm f.fake

git add -A.
git commit -a -m "fake"


