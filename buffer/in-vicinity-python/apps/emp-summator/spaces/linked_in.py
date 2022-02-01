from bs4 import BeautifulSoup


class LinkedInExtractor(object):
    def __init__(self):
        self.soup = None

    def get_company_info(self, filename):
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))
        company_around = self.soup.body.find('span', attrs={'itemprop': 'name'})
        return str(company_around.prettify())

    def get_job_title_html(self, filename):
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))
        job_title_html = self.soup.body.find('h1', attrs={'class': 'title'})
        return str(job_title_html.prettify())

    def get_job_details_html(self, filename):
        """ Ready html """
        if not self.soup:
            self.soup = BeautifulSoup(open(filename))
        job_details = self.soup.body.find('div', attrs={'class': 'description-module'})

        result = str(job_details.prettify('utf-8'))
        #result = result.replace('')
        return result

