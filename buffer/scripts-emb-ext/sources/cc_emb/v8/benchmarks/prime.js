function Primes() {
	// ...
	this.primes = [];
	this.prime_count = 0;

	this.getPrimeCount = function () {
		return this.prime_count;
	};

	this.addPrime = function(i) {
		this.primes[this.prime_count++] = i;
	};

	this.isPrimeDivisible = function (candidate) {
		for (var i = 1; i <= this.prime_count; ++i) {
			if ((candidate % this.primes[i]) == 0) {
				return true;
			}
		}
		return false;
	};
}

function main() {
	p = new Primes();
	var c = 1;
	while (p.getPrimeCount() < 25000) {
		if (!p.isPrimeDivisible(c)) {
			p.addPrime(c);
		}
		c++;
	}
}

main();