#!/usr/bin/env python
from distutils.core import setup
from distutils.extension import Extension

setup( name="PackageName",
    ext_modules=[
        Extension("hello_", ["foo.cc"],
        library_dirs = ['D:\\home\\third_party\\cc\\boost_1_47_0\\libs\\msvc-9.0\\lib'],
            libraries = ["boost_python-vc90-mt-1_47","libboost_system-vc90-mt-1_47"],
            include_dirs = [
                "D:/home/third_party/cc/boost_1_47_0",
                "d:/Python27/include",
                "D:/Program Files/Microsoft SDKs/Windows/v6.1/Include"],
        )
    ])