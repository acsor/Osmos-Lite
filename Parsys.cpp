#include <stdexcept>
#include <cstring>
#include <random>
#include <ctime>
#include "Parsys.hpp"


Particle::Particle(Parsys &s, float x, float y, float radius):
	Particle(x, y, radius) {
	mSys = &s;
	attach();
}

Particle::Particle(float x, float y, float radius) {
	if (radius <= 0)
		throw invalid_argument("radius must be a positive non-zero argument");

	mSys = nullptr;
	mX = x;
	mY = y;
	mRadius = radius;
}

bool Particle::clashes (Particle const &o) const {
	return sqrt(pow(mX - o.mX, 2) + pow(mY - o.mY, 2)) <=
		mRadius + o.mRadius;
}

bool Particle::nearby (Particle const &o, float whence) const {
	return sqrt(pow(mX - o.mX, 2) + pow(mY - o.mY, 2)) <= whence;
}

void Particle::merge (Particle const &o) {
	if (mSys != o.mSys)
		throw invalid_argument("Argument particle not within the same system");

	if (mSys != nullptr) {
		mSys->stopObserve();
		mSys->erase(*this);
		mSys->erase(o);
	}

	mX = (mX + o.mX) / 2;
	mY = (mY + o.mY) / 2;
	mRadius += o.mRadius;

	if (mSys != nullptr) {
		mSys->add(*this);
		mSys->resumeObserve();
	}
}

bool Particle::operator== (Particle const &o) const {
	return mX == o.mX && mY == o.mY && mRadius == o.mRadius;
}

inline bool Particle::attached() const {
	return mSys != nullptr && mSys->contains(*this);
}

inline void Particle::attach() {
	mSys->add(*this);
	mSys->updateClashes();
}

inline float Particle::x() const {
	return mX;
}

inline float Particle::y() const {
	return mY;
}

inline float Particle::radius() const {
	return mRadius;
}

void Particle::x(float x) {
	mX = x;
	mSys->updateClashes();
}

void Particle::y(float y) {
	mY = y;
	mSys->updateClashes();
}

void Particle::radius(float r) {
	if (r <= 0)
		throw invalid_argument("radius must be a positive non-zero argument");

	mRadius = r;
	mSys->updateClashes();
}

void Particle::shift(float xcomp, float ycomp) {
	mX += xcomp;
	mY += ycomp;
	mSys->updateClashes();
}

string Particle::toString () const {
	char o[100];

	snprintf(o, 100, "{%.2f, %.2f, %.2f}", mX, mY, mRadius);

	return o;
}


size_t hash<Particle>::operator() (Particle const &p) const {
	return hash<float>()(p.mX) ^ hash<float>()(p.mY) ^
	        hash<float>()(p.mRadius);
}


ParticleEdge::ParticleEdge (Particle *a, Particle *b) {
	if (*a == *b)
		throw invalid_argument("Cannot have cycles.");

	this->a = a;
	this->b = b;
}

pair<Particle*, Particle*> ParticleEdge::vertices() const {
	return {a, b};
}

bool ParticleEdge::operator== (ParticleEdge const &e) const {
	return (*a == *(e.a)) && (*b == *(e.b));
}


inline pair<Particle*, Particle*> Parsys::findClash() const {
	// TO-DO There are at least a couple of objectives that a more performant
	// and accurate implementation of this model should follow:
	// 1. For performance's sake, a repetition of nested for-loops (as it
	// happens within updateClashes()) should be avoided;
	// 2. A hierarchy of clash orders should be defined when three or more
	// particles clash at the same time.
	for (auto a = particles->begin(); a != particles->end(); a++) {
		for (auto b = particles->begin(); b != particles->end(); b++) {
			if (a != b && a->clashes(*b))
				return pair<Particle*, Particle*>(
					(Particle*)&(*a), (Particle*)&(*b)
				);
		}
	}

	return pair<Particle*, Particle*>(nullptr, nullptr);
}

Parsys::Parsys() {
	particles = new unordered_set<Particle>();
}

Parsys::~Parsys() {
	if (particles != nullptr)
		delete particles;
}

bool Parsys::add(Particle const &p) {
	bool inserted = particles->insert(p).second;
	updateClashes();

	return inserted;
}

bool Parsys::erase(Particle const &p) {
	bool erased = particles->erase(p) > 0;
	notify();

	return erased;
}

bool Parsys::contains(Particle const &p) const {
	return p.mSys == this && particles->find(p) != particles->end();
}

size_t Parsys::size() const {
	return particles->size();
}

void Parsys::toggleDetectClash() {
	mDetectClash = !mDetectClash;
}

void Parsys::updateClashes() {
	pair<Particle*, Particle*> p{nullptr, nullptr};

	if (!mDetectClash)
		return;

	// We do not want to fire up too many update events -- GUI may take lots of
	// time by rebuilding their status at each loop iteration.
	stopObserve();
	p = findClash();

	while (p.first != nullptr && p.second != nullptr) {
		// Merge the particle with the smaller radius into the one which has it
		// bigger.
		if (p.first->radius() > p.second->radius())
			p.first->merge(*p.second);
		else
			p.second->merge(*p.first);

		p = findClash();
	}

	resumeObserve();
	notify();
}

bool Parsys::operator== (Parsys const &g) const {
	return *particles == *(g.particles);
}

string Parsys::toString () const {
	string o {""};

	for (auto i = particles->begin(); i != particles->end(); i++) {
		o += i->toString() + " ";
	}

	return o;
}


ParsysGen::~ParsysGen () {
}

UnirandParsysGen::UnirandParsysGen(
	size_t n, float min, float max, float radius_mean
) {
	this->n = n;
	this->min = min;
	this->max = max;
	this->radius_mean = radius_mean;
	this->radius_stddev = sqrt(radius_mean);
}

Parsys UnirandParsysGen::generate () {
	uniform_real_distribution<float> xd{min, max}, yd{min, max};
	normal_distribution<float> radiusd{radius_mean, radius_stddev};
	default_random_engine e;
	Parsys o;
	// TO-DO Generate particles such that they don't clash together.

	e.seed(time(0));

	for (size_t i = 0; i < n; i++) {
		Particle(o, xd(e), yd(e), abs(radiusd(e)));
	}

	// TO-DO Watch out! Won't the default copy constructor have `o' destroy the
	// shared memory area pointed to by `o.particles'?

	return o;
}
