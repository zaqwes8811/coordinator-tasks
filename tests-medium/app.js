// http://wiki.eclipse.org/VJET/NodeJS
// http://marketplace.eclipse.org/content/vjet-javascript-ide#.Ukscm4Zhg69
// http://mwebhack.blogspot.ru/2012/07/setting-up-vjet-as-your-eclipse-nodejs.html
// http://jonathan-whywecanthavenicethings.blogspot.in/2011/08/war-of-worlds.html ни одна ссылка не была активной
//   но некоторая информация может пригодится

//http://www.tomotaro1065.com/nodeclipse/

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

//run();