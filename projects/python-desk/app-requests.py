# coding: utf-8
'''
Created on 06.04.2013

@author: кей
'''
import requests
import json

# GData API
from xml.etree import ElementTree
import gdata.docs.service
try:
  from xml.etree import ElementTree
except ImportError:
  from elementtree import ElementTree
import gdata.calendar.data
import gdata.calendar.client
import gdata.acl.data
import atom.data
import time

def get_headers_gdocs_files():
    # Create a client class which will make HTTP requests with Google Docs server.
    client = gdata.docs.service.DocsService()
    # Authenticate using your Google Docs email address and password.
    client.ClientLogin('igor.a.lugansky@gmail.com', 'a3fs4gd5h')
    
    # Query the server for an Atom feed containing a list of your documents.
    documents_feed = client.GetDocumentListFeed()
    # Loop through the feed and extract each document entry.
    for document_entry in documents_feed.entry:
          # Display the title of the document on the command line.
          print document_entry.title.text


def get_todos_from_gcalendar():
    client = gdata.calendar.client.CalendarClient()
    client.ClientLogin('igor.a.lugansky@gmail.com', 'a3fs4gd5h', client.source)
    
    def PrintUserCalendars(calendar_client):
        """ Список календарей """
        feed = calendar_client.GetAllCalendarsFeed()
        print feed.title.text
        for i, a_calendar in enumerate(feed.entry):
            print '\t%s. %s' % (i, a_calendar.title.text,)
        
    PrintUserCalendars(client)    
    
get_todos_from_gcalendar()

def get_list_issues_from_ghub():
    r = requests.get('https://api.github.com', auth=('zaqwes8811', 'a3fs4gd5h'))
    #print r.status_code
    #print json.dumps(r.json(), sort_keys=True, indent=2)
    
    #r = requests.get('https://api.github.com/repos/zaqwes8811/coupler-account-repos')
    #print r.status_code
    #print json.dumps(r.json(), sort_keys=True, indent=2)
    
    r = requests.get('https://api.github.com/repos/zaqwes8811/processor-word-frequency-index/issues?state=closed')
    print r.status_code
    #print json.dumps(r.json(), sort_keys=True, indent=2)
    
    # Список проблем
    list_issues = r.json()
    for at in list_issues:
        print at['number'], at['title'], at['state']
    #print len(list_issues)

