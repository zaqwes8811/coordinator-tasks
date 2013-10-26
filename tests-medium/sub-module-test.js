function MyConstructor (opts) {
  //...
	console.log('Ctr');
}

// BROKEN: Does not modify exports
exports = MyConstructor;

// exports the constructor properly
module.exports = MyConstructor;