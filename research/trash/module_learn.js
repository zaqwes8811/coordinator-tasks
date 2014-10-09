var demoApp = angular.module('demoApp', []);


var controllers = {};

controllers.SimpleController = function ($scope) {
  $scope.customers = [
      {name: 'Dave Jones' , city: 'Phoenix'},
      {name: 'Jane Riley' , city: 'Atlanta'},
      {name: 'Heedy Wahlin' , city: 'Chandler'},
      {name: 'Thomas Winter' , city: 'Seattle'}
  ];
};

demoApp.controller(controllers);