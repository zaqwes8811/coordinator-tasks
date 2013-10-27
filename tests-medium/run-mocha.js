/**
 * New node file
 */
var Mocha = require('mocha'),
    fs = require('fs'),
    path = require('path');


/*
//First, you need to instantiate a Mocha instance.
var mocha = new Mocha({
  ui: 'tdd',
  reporter: 'list'
})

// Then, you need to use the method "addFile" on the mocha
// object for each file.

//this is fix
//mocha.suite.on('pre-require', function(context, file) {
//  require.uncache(file);
//});
// end of fix




//Now, you can run the tests.
mocha.reporter('list').ui('tdd').run();*/

var Mocha = require('mocha'); //The root mocha path 

var mocha = new Mocha();

var passed = [];
var failed = [];

//mocha.addFile('test-small/access-test'); // direct mocha to exampleTest.js

//Here is an example:
var dirPath = './test-small/';
fs.readdirSync(dirPath).filter(function(file){
    // Only keep the .js files
    return file.substr(-3) === '.js';

}).forEach(function(file){
    // Use the method "addFile" to add the file to mocha
    mocha.addFile(
        path.join(dirPath, file)
    );
});

mocha.run(function() {
  /*
  console.log(passed.length + ' Tests Passed');
  passed.forEach(function(testName){
      console.log('Passed:', testName);
  });

  console.log("\n"+failed.length + ' Tests Failed');
  failed.forEach(function(testName){
      console.log('Failed:', testName);
  });*/

});/*.on('fail', function(test){
    failed.push(test.title);
}).on('pass', function(test){
    passed.push(test.title);
});*/