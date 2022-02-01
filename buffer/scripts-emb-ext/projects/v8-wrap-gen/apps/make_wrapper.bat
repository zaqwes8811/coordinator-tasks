set PATH_TO_WRAP_TOOLS=D:\home\lugansky-igor\github\in-the-vicinity-cc\embedded_script_engines\tools
set PYTHONPATH=%PYTHONPATH%;%PATH_TO_WRAP_TOOLS%;%PATH_TO_WRAP_TOOLS%\..\..\third_party\gmock-1.6.0\scripts
python %PATH_TO_WRAP_TOOLS%\generate_v8_wrap.py %1
