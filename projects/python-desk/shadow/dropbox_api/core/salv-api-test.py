# Include the Dropbox SDK libraries
import sys
sys.path.append('D:/Dropbox/dropbox-python-sdk-1.5.1')

from dropbox import client, rest, session
import webbrowser

'''
# Get your app key and secret from the Dropbox developer website
APP_KEY = '5pspell9t3vuqlb'
APP_SECRET = 'kc8isieyc13vdti'

# ACCESS_TYPE should be 'dropbox' or 'app_folder' as configured for your app
ACCESS_TYPE = 'app_folder'
sess = session.DropboxSession(APP_KEY, APP_SECRET, ACCESS_TYPE)
print 'Ok'

request_token = sess.obtain_request_token()
f = open('url.txt', 'w')
url = sess.build_authorize_url(request_token)
f.write(url)
f.close()

webbrowser.open_new_tab(url)
print "url:", url
print "Please visit this website and press the 'Allow' button, then hit 'Enter' here."
raw_input()

# This will fail if the user didn't visit the above URL and hit 'Allow'
access_token = sess.obtain_access_token(request_token)
'' ' 'Now that the hard part is done, all you'll need to sign your other 
API calls is to to pass the session object to DropboxClient and 
attach the object to your requests.'' '

# This will fail if the user didn't visit the above URL and hit 'Allow'
#access_token = sess.obtain_access_token(request_token)

client = client.DropboxClient(sess)
print "linked account:", client.account_info()

f = open('working-draft.txt')
response = client.put_file('/magnum-opus.txt', f)
print "uploaded:", response
'''