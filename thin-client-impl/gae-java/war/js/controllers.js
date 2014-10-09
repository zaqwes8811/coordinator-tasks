var phonecatApp = angular.module('phonecatApp', []);
var controllers = {};

controllers.PhoneListCtrlAjax = ['$scope', '$http', function($scope, $http) {
  // action at once
  $http.get('phones/phones.json').success(function(data) {
    $scope.phones = data;
  });

  $scope.orderProp = 'age';
}];

controllers.PhoneListCtrl = function($scope) {
  $scope.phones = [
                   {'name': 'Nexus S',
                    'snippet': 'Fast just got faster with Nexus S.',
                    'age': 7},
                   {'name': 'Motorola XOOM™ with Wi-Fi',
                    'snippet': 'The Next, Next Generation tablet.',
                    'age': 2},
                   {'name': 'MOTOROLA XOOM™',
                    'snippet': 'The Next, Next Generation tablet.',
                    'age': 3}
                 ];

  $scope.hello = "hello world";
  $scope.orderProp = 'age';
};




phonecatApp.controller(controllers);
