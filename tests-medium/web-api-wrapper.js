
function Accessor(username, password)  {
  this.username_ = username;
  this.password_ = password;
}

Accessor.prototype.makeBaseAuthGet = function (url, done, payload) {
	var request = require('request');
	var options = {
	    url: url+'?'+require('querystring').stringify(payload),
	    //qs: require('querystring').stringify(payload),  // No work
	    auth: {
	      user: this.username_,
	      pass: this.password_,
	      sendImmediately: true
	    }  
	};
	request(options, done);
};

Accessor.prototype.makeNoAuthGet = function (action) {
  var request = require('request');
  request(this.url_, action);
};

Accessor.prototype.post = function (payload, action) {
  var options = {
    uri: this.url_,
    auth: {
      user: this.username_,
      pass: this.password_,
      sendImmediately: true
    },
    headers: {'content-type': 'application/x-www-form-urlencoded'},
    body: require('querystring').stringify(payload)
  };
  
  var request = require('request');
  request.post(options, action);
};


module.exports = {'Accessor': Accessor};

