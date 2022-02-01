#-*- coding: utf-8 -*-

# sys
import sys

# Other
import uasio.os_io.dirs_walker as dirs_walker

def init_test():
    roots = ['uno']
    listOfExtension = ['py']
    
    ignoreLists = {}
    ignoreLists[ 'Extentions' ] = [ '' ]
    ignoreLists[ 'Dirs' ] = ['.\\bin', '.\\.git', '.\\sbox', 'Analysers']
    return roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists


def get_list_files(initializer):
    """ """
    # get settings for find
    roots, listOfExtension, ignoreLists, ignoreLists, ignoreLists = initializer()
    
    resultArgv = ''
    for pathes in roots:
        for at in listOfExtension:
            listSlice = list()
            listSlice.append(at)
         
            # поиск
            resultList, msg = dirs_walker.find_files_down_tree_(pathes, listSlice, ignoreLists)
            
            # список получен, можно его обработать
            # в принципе можно передать указатель на функцию обработки
            resultList.append('#')
            resultArgv += '#'.join(resultList)
        
 
    # open editor
    resultArgv = resultArgv.split('#')
    return resultArgv