var demoApp = angular.module('demoApp', []);

demoApp.controller('SimpleController', function ($scope) {
  $scope.customers = [
      {name: 'Dave Jones' , city: 'Phoenix'},
      {name: 'Jane Riley' , city: 'Atlanta'},
      {name: 'Heedy Wahlin' , city: 'Chandler'},
      {name: 'Thomas Winter' , city: 'Seattle'}
  ];
});