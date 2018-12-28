#ifndef PARTICLE_GRAPH_H
#define	PARTICLE_GRAPH_H

#include <set>
#include <cmath>
#include <utility>
#include "Observer.hpp"

using namespace std;


class Parsys;


struct Particle {
	float x, y;
	float radius;
	Parsys *s {nullptr};

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


class Parsys: public Observable {
	private:
		set<Particle> particles {};

		pair<Particle*, Particle*> findClash() const;
	public:
		Parsys();
		Parsys(initializer_list<Particle> in);
		bool add(Particle const &p);
		bool contains(Particle const &p);
		void move(Particle *to_move, float xcoord, float ycord);
		void advance(Particle *to_move, float xcoord, float ycoord);
		void update();

		bool operator== (Parsys const &o) const;

		string toString ();
};


class ParsysGen {
	public:
		virtual ~ParsysGen();
		virtual Parsys generate () = 0;
};

class UnirandParsysGen: public ParsysGen {
	private:
		size_t n;
		float min, max, radius_mean, radius_stddev;
	public:
		UnirandParsysGen (size_t n, float min, float max, float radius_mean);
		Parsys generate () override;
};


#endif
