__author__ = 'zaqwes'

from bs4 import BeautifulSoup


class HhruExtractor(object):
    def __init__(self):
        self.soup = None

    def get_company_info(self, filename):
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))
        company_around = self.soup.body.find('div', attrs={'class':'companyname'})

        if not company_around:
            company_around = self.soup.body.find('div', attrs={'class':'hht-vacancy-company'})

        return str(company_around.prettify(encoding=self.soup.original_encoding))

    def get_job_title_html(self, filename):
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))
        job_title_html = self.soup.body.find('h1', attrs={'class' : 'b-vacancy-title'})
        return str(job_title_html.prettify(encoding=self.soup.original_encoding))

    def get_job_details_html(self, filename):
        """ Ready html """
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))

        job_details = self.soup.body.find('div', attrs={'class':'b-vacancy-desc'})
        return str(job_details.prettify(self.soup.original_encoding))

