#include <stdexcept>
#include <cstring>
#include <random>
#include <ctime>
#include "Parsys.hpp"


Particle::Particle(Parsys &s, float x, float y, float radius):
	Particle(x, y, radius) {
	mSys = &s;
}

Particle::Particle(float x, float y, float radius) {
	if (radius <= 0)
		throw invalid_argument("radius must be a positive non-zero argument");

	mX = x;
	mY = y;
	mRadius = radius;
}

bool Particle::clashes (Particle const &o) const {
	return sqrt(pow(mX - o.mX, 2) + pow(mY - o.mY, 2)) <= mRadius + o.mRadius;
}

bool Particle::nearby (Particle const &o, float whence) const {
	return sqrt(pow(mX - o.mX, 2) + pow(mY - o.mY, 2)) <= whence;
}

void Particle::merge (Particle &o) {
	if (mSys != o.mSys)
		throw invalid_argument("Argument particle not within the same system");

	mX = (mX + o.mX) / 2;
	mY = (mY + o.mY) / 2;
	mRadius += o.mRadius;

	if (mSys != nullptr) {
		mSys->stopObserve();
		mSys->erase(o);
		mSys->updateClashes();
		mSys->resumeObserve();
		mSys->notify();
	}
}

bool Particle::operator== (Particle const &o) const {
	return mX == o.mX && mY == o.mY && mRadius == o.mRadius;
}

float Particle::x() const {
	return mX;
}

float Particle::y() const {
	return mY;
}

float Particle::radius() const {
	return mRadius;
}

void Particle::x(float x) {
	mX = x;

	if (mSys != nullptr)
		mSys->updateClashes();
}

void Particle::y(float y) {
	mY = y;

	if (mSys != nullptr)
		mSys->updateClashes();
}

void Particle::radius(float r) {
	if (r <= 0)
		throw invalid_argument("radius must be a positive non-zero argument");

	mRadius = r;

	if (mSys != nullptr)
		mSys->updateClashes();
}

void Particle::move(float xcomp, float ycomp) {
	mX += xcomp;
	mY += ycomp;

	if (mSys != nullptr)
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


pair<Particle*, Particle*> Parsys::findClash() const {
	// TO-DO There are at least a couple of objectives that a more performant
	// and accurate implementation of this model should follow:
	// 1. For performance's sake, a repetition of nested for-loops (as it
	// happens within updateClashes()) should be avoided;
	// 2. A hierarchy of clash orders should be established when three or more
	// particles clash at the same time.
	for (auto a = mParticles->begin(); a != mParticles->end(); a++) {
		for (auto b = mParticles->begin(); b != mParticles->end(); b++) {
			if (a != b && (*a)->clashes(**b))
				return pair<Particle*, Particle*>(
					(Particle*)&(**a), (Particle*)&(**b)
				);
		}
	}

	return pair<Particle*, Particle*>(nullptr, nullptr);
}

Parsys::Parsys(size_t capacity) {
	mParticles = new vector<shared_ptr<Particle>>();
	mParticles->reserve(capacity);

	if (mParticles->capacity() != capacity) {
        throw length_error(
			"Couldn't allocate enough memory for the particles"
		);
	}
}

Parsys::Parsys(Parsys const &o) {
	if (mParticles != nullptr)
		delete mParticles;

	mParticles = new vector<shared_ptr<Particle>>();
	mParticles->reserve(o.mParticles->capacity());
	mParticles->assign(o.mParticles->cbegin(), o.mParticles->cend());
	mDetectClash = o.mDetectClash;
}

Parsys::Parsys(Parsys &&o) {
	if (mParticles != nullptr)
		delete mParticles;

	// TO-DO Warning: won't the Particle instances in `o' maintain their
	// references to the old Parsys object?
	mParticles = o.mParticles;
	mDetectClash = o.mDetectClash;

	o.mParticles = new vector<shared_ptr<Particle>>();
	o.mParticles->reserve(mParticles->capacity());
}

Parsys::~Parsys() {
	if (mParticles != nullptr)
		delete mParticles;
}

weak_ptr<Particle> Parsys::particle(float x, float y, float radius) {
    mParticles->push_back(
		shared_ptr<Particle>(new Particle(*this, x, y, radius))
	);
	updateClashes();

    return mParticles->back();
}

bool Parsys::erase(Particle &p) {
	auto i = mParticles->begin();

    while (i != mParticles->end()) {
    	if (**i == p) {
            mParticles->erase(i);
            break;
    	}
    	i++;
    }

    if (i != mParticles->end()) {
		notify();
		return true;
	}

	return false;
}

bool Parsys::contains(Particle const &p) const {
    for (auto i = mParticles->begin(); i != mParticles->end(); i++) {
    	if (**i == p)
    		return true;
    }

    return false;
}

size_t Parsys::size() const {
	return mParticles->size();
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

Parsys::const_iterator Parsys::cbegin() const {
    return mParticles->cbegin();
}

Parsys::const_iterator Parsys::cend() const {
	return mParticles->cend();
}

Parsys& Parsys::operator= (Parsys const &o) {
	if (mParticles != nullptr) {
		delete mParticles;
	}

	mParticles = new vector<shared_ptr<Particle>>();
	mParticles->resize(
		o.mParticles->capacity(),
		shared_ptr<Particle>(new Particle(0, 0, 1E-9))
	);
	mParticles->assign(o.mParticles->cbegin(), o.mParticles->cend());
	mDetectClash = o.mDetectClash;

	return *this;
}

Parsys& Parsys::operator= (Parsys &&o) {
    if (mParticles != nullptr) {
    	delete mParticles;
    }

    mParticles = o.mParticles;
    mDetectClash = o.mDetectClash;

    o.mParticles = new vector<shared_ptr<Particle>>();
    o.mParticles->reserve(o.mParticles->capacity());

    return *this;
}

bool Parsys::operator== (Parsys const &g) const {
	for (auto i = mParticles->begin(); i != mParticles->end(); i++) {
		if (!g.contains(**i))
			return false;
	}

    for (auto i = g.mParticles->begin(); i != g.mParticles->end(); i++) {
    	if (!contains(**i))
    		return false;
    }

    return true;
}

string Parsys::toString () const {
	string o {""};

	for (auto i = mParticles->cbegin(); i != mParticles->cend(); i++) {
		o += (*i)->toString() + " ";
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
	Parsys o{n};
	// TO-DO Generate particles such that they don't clash together.

	e.seed(time(0));

	for (size_t i = 0; i < n; i++) {
		o.particle(xd(e), yd(e), abs(radiusd(e)));
	}

	return o;
}
