function MyConstructor () {
  //...
	console.log('Ctr');
}

function MyConstructor2 () {
  //...
	console.log('Ctr2');
}

// BROKEN: Does not modify exports
//exports = MyConstructor;

// exports the constructor properly
module.exports = {'MyConstructor': MyConstructor, 'MyConstructor2': MyConstructor2};