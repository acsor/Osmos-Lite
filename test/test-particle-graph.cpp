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

TEST_CASE("Particle::merge()", "[Particle]") {
	Particle p{0, 1, 1}, q{1, 0, 1};

	REQUIRE(p.merge(q) == Particle{0.5, 0.5, 2});
}


TEST_CASE("ParticleGraph::update()", "[ParticleGraph]") {
	ParticleGraph g{
		Particle{1, 0, 1}, Particle{3, 0, 2}, Particle{6, 6, 1}
	};
	ParticleGraph expected{
		Particle{2, 0, 3}, Particle{6, 6, 1}
	};

	g.update();
	INFO(g.toString());
	REQUIRE(g == expected);
}
