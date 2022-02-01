# coding: utf-8
__author__ = 'кей'

from collections import defaultdict


def report_gtest_result(result):
    processed_response = defaultdict(list)
    for k, v in result:
        processed_response[k].append(v)

    total_tests = 0
    total_fails = 0
    for k, v in processed_response.items():
        total_tests += len(v)
        print '\n'+k.upper()+'s', len(v)
        if k != 'ok':  # Но может быть важен вывод
            total_fails += len(v)
        for item in v:
            if k != 'ok':
                print item[0], 'is', k
                if item[1]:  # Если вывод не пустой
                    print '  With out:'
                    print '   ', item[1].replace('\r', '').replace('\n', '\n    ')
            else:
                if item[1]: 
                    print item[0], 'is', k
                    print '  With out:'
                    print '   ', item[1].replace('\r', '').replace('\n', '\n    ')

    print
    if 'fail' not in processed_response and 'deadlock' not in processed_response:
        print 'Summary: Ok', 'Runned -', total_tests
    else:
        print 'Summary: ', total_fails, 'from', total_tests, ' failed!'

