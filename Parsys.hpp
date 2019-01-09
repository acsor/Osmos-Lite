#ifndef PARTICLE_GRAPH_H
#define	PARTICLE_GRAPH_H

#include <vector>
#include <cmath>
#include <utility>
#include <memory>
#include "Observer.hpp"

// TO-DO Many a developer suggests that using-declarations ought to be removed
// entirely from the source code.
using namespace std;


class Parsys;


class Particle {
	friend class Parsys;
	friend class hash<Particle>;

	private:
		float mX, mY;
		float mRadius;

		Parsys *mSys {nullptr};
	protected:
		/**
		 * @brief Bound-constructor attaching the new particle to the particle
		 * system `s`. Note that the newly created object might result
		 * unattached even just after its creation, i.e. if some collision
		 * occurs.
		 */
		Particle(Parsys &s, float x, float y, float radius);
	public:
		Particle(float x, float y, float radius);
		/**
		 * @return `true` if the calling object clashes with `o`, `false` if
		 * `o` belongs to another particle system or does not clash.
		 */
		bool clashes (Particle const &o) const;
		bool nearby (Particle const &o, float whence) const;
		/**
		 * @brief Merges the current instance with `o' in such a way that the
		 * new information is stored into the calling object.
		 *
		 * @param o Other particle that will be removed from the system.
		 */
		void merge (Particle &o);
		bool operator== (Particle const &o) const;

		// TO-DO Making some of the calls below `inline' causes the linker to
		// not... link. Find out why and fix the issue.
		float x() const;
		float y() const;
		float radius() const;
		void x(float x);
		void y(float y);
		void radius(float r);

		void shift(float xcomp, float ycomp);

		string toString() const;
};


// Needed for unordered containers that specialize to `Particle'.
namespace std {
	template<> struct hash<Particle> {
		size_t operator()(Particle const &p) const;
	};
}


/**
 * An acylic undirected graph edge for connecting variable-radius particles.
 */
struct ParticleEdge {
		Particle *a, *b;

		ParticleEdge (Particle *a, Particle *b);
		pair<Particle*, Particle*> vertices() const;
		bool operator== (ParticleEdge const &e) const;
};


/**
 * @brief A particle system (Parsys) class managing groups of multiple
 * particles. The system features an auto-merging property, whereby clashing
 * are merged together (the smaller into the bigger one).
 */
class Parsys: public Observable {
	friend class Particle;

	private:
		// Although not specifically intended for insertion/editing of data,
		// the typical usage of a Parsys instance is to have elements plucked
		// one after another, and the use of vector is sustainable: in fact,
		// since read accesses are more common, it can prove a good choice.
		vector<shared_ptr<Particle>> *mParticles {nullptr};
		bool mDetectClash {true};

		pair<Particle*, Particle*> findClash() const;
	protected:
		void updateClashes();
	public:
		typedef vector<shared_ptr<Particle>>::const_iterator const_iterator;

		Parsys(size_t capacity);
		// TO-DO Learn more about move semantics and when they should be
		// implemented.
		Parsys(Parsys const &o);
		Parsys(Parsys &&o);
		virtual ~Parsys();
		weak_ptr<Particle> particle(float x, float y, float radius);
		/**
		 * @return `true` if `p` was successfully removed from the system.
		 */
		bool erase(Particle &p);
		bool contains(Particle const &p) const;
		size_t size() const;
		void toggleDetectClash();

		// TO-DO Study more carefully iterator mechanisms and all their prior
		// basics (including Generic Programming and whatnot).
		const_iterator cbegin() const;
		const_iterator cend() const;

		Parsys& operator= (Parsys const &o);
		Parsys& operator= (Parsys &&o);
		bool operator== (Parsys const &o) const;

		// TO-DO Devise a more standard string-conversion mechanism.
		string toString () const;
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
