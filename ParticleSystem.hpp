#ifndef PARTICLE_GRAPH_H
#define	PARTICLE_GRAPH_H

#include <set>
#include <cmath>
#include <utility>
#include "Observer.hpp"

using namespace std;


struct Particle {
	float x, y;
	float radius;

	bool clashes (Particle const &o) const;
	bool nearby (Particle const &o, float whence) const;
	Particle merge (Particle const &o) const;
	bool operator== (Particle const &o) const;
	bool operator< (Particle const &o) const;

	string toString();
};


/**
 * An acylic undirected graph edge for connecting variable-radius particles.
 */
struct ParticleEdge {
		Particle *a, *b;

		ParticleEdge (Particle *a, Particle *b);
		pair<Particle*, Particle*> vertices() const;
		bool operator== (ParticleEdge const &e) const;
};


class ParticleSystem: public Observable {
	private:
		set<Particle> particles {};

		pair<Particle*, Particle*> findClash() const;
	public:
		ParticleSystem();
		ParticleSystem(initializer_list<Particle> in);
		void move(Particle *to_move, float xcoord, float ycord);
		void advance(Particle *to_move, float xcoord, float ycoord);
		void update();

		bool operator== (ParticleSystem const &o) const;

		string toString ();
};


#endif
