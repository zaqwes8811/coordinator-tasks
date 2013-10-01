// http://wiki.eclipse.org/VJET/NodeJS
// http://marketplace.eclipse.org/content/vjet-javascript-ide#.Ukscm4Zhg69
// http://mwebhack.blogspot.ru/2012/07/setting-up-vjet-as-your-eclipse-nodejs.html
// http://jonathan-whywecanthavenicethings.blogspot.in/2011/08/war-of-worlds.html ни одна ссылка не была активной
//   но некоторая информация может пригодится
function IssuesAccessor()  {
  this.username_ = 'zaqwes';
  this.password_ = 'a3fs4gd5h';
  //this.headers = {
  //  "Authorization": "Basic " + Utilities.base64Encode(this.username_ + ":" + this.password_)
  //};
}

// Create issues
IssuesAccessor.prototype.createIssue = function () {
  var accountname = 'zaqwes';
  var repo_slug = 'test-compression';
  
 
  // title=asdfasdf&content=hello&status=new&priority=trivial&kind=bug
  
  var payload = {
     "title" : "Hello from node.js",
     "content" : "value for field two",
     "status": "new",
    "priority": 'trivial',
    'kind': 'bug'
   };

   // Because payload is a JavaScript object, it will be interpreted as
   // an HTML form. (We do not need to specify contentType; it will
   // automatically default to either 'application/x-www-form-urlencoded'
   // or 'multipart/form-data')

  // var options = {
  //   "method" : "post",
  //   "payload" : payload,
  //   "headers": this.headers
  // };
  
  var issuesUrlTemplate = 'https://bitbucket.org/api/1.0/repositories/gangsofmart/'+repo_slug+'/issues/';
  
  //var date = getDataFromJsonUrl(issuesUrlTemplate, options);
  
  var options = {
    uri: issuesUrlTemplate,
    auth: {
      user: this.username_,
      pass: this.password_,
      sendImmediately: true
    },
    headers: {'content-type': 'application/x-www-form-urlencoded'},
    body: require('querystring').stringify(payload)
  };
  
  var request = require('request');
  request.post(options, function(error, response, body){
      if (!error && response.statusCode == 200){
          console.log('body : ' + body)
      } else {
          console.log('Code : ' + response.statusCode)
          console.log('error : ' + error)
          console.log('body : ' + body)
      }
  });
}

IssuesAccessor.prototype.getData = function () {
  var request = require('request');
  request.post('http://some.server.com/').auth('username', 'password', false);
}

IssuesAccessor.prototype.getDataFromJsonUrl = function (url, options) {
  var response = UrlFetchApp.fetch(url, options);
  var json = response.getContentText();
  var data = JSON.parse(json);
  return data;
}

function run() {
  var accessor = new IssuesAccessor();
  accessor.createIssue();
}

function UrlFetchAppFake()  {
  this.username_ = 'zaqwes';
  this.password_ = 'a3fs4gd5h';
  //this.headers = {
  //  "Authorization": "Basic " + Utilities.base64Encode(this.username_ + ":" + this.password_)
  //};
}

run()