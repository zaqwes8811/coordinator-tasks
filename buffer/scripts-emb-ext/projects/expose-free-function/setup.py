#-*- coding: utf-8 -*-

'''
    TODO : узнать как выбирать компиятор и его ключи(директивы)
    
    TODO : подключить чисто Си компилятор
    
    python setup.py install => последний аргумент как-то используется системой сборки
    
    Issues : 
        1. Не принимает *.c файлы
'''

import sys
import shutil
from distutils.core import setup
from distutils.extension import Extension

boost_root = '/opt/big-3rdparty/boost_1_57_0'
kLibraryDirs = [boost_root + '/stage_gcc_x64']
kLibsNames = ['boost_python']
pkg_name = 'PackageName'
srcs = ['hello_module.cc', 'hello_inc.cc']
include_dirs = [boost_root]

# deploy
target_pkg_dir = './'
module_name = 'hello'
path_to_module = 'build/lib.linux-x86_64-2.7/'

def main(module_name, srcs, include_dirs, fp_init):   
    one_extention = Extension(
        module_name, 
        srcs,
        include_dirs=include_dirs,
        libraries=kLibsNames,
        library_dirs=kLibraryDirs)
        
    setup(name=pkg_name, ext_modules=[one_extention])

    # Move result modules
    shutil.copy(path_to_module+module_name+'.so', target_pkg_dir + module_name+'.so')

if __name__=='__main__':
    sys.argv = ['setup.py', 'build']
    main(module_name, srcs, include_dirs, None)
    
