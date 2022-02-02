# -*- coding: utf-8 -*-
hello = "This is a rather long string containing\n\
several lines of text just as you would do in C.\n\
Note that whitespace at the beginning of the line is\n\
significant.\n"
print hello

word = 'He' + 'lp'
print word

wwword = word*5
print wwword

print word[:-2]
print word[-2:]

str = 'qwertyuiopasdfghhjkldfgjhbytouy'
print len(str)

a = ['spam', 'eggs', 100, 1234]
print a

lst = [ 100, 12345, 'ho-ho', 'alala']
print lst

print lst[2:] + ['trololo']

lst=4*lst[2:] + ['trololo']
print lst

a[2]=a[2]+150
print a

print len(lst)


a[0:2] = [1, 12]
print a

a[0:2] = []
print a

a[1:1] = ['bletch', 'xyzzy']
print a

a[:0] = a     
print a

a = ['Mary', 'had', 'a', 'little', 'lamb']
for i in range(len(a)):
print i, a[i]

