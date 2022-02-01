set PROJECT_ROOT=..

set FILE_FILTER=file-filter.txt
set SNODE=.\\dist\\shard-node
mkdir %SNODE%
xcopy /r /d /i /s /y /exclude:%FILE_FILTER% "%PROJECT_ROOT%\\shard-node" "%SNODE%" 
xcopy /r /d /i /s /y /exclude:%FILE_FILTER% "%PROJECT_ROOT%\\lib" "%SNODE%\\libs" 

set MNODE=.\\dist\\master-node
mkdir %MNODE%
xcopy /r /d /i /s /y /exclude:%FILE_FILTER% "%PROJECT_ROOT%\\master-node" "%MNODE%" 
xcopy /r /d /i /s /y /exclude:%FILE_FILTER% "%PROJECT_ROOT%\\lib" "%MNODE%\\libs" 