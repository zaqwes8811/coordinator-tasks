
function IssuesAccessor()  {
  this.username_ = 'zaqwes';
  this.password_ = 'a3fs4gd5h';
  this.headers = {
    "Authorization": "Basic " + Utilities.base64Encode(this.username_ + ":" + this.password_)
  };
}

// Create issues
IssuesAccessor.prototype.createIssue = function () {
  var accountname = 'zaqwes';
  var repo_slug = 'test-compression';
  
 
  // title=asdfasdf&content=hello&status=new&priority=trivial&kind=bug
  
  var payload = {
     "title" : "value for field one",
     "content" : "value for field two",
     "status": "new",
    "priority": 'trivial',
    'kind': 'bug'
   };

   // Because payload is a JavaScript object, it will be interpreted as
   // an HTML form. (We do not need to specify contentType; it will
   // automatically default to either 'application/x-www-form-urlencoded'
   // or 'multipart/form-data')

   var options = {
     "method" : "post",
     "payload" : payload,
     "headers": this.headers
   };
  
  var issuesUrlTemplate = 'https://bitbucket.org/api/1.0/repositories/gangsofmart/'+repo_slug+'/issues/';
  
  var date = getDataFromJsonUrl(issuesUrlTemplate, options);
}

IssuesAccessor.prototype.getDataFromJsonUrl = function (url, options) {
  var response = UrlFetchApp.fetch(url, options);
  var json = response.getContentText();
  var data = JSON.parse(json);
  return data;
}

// Extract issues
IssuesAccessor.prototype.getTransmitterServiceIssues = function () {
  var repoName = 'controller-transmitter';
  var records = getBitbucketRepoIssues(repoName);
  printList(records);
}


// Note: Только для Bitbucket.org
IssuesAccessor.prototype.getBitbucketRepoIssues = function (repoName) {
  var issuesUrlTemplate = 'https://bitbucket.org/api/1.0/repositories/gangsofmart/'+repoName+'/issues/';
  var options = getBitbucketReadAccessOptions();

  // Узнаем сколько задач всего
  // Похоже запрос синхронный
  var url = issuesUrlTemplate+'?limit=1';
  var initData = getDataFromJsonUrl(url, options);
  
  // Всего задач
  var countIssues = initData['count'];
  
  // Шлем индивидуальные запросы
  var records = [];
  var oneIssueUrlTemplate = issuesUrlTemplate;
  // Номера с 1
  for (var i = 0+1; i < countIssues+1; ++i) {
    url = oneIssueUrlTemplate+i.toString()+'/';
    var dataOneIssue = getDataFromJsonUrl(url, options);
    
    // One line
    if (dataOneIssue['status'] === 'new') {
      var line = [];
      line.push(dataOneIssue['title']);
      line.push(dataOneIssue['local_id'].toString());
      line.push(dataOneIssue['content']);
      line.push(dataOneIssue['utc_created_on']);
      records.push(line);
    }
  }
  return records;  
}
