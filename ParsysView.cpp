#include <typeinfo>
#include "ParsysView.hpp"
#include "Observer.hpp"


/**
 * @return Color for a given particle with reference to the commanded one (i.e.
 * `moving').
 */
static Color particleColor(Particle const &p, Particle const &moving);


static const Color COLOR_MOVING = Color::Cyan;
static const Color COLOR_GT = Color::Red;
static const Color COLOR_LT = Color::Blue;

ParsysView::ParsysView(Parsys &s, Particle &controlled):
	mControlled{controlled} {
	mSys = &s;
	mSys->attach(this);
	mSys->notify();

	mPviews = new unordered_map<Particle, CircleShape>();
}

ParsysView::~ParsysView() {
	if (mPviews != nullptr)
		delete mPviews;
	mSys->detach(this);
}

void ParsysView::draw(RenderTarget& target, RenderStates states) const {
	size_t n = 0;

	for (auto i = mPviews->cbegin(); i != mPviews->cend(); i++) {
		target.draw(i->second, states);
		n++;
	}
}

void ParsysView::onChange(Observable const *o) {
	CircleShape c;

	if (typeid(*o) == typeid(Parsys)) {
		if (mPviews != nullptr)
			delete mPviews;
		mPviews = new unordered_map<Particle, CircleShape>();

        for (auto i = mSys->cbegin(); i != mSys->cend(); i++) {
        	c = CircleShape(i->radius());
        	c.setPosition(i->x(), i->y());
        	c.setFillColor(particleColor(*i, mControlled));

        	mPviews->emplace(*i, c);
        }
	}

}


static Color particleColor(Particle const &p, Particle const &moving) {
	// If the controlled particle isn't found within the system, it means it
	// was absorbed into another, and hence the game is lost -- with all the
	// other particles gaining greater radius than it would have.
	if (p == moving) {
		return COLOR_MOVING;
	} else if (!moving.attached()) {
        return COLOR_GT;
    } else {
		return (p.radius() <= moving.radius()) ? COLOR_LT: COLOR_GT;
	}
}
