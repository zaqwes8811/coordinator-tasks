// Авторизованный доступ и неавторизованный
var assert = require('chai').assert;
var expect = require('chai').expect;
var _ = require('underscore');
var qs = require('querystring');
var suite = require('mocha/lib/suite');
var test = require('mocha/lib/test');

// Код запросов по http - это как бы фреймворк, запускающий код, поэтому это отдельный класс,
//   которому передаются функции done

var UNAUTHORIZED_CODE = 401;
var ALL_RIGHT = 200;
var currentPassword = 'mart_user';
var newPassword = 'mart_user_a';
var login = 'mart_user';
var ip = '172.16.1.3';//'127.0.0.1';
var host = 'http://'+ip;
var passwordUrl = "http://"+ip+"/cgi-bin/ajx_passwd.cgi";

function testNoAuthCase(done){
  var testCallback = function (error, response, body) {
    if (!error) {
      assert.equal(UNAUTHORIZED_CODE, response.statusCode);
      done();
    } else {
      done();
    }
  };

  new Accessor(host).makeNoAuthGet(testCallback);
}

function testAuthCase(done){
  var testCallback = function (error, response, body) {
    // Обратный вызов завершился
    if (!error) {
      assert.equal(ALL_RIGHT, response.statusCode);
      done();  // Нужно по сути для тестирования
    } else {
      done();
    }
  };
  
  var accessor = new Accessor(host, login, currentPassword);
  accessor.makeBaseAuthGet(testCallback);
}
  
function testChangePassword(done){
  var actionOnSuccessChangePassword = function (error, response, body) {
    assert.equal(!error, 1);
    if (!error) {
      assert.equal(ALL_RIGHT, response.statusCode);
      assert.notEqual(0, response.body.indexOf("The"), "Возможно отключен модуль сбора данных");
      assert.notEqual(0, qs.parse(body).Res.indexOf("Err"), "Ошибка смены пароля");
    }
    done();
  }
  var payload = {
    'pas0': newPassword,
    'pas1': newPassword,
    'curpas': currentPassword
  };
   
  new Accessor(passwordUrl, login, currentPassword).post(payload, actionOnSuccessChangePassword);
}

function testAccessOnChangePass(done) {
  var readNoAuth = function (error, response, body) {
    if (!error) {
      expect(response.statusCode).to.equal(ALL_RIGHT);
    }
    done();
  };
  
  new Accessor(host, login, newPassword).makeBaseAuthGet(readNoAuth);
}

function testAccessOnChangePassToPassPage(done) {
  var readNoAuth = function (error, response, body) {
    if (!error) {
      expect(response.statusCode).to.equal(UNAUTHORIZED_CODE);
    }
    done();
  };
  
  new Accessor(passwordUrl, login, currentPassword).makeBaseAuthGet(readNoAuth);
}

function testPassBack(done) {
  var actionOnSuccessChangePassword = function (error, response, body) {
    assert.equal(!error, 1);
    if (!error) {
      assert.equal(ALL_RIGHT, response.statusCode);
      assert.notEqual(0, response.body.indexOf("The"), "Возможно отключен модуль сбора данных");
      assert.notEqual(0, qs.parse(body).Res.indexOf("Err"), "Ошибка смены пароля");
    }
    done();
  }
  // Меняем обратно
  payload = {
    'pas0': currentPassword,
    'pas1': currentPassword,
    'curpas': newPassword
  };
   
  new Accessor(passwordUrl, login, newPassword).post(payload, actionOnSuccessChangePassword);
}

  
/*
suite('Base access', function() {
  //@Note(base access)
  test('no auth', function(done) {testNoAuthCase(done);});
  test('auth', function(done) {testAuthCase(done);});
  
  //@Note(sequence important)
  test('change password: Set new', function(done) {testChangePassword(done);});
  test('change password: Read by new', function(done) {testAccessOnChangePass(done);});
  test('change password: Read by new', function(done) {testAccessOnChangePassToPassPage(done);});
  test('change password: Password back source', function(done) {testPassBack(done);});
  test('change password: Read by source', function(done) {testAuthCase(done);});
  //@Note(sequence important)
});
*/

var assert = require("assert")
describe('Array', function(){
  describe('#indexOf()', function(){
    it('should return -1 when the value is not present', function(){
      assert.equal(-1, [1,2,3].indexOf(5));
      assert.equal(-1, [1,2,3].indexOf(0));
    });
  });
});

describe('User', function(){
  describe('#save()', function(){
    it('should save without error', function(done){
        done();
    });
  });
});

