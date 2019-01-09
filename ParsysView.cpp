#include <typeinfo>
#include "ParsysView.hpp"
#include "Observer.hpp"


/**
 * @return Color for a given particle with reference to the commanded one (i.e.
 * `moving').
 */
static Color particleColor(Particle const &p, weak_ptr<Particle> &moving);


static const Color COLOR_MOVING = Color::Cyan;
static const Color COLOR_GT = Color::Red;
static const Color COLOR_LT = Color::Blue;

ParsysView::ParsysView(Parsys &s, weak_ptr<Particle> const &controlled) {
	mSys = &s;
	mSys->attach(this);
	mSys->notify();
	mControlled = controlled;

	mPviews = new unordered_map<Particle, CircleShape>();
}

ParsysView::~ParsysView() {
	if (mPviews != nullptr)
		delete mPviews;
	mSys->detach(this);

	mControlled.reset();
}

void ParsysView::draw(RenderTarget& target, RenderStates states) const {
	for (auto i = mPviews->cbegin(); i != mPviews->cend(); i++) {
		target.draw(i->second, states);
	}
}

void ParsysView::onChange(Observable const *o) {
	shared_ptr<Particle> p;
	CircleShape c;

	if (typeid(*o) == typeid(Parsys)) {
		if (mPviews != nullptr)
			delete mPviews;
		mPviews = new unordered_map<Particle, CircleShape>();

        for (auto i = mSys->cbegin(); i != mSys->cend(); i++) {
        	p = *i;

        	c = CircleShape(p->radius());
        	c.setPosition(p->x(), p->y());
        	c.setFillColor(particleColor(*p, mControlled));

        	mPviews->emplace(*p, c);
        }
	}

}


static Color particleColor(Particle const &p, weak_ptr<Particle> &moving) {
	// If the controlled particle isn't found within the system, it means it
	// was absorbed into another, and hence the game is lost -- with all the
	// other particles gaining greater radius than it would have.
	if (moving.expired()) {
        return COLOR_GT;
    } else if (p == *moving.lock()) {
        return COLOR_MOVING;
	} else {
		return (p.radius() <= moving.lock()->radius()) ? COLOR_LT: COLOR_GT;
	}
}
