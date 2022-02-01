# coding: utf-8
import re

from inner_reuse import chunks


class Ptr(object):
    """
    Contain operation data
    """
    def __init__(self, pos, type_key):
        self.position = pos
        self.type_response = type_key
        self.name_test = None
        self.out = ""

    def __repr__(self):
        return str(dict((k, v) for k, v in self.__dict__.items() if k != 'position'))

    def get_data(self):
        """
        Returns:
            key, value
        """
        return self.type_response, (self.name_test, self.out)

    def split_test_response(self, content_value):
        """
        Returns:
            (name, out)
        """
        end_name_pos = content_value.find('\n')
        if end_name_pos != -1:
            self.name_test = content_value[:end_name_pos]
            # Почему-то если в строке выше, то влияет на все строку, а не только на срезанную
            self.name_test = self.name_test.replace('\r', '')
            self.out = content_value[end_name_pos:].strip()
        else:
            self.name_test = content_value


def parser_out(out):
    """
    Returns:
        [(name, ok/fail/deadlock, out_one), ..., ]
    """
    OK = "\[       OK \]"
    RUN = "\[ RUN      \]"
    FAILED = "\[  FAILED  \]"
    template = []
    template.extend([Ptr(a.start(), 'ok') for a in list(re.finditer(OK, out))])
    template.extend([Ptr(a.start(), 'fail') for a in list(re.finditer(FAILED, out))])
    template.extend([Ptr(a.end(), 'run') for a in list(re.finditer(RUN, out))])

    how_split = 2  # По парам
    response_pairs = chunks(sorted(template, key=lambda record: record.position), how_split)
    result = []
    for pair in response_pairs:
        head = pair[0]  # с ключем run идет вервой
        if head.type_response == 'run':
            if len(pair) == 1:
                pair.append(Ptr(out.__len__(), 'deadlock'))
            bottom = pair[1]

            content = out[head.position:bottom.position].strip()
            bottom.split_test_response(content)
            yield bottom.get_data()


def parse_out_not_gen(out):
    gen = parser_out(out)
    report = []
    for i in gen:
        report.append(i)
    return report
