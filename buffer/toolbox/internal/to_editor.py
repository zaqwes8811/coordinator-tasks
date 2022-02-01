#!/usr/bin/python
#-*- coding: utf-8 -*-
import sys
from subprocess import call
import time

import uasio.os_io.dirs_walker as dirs_walker

def init_test():
    roots = ['uno']
    listOfExtension = ['py']
    
    ignoreLists = {}
    ignoreLists[ 'Extentions' ] = [ '' ]
    ignoreLists[ 'Dirs' ] = ['.\\bin', '.\\.git', '.\\sbox', 'Analysers']
    return roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists


def Run(initializer):
    """ """
    # get settings for find
    roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists = initializer()
    
    result_argv = ''
    for pathes in roots:
        for at in listOfExtension:
            listSlice = list()
            listSlice.append(at)
         
            # поиск
            resultList, msg = dirs_walker.find_files_down_tree_(pathes, listSlice, ignoreLists)
            
            # список получен, можно его обработать
            # в принципе можно передать указатель на функцию обработки
            if resultList:
                resultList.append('#')
                result_argv += '#'.join(resultList)
        
 
    # open editor
    result_argv = result_argv.split('#')
    listToOpen = list()
    listToOpen.append("notepad++")
    for at in result_argv:
        listToOpen.append(at)
        print at
        
    call(listToOpen)
    
    
def get_list_files(initializer):
    """ """
    # get settings for find
    roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists = initializer()
    
    result_argv = ''
    for pathes in roots:
        for at in listOfExtension:
            listSlice = list()
            listSlice.append(at)
         
            # поиск
            resultList, msg = dirs_walker.find_files_down_tree_(pathes, listSlice, ignoreLists)
            
            # список получен, можно его обработать
            # в принципе можно передать указатель на функцию обработки
            resultList.append('#')
            result_argv += '#'.join(resultList)
        
 
    # open editor
    result_argv = result_argv.split('#')
    return result_argv


# Run()
if __name__=="__main__":
    ticks0 = time.time()

    Run(init_test)

    ticks1 = time.time()
    print '\nTimeRun = '+str(ticks1-ticks0)+' sec'
