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


void ParsysView::updateViews() {
	mMainView.setSize(mSize);
	mMapView.setCenter(mPosition.x + mSize.x / 2, mPosition.y + mSize.y / 2);
	mMapView.setSize(mSize);

	mMainBackground.setPosition(mPosition);
	mMainBackground.setSize(mSize);
	mMapBackground.setPosition(mPosition);
	mMapBackground.setSize(mSize);
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

ParsysView::ParsysView(Parsys &s, weak_ptr<Particle> const &controlled):
	mSys{s} {
	mControlled = controlled;
	mPviews = new unordered_map<Particle, CircleShape>();
	mSys.attach(this);
	
	mMapView.setViewport(FloatRect(0.0f, 0.0f, 0.22f, 0.25f));

	mMainBackground.setFillColor(COLOR_BACKGROUND);
	mMainBackground.setOutlineThickness(5);
	mMainBackground.setOutlineColor(COLOR_OUT_BACKGROUND);

	mMapBackground.setFillColor(COLOR_BACKGROUND_MAP);
	mMapBackground.setOutlineThickness(-10);
	mMapBackground.setOutlineColor(COLOR_OUT_BACKGROUND_MAP);

	updateViews();
	mSys.notify();
}

ParsysView::~ParsysView() {
	if (mPviews != nullptr)
		delete mPviews;
	mSys.detach(this);

	mControlled.reset();
}

void ParsysView::draw(RenderTarget& target, RenderStates states) const {
	mMainView.setSize(target.getSize().x, target.getSize().y);
	mMainView.zoom(pow(ZOOM_IN, mZoom));

	target.setView(mMainView);
	target.draw(mMainBackground);

	for (auto i = mPviews->cbegin(); i != mPviews->cend(); i++) {
		target.draw(i->second, states);
	}

	target.setView(mMapView);
	target.draw(mMapBackground);

	for (auto i = mPviews->cbegin(); i != mPviews->cend(); i++) {
		target.draw(i->second, states);
	}
}

void ParsysView::onChange(Observable const *o) {
	CircleShape c;

	if (typeid(*o) == typeid(Parsys)) {
		if (!mControlled.expired()) {
			mShared = mControlled.lock();
			mMainView.setCenter(
				mPosition.x + mShared->x(), mPosition.y + mShared->y()
			);
			mShared.reset();
		}

		// TO-DO Optimize the caching of view instances
		if (mPviews != nullptr)
			delete mPviews;
		mPviews = new unordered_map<Particle, CircleShape>();

        for (auto i = mSys.cbegin(); i != mSys.cend(); i++) {
        	c = CircleShape();
			// CircleShape (and any *Shape in general) deals with origin-wise
			// coordinates. In the case of CircleShape, setPosition() doesn't
			// look at the center coordinate, but at its top-left by default.
			c.setOrigin((*i)->radius(), (*i)->radius());
			c.setRadius((*i)->radius());
        	c.setPosition(mPosition.x + (*i)->x(), mPosition.y + (*i)->y());
			particleDecoration(c, **i, mControlled);

        	mPviews->emplace(**i, c);
        }
	}

}

void ParsysView::position(float x, float y) {
	mPosition.x = x;
	mPosition.y = y;
	updateViews();
}

Vector2f ParsysView::position() const {
	return mPosition;
}

void ParsysView::size(float width, float height) {
	if (width <= 0 || height <= 0)
		throw invalid_argument("Width/height can't be <= 0");

	mSize = Vector2f(width, height);
	updateViews();
}

Vector2f ParsysView::size() const {
	return mSize;
}

void ParsysView::zoomIn() {
	mZoom++;
}

void ParsysView::zoomOut() {
	mZoom--;
}

void ParsysView::zoomReset() {
	mZoom = 0;
}

short int ParsysView::zoom() const {
	return mZoom;
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
