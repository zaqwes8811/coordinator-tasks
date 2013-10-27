# coding: utf-8
# https://developers.google.com/console/help/#WhatIsKey
# https://developers.google.com/api-client-library/python/guide/aaa_oauth
__author__ = 'кей'

import random

# Create a state token to prevent request forgery.
# Store it in the session for later validation.
state = ''.join(random.choice(string.ascii_uppercase + string.digits) for x in xrange(32))
session['state'] = state
# Set the Client ID, Token State, and Application Name in the HTML while
# serving it.
response = make_response(
  render_template('index.html',
                  CLIENT_ID=CLIENT_ID,
                  STATE=state,
                  APPLICATION_NAME=APPLICATION_NAME))
