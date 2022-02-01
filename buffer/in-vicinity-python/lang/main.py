__author__ = 'SYSTEM'


class Iterable(list):
    def __iter__(self):
        return IteratorIterable(self)


class IteratorIterable(object):
    """ A sample implementation of a list iterator. NOTE: This is just a
    demonstration of concept!!! YOU SHOULD NEVER IMPLEMENT SOMETHING LIKE THIS!
    Even if you have to (for any reason), there are many better ways to
    implement this."""

    def __init__(self, lst):
        self.state_ = lst
        self.idx = -1

    def __iter__(self):
        return self

    def next(self):
        if self.idx < len(self.state_) - 1:
            self.idx += 1
            return self.idx, self.state_[self.idx]
        else:
            raise StopIteration

# Class contained list


if __name__ == '__main__':
    a = Iterable([1, 2, 3, 4])
    ia = iter(a)
    #print 'type(a): %r, type(ia): %r' % (type(a), type(ia))
    #for i in a:
    #    print i,

    for i, v in a:
        print i, v