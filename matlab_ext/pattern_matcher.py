#!/usr/bin/env python
# coding: utf-8

import re

if __name__ == '__main__':
    s1 = 'aaabbbaa'
    m = re.search(r'^a', s1)
    print m.groups()
    
    # https://habrahabr.ru/post/115825/
    print re.findall("^a", s1)
    print re.findall("^a*b+a", s1)
    print re.findall("^a*b*a", 'aaaaaa')
    print re.findall("^a*b*a", 'aaaaaa')
    
    # сверхжадные не заработали сходу
    # http://easy-code.ru/lesson/java-quantifiers-regular-expressions
    p = re.compile(r"^a{1,10}?b{2,4}?a", re.DEBUG)
    print p.findall('aaabbaaabbba')
    
    # DFA, NFA, e-NFA - engines
    # https://msdn.microsoft.com/ru-ru/library/e347654k(v=vs.110).aspx
    #http://www.intuit.ru/studies/courses/1148/187/lecture/4888
    # !! https://learn.javascript.ru/regular-expressions-javascript
    # NP - упоминается, а значит проблемы с произв.