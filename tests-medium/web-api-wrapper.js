
console.log('hello');

var MyConstructor = require('./sub-module-test.js').MyConstructor;

var tmp = new MyConstructor();

var MyConstructor2 = require('./sub-module-test.js').MyConstructor2;

var tmp = new MyConstructor2();