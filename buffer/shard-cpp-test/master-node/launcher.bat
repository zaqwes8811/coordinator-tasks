@echo off

set PATH_TO_SHARD_FRAMEWORK=%~dp0

set EXE_FILE_NAME=%1 

if exist %EXE_FILE_NAME% (
  c:\\python26\\python.exe %PATH_TO_SHARD_FRAMEWORK%estimate_cluster_and_run_tests.py %EXE_FILE_NAME%
)