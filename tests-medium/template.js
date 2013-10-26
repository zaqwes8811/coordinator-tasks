function OneCard(context) {
  this.context_ = context;
}
 
OneCard.prototype.exchange = function () {
  console.log(this);
}

suite('Base access', function() {
  test('need auth', function(done) {
    var request = require('request');
    var card = new OneCard("hello");
    var responseProcessor = _.bind(card.exchange, card)
    request('http://192.168.1.230/a_ajxtmn.cgi', 
        function (error, response, body) {
          if (!error) {
            done();
            assert.equal(403, response.statusCode);
            responseProcessor();
            card.exchange();
          }
    });
  });
})

suite('nextPrime', function() {
  test('nextPrime should return the next prime number', function() {
    var Person = {
      name: "Tim",
      age: 28,
      greeting: function () {
        return "Hello " + this.name + ".  Wow, you are " + this.age + " years old.";
      }
    };

    Person.greeting();

    var greeting = Person.greeting;
    // BAD!
    //console.log(greeting()); // Will get undefined for `this.name` and `this.age`
  });
});

