#include "../catch.hpp"
#include "../ParticleGraph.hpp"


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
