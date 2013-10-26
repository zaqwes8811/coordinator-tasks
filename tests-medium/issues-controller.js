/**
 * New node file
 */


var Accessor = require('./web-api-wrapper');

function IssuesAccessor(accessor)  {
  this.accessor_ = accessor;
}

// Create issues
IssuesAccessor.prototype.createIssue = function () {
  var payload = {
    "title" : "Hello from node.js",
    "content" : "value for field two",
    "status": "new",
    "priority": 'trivial',
    'kind': 'bug'
   };
  
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
          console.log('body : ' + body);
      } else {
          console.log('Code : ' + response.statusCode);
          console.log('error : ' + error);
          console.log('body : ' + body);
      }
  });
};

IssuesAccessor.prototype.getData = function () {
  var request = require('request');
  request.post('http://some.server.com/').auth('username', 'password', false);
};

IssuesAccessor.prototype.getDataFromJsonUrl = function (url, options) {
  var response = UrlFetchApp.fetch(url, options);
  var json = response.getContentText();
  var data = JSON.parse(json);
  return data;
};

module.exports = {'IssuesAccessor': IssuesAccessor};
