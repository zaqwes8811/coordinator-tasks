# coding: utf-8
"""
Fail: read it https://developers.google.com/appengine/docs/python/gettingstartedpython27/devenvironment

ide - https://developers.google.com/eclipse/docs/running_and_debugging_2_0
https://developers.google.com/appengine/articles/eclipse !!

Сервисы на gae
https://developers.google.com/api-client-library/python/guide/google_app_engine
"""

import webapp2

class MainPage(webapp2.RequestHandler):
    def get(self):
        self.response.headers['Content-Type'] = 'text/plain'
        self.response.out.write('Hello, webapp World!')

app = webapp2.WSGIApplication([('/', MainPage)],
                              debug=True)

def main():
    run_wsgi_app(application)

if __name__ == '__main__':
    main()