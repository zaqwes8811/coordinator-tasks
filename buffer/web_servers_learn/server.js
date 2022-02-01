// Install node
/*
 https://github.com/joyent/node/wiki/installation
 
 http://stackoverflow.com/questions/13465829/node-js-modules-path
 */

var cgi = require('cgi');
var http = require('http');
var path = require('path');

// chmod +x обязательно
var script = path.resolve(__dirname, 'cgi_action.py');

http.createServer( cgi(script) ).listen(8080);