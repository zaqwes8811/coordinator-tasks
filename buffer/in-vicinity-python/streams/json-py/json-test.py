#-*- coding: utf-8 -*-
import json
#import com.xhaus.jyson.JysonCodec as json
text = "[ \
	{\
    \"url\": \"https://api.github.com/repos/octocat/Hello-World/commits/6dcb09b5b57875f334f61aebed695e2e4193db5e\",\
    \"sha\": \"6dcb09b5b57875f334f61aebed695e2e4193db5e\",\
    \"commit\": {\
      \"url\": \"https://api.github.com/repos/octocat/Hello-World/git/commits/6dcb09b5b57875f334f61aebed695e2e4193db5e\",\
      \"sha\": \"6dcb09b5b57875f334f61aebed695e2e4193db5e\",\
      \"author\": {\
        \"name\": \"Monalisa Octocat\",\
        \"email\": \"support@github.com\",\
        \"date\": \"2011-04-14T16:00:49Z\"\
      },\
      \"committer\": {\
        \"name\": \"Monalisa Octocat\",\
        \"email\": \"support@github.com\",\
        \"date\": \"2011-04-14T16:00:49Z\"\
      },\
      \"message\": \"Fix all the bugs\",\
      \"tree\": {\
        \"url\": \"https://api.github.com/repos/octocat/Hello-World/tree/6dcb09b5b57875f334f61aebed695e2e4193db5e\",\
        \"sha\": \"6dcb09b5b57875f334f61aebed695e2e4193db5e\"\
      }\
    },\
    \"author\": {\
      \"login\": \"octocat\",\
      \"id\": 1,\
      \"avatar_url\": \"https://github.com/images/error/octocat_happy.gif\",\
      \"gravatar_id\": \"somehexcode\",\
      \"url\": \"https://api.github.com/users/octocat\"\
    },\
    \"committer\": {\
      \"login\": \"octocat\",\
      \"id\": 1,\
      \"avatar_url\": \"https://github.com/images/error/octocat_happy.gif\",\
      \"gravatar_id\": \"somehexcode\",\
      \"url\": \"https://api.github.com/users/octocat\"\
    },\
    \"parents\": [\
      {\
        \"url\": \"https://api.github.com/repos/octocat/Hello-World/commits/6dcb09b5b57875f334f61aebed695e2e4193db5e\",\
        \"sha\": \"6dcb09b5b57875f334f61aebed695e2e4193db5e\"\
      }\
    ]\
  }\
]"
#print text

#import json

data = [ { 'a':'A', 'b':(2, 4), 'c':3.0, 'c':{ 'r' :"asdfdsf", 't':"asfdsdf"} } ]
print 'DATA:', repr(data)

print 'NORMAL:', json.dumps(data, sort_keys=True)
print 'INDENT:', json.dumps(data, sort_keys=True, indent=2)