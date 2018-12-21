#include <stdexcept>
#include "ParticleGraph.hpp"


bool Particle::clashes (Particle const &o) const {
	return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2)) <= radius + o.radius;
}

bool Particle::nearby (Particle const &o, float whence) {
	return sqrt(pow(x - o.x, 2) + pow(y - o.y, 2)) <= whence;
}

bool Particle::operator== (Particle const &o) const {
	return x == o.x && y == o.y && radius == o.radius;
}

bool Particle::operator< (Particle const &o) const {
	return x < o.x ? true: y < o.y;
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


ParticleGraph::ParticleGraph() {
}

ParticleGraph::ParticleGraph(initializer_list<Particle> in) {
	for (auto i = in.begin(); i != in.end(); i++) {
		particles.insert(*i);
	}
}

void ParticleGraph::update() {
}

void ParticleGraph::move(Particle *to_move, float xcoord, float ycord) {

}

void ParticleGraph::advance(Particle *to_move, float xcoord, float ycord) {

}
