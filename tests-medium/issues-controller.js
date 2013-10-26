/**
 * New node file
 */


var Accessor = require('./web-api-wrapper');

function IssuesAccessor(accessor, pathToRepo)  {
  this.accessor_ = accessor;
  this.pathToRepo_ = pathToRepo;
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

IssuesAccessor.prototype.getPkgIssues = function (idxFirst, pageSize, done) {
  if (pageSize > 50) 
    pageSize = 50;
  var payload = {
      limit: pageSize,
      start: idxFirst
  };
  var url = this.pathToRepo_+'/issues/';
  this.accessor_.makeBaseAuthGet(url, done, payload);
};

IssuesAccessor.prototype.getDataFromJsonUrl = function (url, options) {
  var response = UrlFetchApp.fetch(url, options);
  var json = response.getContentText();
  var data = JSON.parse(json);
  return data;
};

module.exports = {'IssuesAccessor': IssuesAccessor};
