// http://wiki.eclipse.org/VJET/NodeJS
// http://marketplace.eclipse.org/content/vjet-javascript-ide#.Ukscm4Zhg69
// http://mwebhack.blogspot.ru/2012/07/setting-up-vjet-as-your-eclipse-nodejs.html
// http://jonathan-whywecanthavenicethings.blogspot.in/2011/08/war-of-worlds.html ни одна ссылка не была активной
//   но некоторая информация может пригодится

//http://www.tomotaro1065.com/nodeclipse/


var IssuesAccessor = require('./issues-controller');
var Accessor = require('./web-api-wrapper').Accessor;

function done(error, response, body) {
  console.log(JSON.parse(body));
}

function run(done) {
  var username = 'zaqwes';
  var password = 'a3fs4gd5h';
  
  var repo_slug = 'decoder-reed-solomon';
  var payload = {
      limit: 2,
      start: 1
  };
  var pathToRepo = 'https://bitbucket.org/api/1.0/repositories/zaqwes/'+repo_slug;
  var issuesUrlTemplate = pathToRepo+'/issues/';
  var accessor = new Accessor(issuesUrlTemplate, username, password);
  
  accessor.makeBaseAuthGet(done, payload);
  
  //accessor.createIssue();
}

run(done);


