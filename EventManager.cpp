#include "EventManager.hpp"


EventManager::~EventManager () {
}


CParticleManager::CParticleManager(
	Vector2f bounds, weak_ptr<Particle> controlled
) {
	mBounds = bounds;
	mControlled = controlled;
}

void CParticleManager::manage(Event &e) {
	if (e.type == Event::KeyPressed && !mControlled.expired()) {
		mCopy = *mControlled.lock();

		switch (e.key.code) {
			case Keyboard::Numpad2:
			case Keyboard::S:
				mCopy.move(0, STEP_SIZE);

				if (withinBounds(mCopy))
					mControlled.lock()->move(0, STEP_SIZE);

				break;
			case Keyboard::Numpad4:
			case Keyboard::A:
				mCopy.move(-STEP_SIZE, 0);

				if (withinBounds(mCopy))
					mControlled.lock()->move(-STEP_SIZE, 0);

				break;
			case Keyboard::Numpad6:
			case Keyboard::D:
				mCopy.move(STEP_SIZE, 0);

				if (withinBounds(mCopy))
					mControlled.lock()->move(STEP_SIZE, 0);

				break;
			case Keyboard::Numpad8:
			case Keyboard::W:
				mCopy.move(0, -STEP_SIZE);

				if (withinBounds(mCopy))
					mControlled.lock()->move(0, -STEP_SIZE);

				break;
			default:
				break;
		}
	}
}

bool CParticleManager::withinBounds(Particle p) const {
	return
		mBounds.x < (p.x() - p.radius()) &&
		(p.x() + p.radius()) < mBounds.y &&
		mBounds.x < (p.y() - p.radius()) &&
		(p.y() + p.radius()) < mBounds.y;
}


WindowManager::WindowManager(RenderWindow &window): mW{window} {
}

void WindowManager::manage(Event &e) {
    switch (e.type) {
    	case Event::Closed:
    		mW.close();
    		break;
    	default:
    		break;
    }
}


ViewManager::ViewManager(RenderWindow &window, ParsysView &sysview):
	mW{window}, mSysview{sysview} {
}

void ViewManager::manage(Event &e) {
	if (e.type == Event::KeyPressed) {
		if (e.key.code == Keyboard::Add && mSysview.zoom() < mMaxZoom) {
			mSysview.zoomIn();
		} else if (
				e.key.code == Keyboard::Subtract && mMinZoom < mSysview.zoom()
		) {
			mSysview.zoomOut();
		} else if (e.key.code == Keyboard::Equal) {
			mSysview.zoomReset();
		}
	}
}
