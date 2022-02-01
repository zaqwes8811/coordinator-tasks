# http://stackoverflow.com/questions/5049842/autocomplete-in-pycharm-for-python-compiled-extensions

import hello

hello.say_hello("you")
hello.Foo().doSomething() # In PyCharm bad autocomplete


class Foo(object):
    def doSomething(self):
        hello.Foo().doSomething()

Foo().doSomething()