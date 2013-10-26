// http://wiki.eclipse.org/VJET/NodeJS
// http://marketplace.eclipse.org/content/vjet-javascript-ide#.Ukscm4Zhg69
// http://mwebhack.blogspot.ru/2012/07/setting-up-vjet-as-your-eclipse-nodejs.html
// http://jonathan-whywecanthavenicethings.blogspot.in/2011/08/war-of-worlds.html ни одна ссылка не была активной
//   но некоторая информация может пригодится

//http://www.tomotaro1065.com/nodeclipse/


IssuesAccessor = require('./issues-controller');

function run() {
  var accessor = new IssuesAccessor();
  accessor.createIssue();
}


//run();