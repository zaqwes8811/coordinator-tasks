/*describe('PhoneCat controller', function() {
  describe('PhoneListCtrl', function() {
    it('should create "phones" model with 3 phones', function() {
      var scope = {},
        ctrl = new PhoneListCtrl(scope);
      expect(scope.phones.length).toBe(3);
    })
  });
});*/

describe('PhoneCat controllers AJS', function() {
  beforeEach(module('phonecatApp'));
 
  describe('PhoneListCtrl', function(){
 
    it('should create "phones" model with 3 phones', inject(function($controller) {
      var scope = {},
          ctrl = $controller('PhoneListCtrl', { $scope: scope });
 
      expect(scope.phones.length).toBe(3);
    }));
  });
});

describe('PhoneCat controllers AJS2', function() {
  beforeEach(module('phonecatApp'));
 
  describe('PhoneListCtrl', function(){
 
    it('should create "phones" model with 3 phones', 
        inject(function($controller) {
      var scope = {},
          ctrl = $controller('PhoneListCtrl', { $scope: scope });
 
      expect(scope.phones.length).toBe(3);
    }));
  });
});

/*
describe('PhoneCat controllers AJS21', function() {
  beforeEach(module('phonecatApp'));
 
  describe('PhoneListCtrl', function(){
 
    it('should create "phones" model with 3 phones', 
        inject(function($controller) {
      var scope = {},
          ctrl = $controller('PhoneListCtrlAjax', { $scope: scope });
 
      expect(scope.phones.length).toBe(3);
    }));
  });
});
*/

describe('PhoneCat controllers', function() {
  describe('PhoneListCtrl', function(){
    var scope, ctrl, $httpBackend;
    beforeEach(module('phonecatApp'));
 
    // The injector ignores leading and trailing underscores here (i.e. _$httpBackend_).
    // This allows us to inject a service but then attach it to a variable
    // with the same name as the service.
    beforeEach(inject(function(_$httpBackend_, $rootScope, $controller) {
      $httpBackend = _$httpBackend_;
      $httpBackend.expectGET('phones/phones.json').
          respond([{name: 'Nexus S'}, {name: 'Motorola DROID'}]);
 
      scope = $rootScope.$new();
      ctrl = $controller('PhoneListCtrlAjax', {$scope: scope});
    }));

    it('should create "phones" model with 2 phones fetched from xhr', function() {
      expect(scope.phones).toBeUndefined();
      $httpBackend.flush();
     
      expect(scope.phones).toEqual([{name: 'Nexus S'},
                                   {name: 'Motorola DROID'}]);
    });

    it('should set the default value of orderProp model', function() {
      expect(scope.orderProp).toBe('age');
    });

  });
});
