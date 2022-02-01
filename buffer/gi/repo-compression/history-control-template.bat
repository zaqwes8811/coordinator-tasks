http://blog.sensible.io/2012/10/09/git-to-force-put-or-not-to-force-push.html

set filename=sln/tests_service/Debug/unit-tests.exe
git log --pretty=oneline --branches -- %filename

git filter-branch --index-filter "git rm --cached ./sln/tests_service/Debug/unit-tests.exe" -- daf0e39d4f87600e5a7255d007ada718c783ea29..fd268a9ba9e274d8507f02acb1b1d14674e56404

git filter-branch -f --index-filter "git rm -q -r -f --cached --ignore-unmatch lib/xxx/$REPO" --prune-empty HEAD

git filter-branch -f --index-filter "git rm --cached --ignore-unmatch toth.txt *.exe" -- daf0e39d4^..

git filter-branch -f --index-filter "git rm --cached --ignore-unmatch *.pdb *.exe *.ilk *BuildLog.htm *.manifest *.dep *journal.txt" -- --all
git filter-branch -f --index-filter "git rm --cached --ignore-unmatch *service-engine.bin *.tmp *.pbi *.pbd" -- --all

git filter-branch -f --index-filter "git rm --cached --ignore-unmatch *TVServer.bin *.cout *pbi*" -- --all

git filter-branch -f --index-filter "git rm --cached --ignore-unmatch *.bin *.BIN *.browse *pbi* *deploy/*" -- 1e5d526eab25ff5ca0e1f977a8^..

git filter-branch -f --index-filter "touch .\some.htm" -- --all
git filter-branch -f --tree-filter "touch .\some.htm" -- 5367a12a8e^..

cp -r D:/home/lugansky-igor/Job/deploy ./deploy  - добавление всей папки

rm -Rf .git/refs/original
rm -Rf .git/logs/
git gc

git branch -r
 
git push -f origin --all


