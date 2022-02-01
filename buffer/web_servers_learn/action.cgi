#!/usr/bin/python
# coding: utf-8

import cgitb
cgitb.enable()

import os
import sys

from urlparse import urlparse, parse_qs

print "Content-type: text/html"
print

def process():
    try:
        qstring = parse_qs(os.environ['QUERY_STRING'])
        access_method = os.environ['REQUEST_METHOD']
        
        result = []
        
        if access_method != 'GET':
          result = str({'error': 1, 'message': "Данный тип запроса не поддерживается." })
        
        # TODO: почему это список то?
        action_name = qstring['action'][0]
    
    
        if action_name == 'down_power':
            result = str({'error': 0, 'massage': ""})
          
        elif action_name == 'up_power':
            result = str({'error': 0, 'massage': ""})

        else:
            result = str({'error': 1, 'message': "Комманда не известна."})
        
    # Не подклю к каналу
    #except IOException as e:
    #
    except Exception as e:
        result = str({e})
      
    return result
    
    
if __name__=='__main__':
  response = process()
  print response

