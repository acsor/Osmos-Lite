#ifndef PARTICLE_GRAPH_H
#define	PARTICLE_GRAPH_H

#include <set>
#include <cmath>
#include <utility>

using namespace std;


struct Particle {
	float x, y;
	float radius;

	bool clashes (Particle const &o) const;
	bool nearby (Particle const &o, float whence);
	Particle merge (Particle const &o) const;
	bool operator== (Particle const &o) const;
	bool operator< (Particle const &o) const;
};


/**
 * An acylic undirected graph edge for connecting variable-radius particles.
 */
class ParticleEdge {
	private:
		Particle *a, *b;
	public:
		ParticleEdge (Particle *a, Particle *b);
		pair<Particle*, Particle*> vertices() const;
		bool operator== (ParticleEdge const &e) const;
};


class ParticleGraph {
	private:
		set<Particle> particles {};
		set<ParticleEdge> edges {};

	public:
		static float const constexpr NEARBY_RADIUS = 5;

		ParticleGraph();
		ParticleGraph(initializer_list<Particle> in);
		void update();
		void move(Particle *to_move, float xcoord, float ycord);
		void advance(Particle *to_move, float xcoord, float ycord);
};


#endif
