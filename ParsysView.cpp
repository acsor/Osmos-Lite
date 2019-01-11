#include <cmath>
#include <valarray>
#include "ParsysView.hpp"
#include "Observer.hpp"


/**
 * @brief Stylizes a given particle according to its role in the game (whether
 * smaller and hence to be absorbed, bigger and thus a menace, or the moving
 * particle).
 */
static void particleDecoration(
	CircleShape &target, Particle const &p, weak_ptr<Particle> &moving
);


static const Color COLOR_MOVING = Color::Cyan;
static const Color COLOR_GT = Color::Red;
static const Color COLOR_LT = Color::Blue;

ParsysView::ParsysView(Parsys &s, weak_ptr<Particle> const &controlled) {
	mSys = &s;
	mSys->attach(this);
	mSys->notify();
	mControlled = controlled;
}

const Color ParsysView::COLOR_BACKGROUND = Color(0, 22, 107);
const Color ParsysView::COLOR_OUT_BACKGROUND = Color(0, 24, 92);
const Color ParsysView::COLOR_BACKGROUND_MAP = Color(0, 15, 71);
const Color ParsysView::COLOR_OUT_BACKGROUND_MAP = Color(0, 62, 86);

const Color ParsysView::COLOR_MOVING = Color::Cyan;
const Color ParsysView::COLOR_OUT_MOVING = Color(0, 179, 181);
const Color ParsysView::COLOR_GT = Color(202, 0, 44);
const Color ParsysView::COLOR_OUT_GT = Color(145, 0, 20);
const Color ParsysView::COLOR_LT = Color(0, 40, 151);
const Color ParsysView::COLOR_OUT_LT = Color(0, 4, 98);
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

static void particleDecoration(
	CircleShape &target, Particle const &p, weak_ptr<Particle> &moving
) {
	target.setOutlineThickness(-1.5);

	// If the controlled particle isn't found within the system, it means it
	// was absorbed into another, and hence the game is lost -- with all the
	// other particles gaining greater radius than it would have.
	if (moving.expired()) {
		target.setFillColor(ParsysView::COLOR_GT);
		target.setOutlineColor(ParsysView::COLOR_OUT_GT);
    } else if (p == *moving.lock()) {
		target.setFillColor(ParsysView::COLOR_MOVING);
		target.setOutlineColor(ParsysView::COLOR_OUT_MOVING);
	} else if (p.radius() < moving.lock()->radius()) {
		target.setFillColor(ParsysView::COLOR_LT);
		target.setOutlineColor(ParsysView::COLOR_OUT_LT);
	} else {
		target.setFillColor(ParsysView::COLOR_GT);
		target.setOutlineColor(ParsysView::COLOR_OUT_GT);
	}
}
