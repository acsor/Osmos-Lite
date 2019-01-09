#include <unordered_set>
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
	Particle p{0, 10, 1}, q{10, 0, 1};
	Parsys s{10};
	shared_ptr<Particle> t = s.particle(0, 10, 1).lock(),
	        u = s.particle(10, 0, 1).lock();

	// Unbound particles
	p.merge(q);
	REQUIRE(p == Particle(5, 5, 2));

	// Bound particles
	INFO(s.toString());
	REQUIRE(s.contains(*t));
	REQUIRE(s.contains(*u));
	REQUIRE(s.size() == 2);

	t->merge(*u);
	INFO(s.toString());
	REQUIRE(t->x() == 5);
	REQUIRE(t->y() == 5);
	REQUIRE(t->radius() == 2);
	REQUIRE(s.contains(*t));
	REQUIRE(s.size() == 1);
}


TEST_CASE("Parsys::Parsys()", "[Parsys]") {
	Parsys s{10}, r{10};
	shared_ptr<Particle> p;

	SECTION("Copy assignment") {
		s.particle(0, 0, 1);
		s.particle(10, 0, 1);
		s.particle(0, 10, 1);

		r = s;

		REQUIRE(s == r);
	}

	SECTION("Duplicate element") {
		s.particle(0, 0, 1);
		p = s.particle(0, 0, 1).lock();

		INFO(p->toString());
		REQUIRE(*p == Particle(0, 0, 2));
	}
}

TEST_CASE("Parsys::updateClashes()", "[Parsys]") {
	Parsys actual{10}, expected{10};

	SECTION("Multiple simple particles") {
		actual.particle(1, 0, 1);
		actual.particle(3, 0, 2);
		actual.particle(6, 6, 1);

		expected.particle(2, 0, 3);
		expected.particle(6, 6, 1);

		INFO(actual.toString());
		REQUIRE(actual == expected);
	}

	SECTION("Variable-radius particles centered around the same point") {
		actual.particle(10, 10, 4);
		actual.particle(10, 10, 5);
		actual.particle(10, 10, 6);
		expected.particle(10, 10, 15);

		INFO(actual.toString());
		REQUIRE(actual == expected);
	}

	SECTION("Single repeated particle with constant radius") {
		actual.particle(10, 10, 5);
		actual.particle(10, 10, 5);
		actual.particle(10, 10, 5);
		expected.particle(10, 10, 15);

		INFO(actual.toString());
		REQUIRE(actual == expected);
	}
}

TEST_CASE("Parsys::contains()", "[Parsys]") {
	Parsys s{10};
	shared_ptr<Particle> ps[] = {
		s.particle(-1, 1, 1).lock(), s.particle(2, -3, 1).lock(),
		s.particle(0, 6, 1).lock(), s.particle(-10, 10, 1).lock(),
		s.particle(2, -30, 1).lock(), s.particle(0, 60, 15).lock(),
	};
	const size_t size = sizeof(ps) / sizeof(Particle);

	INFO("Looking inside " + s.toString() + " for... ");
	for (size_t i = 0; i < size; i++) {
		INFO(ps[i]->toString());
		REQUIRE(s.contains(*ps[i]));
	}

	// REQUIRE(!s.contains(ps[0].merge(ps[1])));
}

TEST_CASE("Parsys::erase()", "[Parsys]") {
	Parsys s{10};
	shared_ptr<Particle> ps[] = {
		s.particle(-1, 1, 1).lock(), s.particle(2, -3, 1).lock(),
		s.particle(0, 6, 1).lock(), s.particle(-10, 10, 1).lock(),
		s.particle(2, -30, 1).lock(), s.particle(0, 60, 15).lock(),
	};
	const size_t size = sizeof(ps) / sizeof(shared_ptr<Particle>);

	for (size_t i = 0; i < size; i++) {
		s.erase(*ps[i]);
		REQUIRE(s.contains(*ps[i]) == false);
		REQUIRE(s.size() == size - 1 - i);
	}

	REQUIRE(s.size() == 0);
}

TEST_CASE("Parsys::cbegin() Parsys::cend()", "[Parsys]") {
	Parsys s{10};
    unordered_set<shared_ptr<Particle>> ps {
		s.particle(0, 0, 1).lock(), s.particle(10, 0, 1).lock(),
		s.particle(0, 10, 1).lock(), s.particle(-10, 0, 1).lock(),
		s.particle(0, -10, 1).lock(),
    };

    REQUIRE(s.size() == ps.size());
    // Note that the Parsys::cend() method is tested as well.
	for (auto i = s.cbegin(); i != s.cend(); i++) {
		REQUIRE(ps.find(*i) != ps.end());
	}
}
