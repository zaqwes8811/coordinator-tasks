# coding: utf-8
# https://developers.google.com/console/help/#WhatIsKey
# https://developers.google.com/api-client-library/python/guide/aaa_oauth
# http://stackoverflow.com/questions/10271110/python-oauth2-login-with-google
# !http://stackoverflow.com/questions/14286402/google-drive-python-api-resumable-upload-error-401-after-2-hours/14320908#14320908
__author__ = 'кей'

import httplib2
import json
import time

from apiclient.discovery import build
from apiclient.http import MediaFileUpload
from apiclient import errors
from oauth2client.client import OAuth2WebServerFlow


def build_gdrive_bridge():
    """ Похоже можно ужать вызов и удалить ключи из кода
    https://developers.google.com/api-client-library/python/guide/aaa_client_secrets
    """
    # Copy your credentials from the APIs Console
    CLIENT_ID = '105447427029.apps.googleusercontent.com'
    CLIENT_SECRET = 'azmcKEseppjZvuu-JQKa42rA'
    # Redirect URI for installed apps
    REDIRECT_URI = 'urn:ietf:wg:oauth:2.0:oob'

    # Scopes
    # Check https://developers.google.com/drive/scopes for all available scopes
    OAUTH_SCOPE = 'https://www.googleapis.com/auth/drive'

    # Run through the OAuth flow and retrieve credentials
    flow = OAuth2WebServerFlow(CLIENT_ID, CLIENT_SECRET, OAUTH_SCOPE, REDIRECT_URI)

    # Нужно открыть в браузере!
    # Далее
    authorize_url = flow.step1_get_authorize_url()
    print 'Go to the following link in your browser: ' + authorize_url

    # Ввод кода авторизации
    code = raw_input('Enter verification code: ').strip()
    credentials = flow.step2_exchange(code)

    # Create an httplib2.Http object and authorize it with our credentials
    http = httplib2.Http()
    http = credentials.authorize(http)
    http = credentials.authorize(http)

    drive_service = build('drive', 'v2', http=http)
    return drive_service


def retrieve_all_files(service):
    """Retrieve a list of File resources.

  Args:
    service: Drive API service instance.
  Returns:
    List of File resources.
  """
    result = []
    page_token = None
    while True:
        try:
            param = {}
            if page_token:
                param['pageToken'] = page_token
            files = service.files().list(**param).execute()

            result.extend(files['items'])
            for at in files['items']:
                print at['title']
            page_token = files.get('nextPageToken')
            if not page_token:
                break
        except errors.HttpError, error:
            print 'An error occurred: %s' % error
            break
    return result


if __name__ == '__main__':
    # Как понял можно работать с файлами!
    drive_service = build_gdrive_bridge()
    retrieve_all_files(drive_service)


def upload_file():
    # Insert a file
    media_body = MediaFileUpload('bigfile.zip', mimetype='application/octet-stream', chunksize=1024 * 256,
                                 resumable=True)
    body = {
        'title': 'bigfile.zip',
        'description': 'Big File',
        'mimeType': 'application/octet-stream'
    }

    retries = 0
    request = drive_service.files().insert(body=body, media_body=media_body)
    response = None
    while response is None:
        try:
            print http.request.credentials.access_token
            status, response = request.next_chunk()
            if status:
                print "Uploaded %.2f%%" % (status.progress() * 100)
                retries = 0
        except errors.HttpError, e:
            if e.resp.status == 404:
                print "Error 404! Aborting."
                exit()
            else:
                if retries > 10:
                    print "Retries limit exceeded! Aborting."
                    exit()
                else:
                    retries += 1
                    time.sleep(2 ** retries)
                    print "Error (%d)... retrying." % e.resp.status
                    continue
    print "Upload Complete!"
