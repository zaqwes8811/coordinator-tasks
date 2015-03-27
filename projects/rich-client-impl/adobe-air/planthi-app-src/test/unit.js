// form site QUnit
$(document).ready(function(){
	// separate test into module
	module("Module X");
	function toTest() {
		ok( true, "this test is fine" );
		var value = "hello";
		equal( value, "hello", "We expect value to be hello" );
	}
		
	test( "a basic test example",toTest );

	// separate test into module
	module("Module A");
	test("first test within module", function() {
	  ok( true, "all pass" );
	});

	test("second test within module", function() {
	  ok( true, "all pass" );
	});

	// separate test into module
	module("Module B");
	test("some other test", function() {
	  expect(2);
	  equal( true, true, "failing test" );
	  equal( true, true, "passing test" );
	});

});