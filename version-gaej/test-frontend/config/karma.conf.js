module.exports = function(config){
  config.set({
    basePath : '../../',

    files : [
      'war/lib/angular/angular.js',
      'war/lib/angular/angular-*.js',
      'war/js/**/*.js',
      'test-frontend/unit/**/*.js'
    ],

    exclude : [
      'war/lib/angular/angular-loader.js',
      'war/lib/angular/*.min.js',
      'war/lib/angular/angular-scenario.js'
    ],

    autoWatch : true,

    frameworks: ['jasmine'],

    browsers : ['Chrome'],

    plugins : [
            'karma-junit-reporter',
            'karma-chrome-launcher',
            'karma-firefox-launcher',
            'karma-script-launcher',
            'karma-jasmine'
            ],

    junitReporter : {
      outputFile: 'test_out/unit.xml',
      suite: 'unit'
    }
  });
};
