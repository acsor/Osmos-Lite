#ifndef PARTICLE_GRAPH_H
#define	PARTICLE_GRAPH_H

#include <unordered_set>
#include <cmath>
#include <utility>
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

	public:
		/**
		 * @brief Bound-constructor attaching the new particle to the particle
		 * system `s`. Note that the newly created object might result
		 * unattached even just after its creation, i.e. if some collision
		 * occurs.
		 */
		Particle(Parsys &s, float x, float y, float radius);
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
		void merge (Particle const &o);
		bool operator== (Particle const &o) const;

		/**
		 * @return `true` if the calling object is found within its particle
		 * system.
		 */
		inline bool attached() const;
		/**
		 * @brief Attaches the current Particle to the belonging particle
		 * system. Note that the newly attached particle might not be found
		 * into the system even right after this call if, for example, it gets
		 * immediately merged.
		 */
		inline void attach();
		void attach(Parsys &s);
		inline float x() const;
		inline float y() const;
		inline float radius() const;
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
		unordered_set<Particle> *particles {nullptr};
		bool mDetectClash {true};

		pair<Particle*, Particle*> findClash() const;
	protected:
		void updateClashes();
	public:
		Parsys();
		virtual ~Parsys();
		/**
		 * @return `true` if `p` was successfully added as a new particle into
		 * the system.
		 */
		bool add(Particle const &p);
		/**
		 * @return `true` if `p` was successfully removed from the system.
		 */
		bool erase(Particle const &p);
		bool contains(Particle const &p) const;
		size_t size() const;
		void toggleDetectClash();

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
