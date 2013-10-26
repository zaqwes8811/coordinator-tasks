// Авторизованный доступ и неавторизованный
var assert = require('chai').assert
var expect = require('chai').expect;
var _ = require('underscore');
var snmp = require ("net-snmp");

// Default options
var options = {
    port: 163,
    retries: 1,
    timeout: 5000,
    transport: "udp4",
    trapPort: 162,
    version: snmp.Version2c
};

var ip = "172.16.1.3"
var powerOid = "1.3.6.1.4.1.34491.1.1.2.3.0";

var oids = [
"1.3.6.1.4.1.34491.1.1.1.1.0",
"1.3.6.1.4.1.34491.1.1.2.1.0",
"1.3.6.1.4.1.34491.1.1.2.3.0",  // power
"1.3.6.1.4.1.34491.1.1.2.5.0",
"1.3.6.1.4.1.34491.1.1.2.6.0"
];

function testConnection(done) {
  var session = snmp.createSession (ip, "public", options);
  session.get(oids, function (error, varbinds) {
    assert.notEqual(0, error, "Ошибка при обработка GET-запроса.");
    done();
  });
}


function makeGet(done) {
  var session = snmp.createSession (ip, "public", options);
  session.get(oids, function (error, varbinds) {
    assert.notEqual(true, error, "Ошибка при обработка GET-запроса.");
    
    var activeResponses = 0;
    if (error) {
      console.error (error.toString ());
    } else {
      for (var i = 0; i < varbinds.length; i++) {
        var isValid = !snmp.isVarbindError(varbinds[i]);
        expect(isValid).to.equal(true);
        if (isValid) {
          activeResponses++;
        }
      }
      assert.equal(activeResponses, varbinds.length);
    }
    done();
  });
}

function testGETPower(done) {
  makeGet(done);
}

suite('Snmp access', function() {
  test('Power', function(done) {testConnection(done);});
  test('Power', function(done) {testGETPower(done);});
})