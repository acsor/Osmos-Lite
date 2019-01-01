#include "catch.hpp"
#include "../Parsys.hpp"


TEST_CASE("Particle::clashes()", "[Particle]") {
	Particle p{0, 0, 1}, q{0, 1, 2}, r{0, 3, 1};

	REQUIRE(p.clashes(q));
	REQUIRE(p.clashes(r) == 0);
	REQUIRE(q.clashes(r));
}

TEST_CASE("Particle::nearby()", "[Particle]") {
	Particle p{0, 0, 1}, q{0, 5, 2};

	REQUIRE(p.nearby(q, 5));
	REQUIRE(p.nearby(q, 4.99) == 0);
	REQUIRE(p.nearby(q, 4) == 0);
	REQUIRE(p.nearby(q, 3) == 0);
	REQUIRE(p.nearby(q, 2) == 0);
	REQUIRE(p.nearby(q, 1) == 0);
}

TEST_CASE("Particle::merge()", "[Particle]") {
	Particle p{0, 1, 1}, q{1, 0, 1};

	p.merge(q);
	REQUIRE(p == Particle(0.5, 0.5, 2));
}


TEST_CASE("Parsys::updateClashes()", "[Parsys]") {
	Parsys actual;
	Parsys expected;

	Particle(actual, 1, 0, 1);
	Particle(actual, 3, 0, 2);
	Particle(actual, 6, 6, 1);

	Particle(expected, 2, 0, 3);
	Particle(expected, 6, 6, 1);

	INFO(actual.toString());
	REQUIRE(actual == expected);
}

TEST_CASE("Parsys::contains()", "[Parsys]") {
	Parsys s;
	Particle ps[] = {
		{s, -1, 1, 1}, {s, 2, -3, 1}, {s, 0, 6, 1}
	};
	const size_t size = sizeof(ps) / sizeof(Particle);

	INFO("Looking inside " + s.toString() + " for... ");
	for (size_t i = 0; i < size; i++) {
		INFO(ps[i].toString());
		REQUIRE(s.contains(ps[i]));
	}

	// REQUIRE(!s.contains(ps[0].merge(ps[1])));
}
