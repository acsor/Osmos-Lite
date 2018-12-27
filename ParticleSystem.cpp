#include <stdexcept>
#include <cstring>
#include <random>
#include <ctime>
#include "ParticleSystem.hpp"


bool Particle::clashes (Particle const &o) const {
	return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2)) <= radius + o.radius;
}

bool Particle::nearby (Particle const &o, float whence) const {
	return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2)) <= whence;
}

Particle Particle::merge (Particle const &o) const {
	return {
		(x + o.x) / 2, (y + o.y) / 2, radius + o.radius
	};
}

bool Particle::operator== (Particle const &o) const {
	return x == o.x && y == o.y && radius == o.radius;
}

bool Particle::operator< (Particle const &o) const {
	return x < o.x ? true: y < o.y;
}

string Particle::toString () {
	char o[40];

	snprintf(o, 40, "{%.2f, %.2f, %.2f}", x, y, radius);

	return o;
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


inline pair<Particle*, Particle*> ParticleSystem::findClash() const {
	for (auto a = particles.begin(); a != particles.end(); a++) {
		for (auto b = next(a); b != particles.end(); b++) {
			if (a->clashes(*b))
				return pair<Particle*, Particle*>(
					(Particle*)&(*a), (Particle*)&(*b)
				);
		}
	}

	return pair<Particle*, Particle*>(nullptr, nullptr);
}

ParticleSystem::ParticleSystem() {
}

ParticleSystem::ParticleSystem(initializer_list<Particle> in) {
	for (auto i = in.begin(); i != in.end(); i++) {
		particles.insert(*i);
	}
}

bool ParticleSystem::add(Particle const &p) {
	// TO-DO Decide whether to add exception throwing in place of return-value
	// signaling.
	if (particles.find(p) != particles.end())
		return true;

	particles.insert(p);
	return false;
}

void ParticleSystem::update() {
	pair<Particle*, Particle*> p = findClash();

	while (p.first != nullptr && p.second != nullptr) {
		particles.insert(p.first->merge(*(p.second)));
		particles.erase(*(p.first));
		particles.erase(*(p.second));

		p = findClash();
	}
}

void ParticleSystem::move(Particle *to_move, float xcoord, float ycoord) {
	to_move->x = xcoord;
	to_move->y = ycoord;
	update();
}

void ParticleSystem::advance(Particle *to_move, float xcoord, float ycoord) {
	to_move->x += xcoord;
	to_move->y += ycoord;
	update();
}

bool ParticleSystem::operator== (ParticleSystem const &g) const {
	return particles == g.particles;
}

string ParticleSystem::toString () {
	// TO-DO Implement with proper (and not heuristic) character allocation.
	char buff[30], s[20 * particles.size()] = "";
	size_t size = sizeof(s) / sizeof(char);

	for (auto i = particles.begin(); i != particles.end(); i++) {
		snprintf(buff, 30, "{%.2f, %.2f, %.2f}", i->x, i->y, i->radius);
		strncat(s, buff, size);
		size -= strlen(buff);

		if (next(i) != particles.end()) {
			strncat(s, ", ", size);
			size -= 2;
		}
	}

	return s;
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

ParticleSystem UnirandParsysGen::generate () {
	uniform_real_distribution<float> xd{min, max}, yd{min, max};
	normal_distribution<float> radiusd{radius_mean, radius_stddev};
	default_random_engine e;
	ParticleSystem o;
	Particle p;

	e.seed(time(0));

	for (int i = 0; (size_t)i < n; i++) {
		p = Particle{xd(e), yd(e), radiusd(e)};

		if (o.add(p))	// If `p' was already into `o':
			i--;
	}

	return o;
}
