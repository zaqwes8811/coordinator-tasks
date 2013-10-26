// http://wiki.eclipse.org/VJET/NodeJS
// http://marketplace.eclipse.org/content/vjet-javascript-ide#.Ukscm4Zhg69
// http://mwebhack.blogspot.ru/2012/07/setting-up-vjet-as-your-eclipse-nodejs.html
// http://jonathan-whywecanthavenicethings.blogspot.in/2011/08/war-of-worlds.html ни одна ссылка не была активной
//   но некоторая информация может пригодится

//http://www.tomotaro1065.com/nodeclipse/


var IssuesAccessor = require('./issues-controller').IssuesAccessor;
var Accessor = require('./web-api-wrapper').Accessor;

function done(error, response, body) {
  var issues = JSON.parse(body)['issues'];
  
  // Запускаем цепочку переливки
}

function run(done) {
  var username = 'zaqwes';
  var password = 'a3fs4gd5h';
  
  //var repo_slug = 'decoder-reed-solomon';
  var repo_slug = "controller-transmitters-rtos";
  
  var pathToRepo = 'https://bitbucket.org/api/1.0/repositories/gangsofmart/'+repo_slug;
  var accessor = new Accessor(username, password);

  issuesAccessor = new IssuesAccessor(accessor, pathToRepo);
  issuesAccessor.getPkgIssues(1, 100, done);
}

run(done);


