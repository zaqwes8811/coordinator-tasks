# coding: utf-8
"""
  Проблемы с python2.6 - VerifiedHTTPSConnection instance has no attribute '_tunnel_host'
"""
__author__ = 'кей'

import requests


class Accessor(object):
    def __init__(self, username, password):
        self.username_ = username
        self.password_ = password

    def make_get(self, url):
        r = requests.get(url, auth=(self.username_, self.password_))
        if r.status_code == 200:
            loaded_response = r.json()


if __name__ == '__main__':
    username = 'zaqwes'
    password = 'a3fs4gd5h'
    repo_slug = "controller-transmitters-bugtracker-read-only"
    pathToRepo = 'https://bitbucket.org/api/1.0/repositories/gangsofmart/'+repo_slug+'/issues'

    accessor = Accessor(username, password)
    accessor.make_get(pathToRepo)
