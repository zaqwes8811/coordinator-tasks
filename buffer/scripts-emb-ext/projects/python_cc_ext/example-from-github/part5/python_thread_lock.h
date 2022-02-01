#ifndef PYTHON_THREAD_LOCK_H
#define PYTHON_THREAD_LOCK_H

#include <Python.h>

class PythonThreadLock{
    private:
        PyGILState_STATE gstate_;
    public:
        PythonThreadLock(){
            gstate_ = PyGILState_Ensure();
        }
        ~PythonThreadLock(){
            PyGILState_Release(gstate_);
        }
};

#endif
