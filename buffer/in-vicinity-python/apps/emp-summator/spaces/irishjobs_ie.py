
# html parsers
# http://stackoverflow.com/questions/11709079/parsing-html-python

from bs4 import BeautifulSoup


class IrishJobsIEExtractor(object):
    def __init__(self):
        self.soup = None

    def get_company_info(self, filename):
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))
        company_around = self.soup.body.find('div', attrs={'class':'border-wrap'})
        return str(company_around.prettify())

    def get_job_title_html(self, filename):
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))
        job_title_html = self.soup.body.find('h1')
        return str(job_title_html.prettify())

    def get_job_details_html(self, filename):
        """ Ready html """
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))

        job_details = self.soup.body.find('div', attrs={'class':'job-details'})
        return str(job_details.prettify(self.soup.original_encoding))
