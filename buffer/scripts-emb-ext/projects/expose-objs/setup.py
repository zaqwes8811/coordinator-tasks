#!/usr/bin/env python

import shutil

from distutils.core import setup
from distutils.extension import Extension

boost_root = '/opt/big-3rdparty/boost_1_57_0'

module_name = 'hello'

setup( name="PackageName",
    ext_modules=[
        Extension(module_name, ["foo.cc"],
        library_dirs = [boost_root + '/stage_gcc_x64'],
            libraries = ["boost_python"],
            include_dirs = [boost_root],
        )
    ])

target_pkg_dir = './'
path_to_module = 'build/lib.linux-x86_64-2.7/'

shutil.copy(path_to_module+module_name+'.so'
	, target_pkg_dir + module_name+'.so')