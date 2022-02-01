rem To create a new branch (locally) called branchname
git branch %1

rem Then to sync it with the remote repository like github (if applicable)
git push origin %1

rem And to use it for development / make the branch the active branch
git checkout %1

