
def get_test_name(line):
    return line.split('\\r\\n')[0].replace(' ', '').replace(']', '').rstrip().lstrip()

response = open(file_name).readlines()
response = '\\r\\n'.join(response).replace('\r', '').replace('\n', '')
data_of_tests = filter(lambda line: 'OK' in line or 'FAILED' in line, response.split('RUN'))

result = map(get_test_name, data_of_tests)


class Functor(object):
    def __init__(self):
        self.result_ = {}

    def __call__(self, elem):
        self.result_[elem] = None

#result = reduce(lambda y, x: [x] = None, result, {})

#tmp = []
#result = map(lambda line: tmp.extend(line.split('RUN')), result)
#result = filter(lambda line: 'OK' in line, tmp)

summary = Functor()
map(summary, result)

print summary.result_


# BAD! No reliability! Hack!
class FunctorReduce(object):
    def __init__(self):
        self.result_ = {}

    def __call__(self, x, elem):
        self.result_[elem] = None
        return self

    def get_result(self):
        return self.result_

print reduce(FunctorReduce(), result, "").get_result()
