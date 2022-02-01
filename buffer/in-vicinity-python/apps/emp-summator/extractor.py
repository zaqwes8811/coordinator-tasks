# coding: utf-8

# html parsers
# http://stackoverflow.com/questions/11709079/parsing-html-python

# TODO: сделать arial шрифт - таймс тяжело воспринимается

import glob
import os
from collections import namedtuple

import spaces.linked_in
import spaces.irishjobs_ie
import spaces.hh_ru


def builder_in():
    return spaces.linked_in.LinkedInExtractor()


def builder_ir():
    return spaces.irishjobs_ie.IrishJobsIEExtractor()

def builder_hh():
    return spaces.hh_ru.HhruExtractor()

if __name__ == '__main__':
    Pair = namedtuple('Pair', 'path builder')

    processing_map = [
        #Pair('../Dropbox/job_statistic/linkedin/cc', builder_in),
        #Pair('../Dropbox/job_statistic/irishjobs.ie/cc', builder_ir),
        Pair('../../Dropbox/job_statistic/hh.ru/cc', builder_hh)]

    result_filename = 'report.html'
    # Erase file
    f = open(result_filename, 'w')
    f.close()
    f = open(result_filename, 'a')

    for elem in processing_map:
        files = []
        print elem.path+"/*.html"
        for filename in glob.glob(elem.path+"/*.html"):
            files.append(filename)

        f.write("<html>")

        # TODO: пока кодировка utf-8, но наверное лучше так и оставить
        f.write("""
        <head>
        <meta charset="UTF-8">
        <link rel="stylesheet" type="text/css" href="style.css">
        </head>
        """)
        f.write("<body>")
        f.write("<div style=\"width:600px\">")
        for filename in files:
            print 'Processing file:', filename
            extractor = elem.builder()
            f.write(extractor.get_company_info(filename))
            f.write(extractor.get_job_title_html(filename))
            f.write(extractor.get_job_details_html(filename))
        f.write("</div>")
        f.write("</body>")
        f.write("</html>")

    f.close()  # you can omit in most cases as the destructor will call if
