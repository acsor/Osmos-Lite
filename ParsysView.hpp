#ifndef PARTICLE_GRID_H
#define PARTICLE_GRID_H

#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"

using namespace sf;
using namespace std;


class ParsysView: public Drawable, public Observer {
	private:
		Parsys *mSys{nullptr};
		Particle &mControlled;
		unordered_map<Particle, CircleShape> *mPviews{nullptr};
	
	public:
		static const Color COLOR_MOVING;
		static const Color COLOR_GT;
		static const Color COLOR_LT;

		ParsysView(Parsys &s, Particle &controlled);
		~ParsysView();

		void draw(RenderTarget& target, RenderStates states) const override;
		void onChange(Observable const *o) override;
};


#endif
