#include "catch.hpp"
#include "../Parsys.hpp"


TEST_CASE("Particle::clashes()", "[Particle]") {
	Particle p{0, 0, 1}, q{0, 1, 2}, r{0, 3, 1};

	REQUIRE(p.clashes(q));
	REQUIRE(!p.clashes(r));
	REQUIRE(q.clashes(r));
}

TEST_CASE("Particle::nearby()", "[Particle]") {
	Particle p{0, 0, 1}, q{0, 5, 2};

	REQUIRE(p.nearby(q, 5));
	REQUIRE(!p.nearby(q, 4.99));
	REQUIRE(!p.nearby(q, 4));
	REQUIRE(!p.nearby(q, 3));
	REQUIRE(!p.nearby(q, 2));
	REQUIRE(!p.nearby(q, 1));
}

TEST_CASE("Particle::merge()", "[Particle]") {
	Parsys s;
	Particle p{0, 10, 1}, q{10, 0, 1};

	SECTION("Unbound particles") {
		p.merge(q);
		REQUIRE(p == Particle(5, 5, 2));
	}

	SECTION("Bound particles") {
        p.attach(s);
		q.attach(s);

		INFO(s.toString());
		REQUIRE(s.contains(p));
		REQUIRE(s.contains(q));
		REQUIRE(s.size() == 2);

		p.merge(q);
		REQUIRE(s.contains(p));
		REQUIRE(s.size() == 1);
	}
}


TEST_CASE("Parsys::updateClashes()", "[Parsys]") {
	Parsys actual, expected;

	SECTION("Multiple simple particles") {
		Particle(actual, 1, 0, 1);
		Particle(actual, 3, 0, 2);
		Particle(actual, 6, 6, 1);

		Particle(expected, 2, 0, 3);
		Particle(expected, 6, 6, 1);

		INFO(actual.toString());
		REQUIRE(actual == expected);
	}

	SECTION("Variable-radius particles centered around the same point") {
		Particle(actual, 10, 10, 4);
		Particle(actual, 10, 10, 5);
		Particle(actual, 10, 10, 6);
		Particle(expected, 10, 10, 15);

		INFO(actual.toString());
		REQUIRE(actual == expected);
	}

	SECTION("Single repeated particle with constant radius") {
		Particle(actual, 10, 10, 5);
		Particle(actual, 10, 10, 5);
		Particle(actual, 10, 10, 5);
		Particle(expected, 10, 10, 15);

		INFO(actual.toString());
		REQUIRE(actual == expected);
	}
}

TEST_CASE("Parsys::contains()", "[Parsys]") {
	Parsys s;
	Particle ps[] = {
		{s, -1, 1, 1}, {s, 2, -3, 1}, {s, 0, 6, 1},
		{s, -10, 10, 1}, {s, 2, -30, 1}, {s, 0, 60, 15},
	};
	const size_t size = sizeof(ps) / sizeof(Particle);

	INFO("Looking inside " + s.toString() + " for... ");
	for (size_t i = 0; i < size; i++) {
		INFO(ps[i].toString());
		REQUIRE(s.contains(ps[i]));
	}

	// REQUIRE(!s.contains(ps[0].merge(ps[1])));
}

TEST_CASE("Parsys::erase()", "[Parsys]") {
	Parsys s;
	Particle ps[] = {
		{s, -1, 1, 1}, {s, 2, -3, 1}, {s, 0, 6, 1},
		{s, -10, 10, 1}, {s, 2, -30, 1}, {s, 0, 60, 15},
	};
	const size_t size = sizeof(ps) / sizeof(Particle);

	for (size_t i = 0; i < size; i++) {
		s.erase(ps[i]);
		REQUIRE(s.size() == size - 1 - i);
	}

	REQUIRE(s.size() == 0);
}
