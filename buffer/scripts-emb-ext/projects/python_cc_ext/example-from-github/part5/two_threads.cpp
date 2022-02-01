#include <Python.h>
#include <iostream>
#include <boost/python.hpp>
#include <pthread.h>
#include <unistd.h>
#include "handle_error.h"
#include "python_thread_lock.h"

namespace py = boost::python;
using namespace std;

void py_run(){
    // Acquire the thread lock
    PythonThreadLock p;
    // Do the init
    py::object mm = py::import("__main__");
    py::object mn = mm.attr("__dict__");
    // Release the thread lock, and acquire it on each iteration instead
    for(int i = 0; i < 5; i++){
        py::exec("print 'Hey I am a thread!'", mn);
        sleep(1);
    }
}

// Brainless python thread function
void *do_python(void *arg){
    try{
        call_python_func<void>(py_run, "Error during thread execution: ");
    }catch(exception &e){
        // As before, any python error string will be in the thrown exception
        cout << e.what() << endl;
    }
    return NULL;
}

// Initialization steps for multithreaded embedded Python
void init_threading(){
    // Initialize the GIL
    PyEval_InitThreads();
    // InitThreads aquires the GIL, so delete the thread state and release the 
    //  lock to start from scratch
    PyThreadState* tcur = PyThreadState_Get();
    PyThreadState_Swap(NULL);
    PyThreadState_Clear(tcur);
    PyThreadState_Delete(tcur);
    PyEval_ReleaseLock();
}

int main(){
    // This is basically the simplest pthread introduction, with Python to boot
    Py_Initialize();
    init_threading();
    // Pthread stuff
    pthread_t *threads;
    pthread_attr_t pthread_custom_attr;
    threads = new pthread_t[4 * sizeof(pthread_t)];
    pthread_attr_init(&pthread_custom_attr);

    // Spawn 4 threads
    for(int i = 0; i < 4; i++){
        pthread_create(&threads[i], &pthread_custom_attr, do_python, NULL);
    }
    // Wait for the threads to finish
    for (int i = 0; i < 4; i++){
        pthread_join(threads[i], NULL);
    }
}
