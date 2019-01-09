#include "EventManager.hpp"


EventManager::~EventManager () {
}


CParticleManager::CParticleManager(weak_ptr<Particle> controlled) {
	mControlled = controlled;
}

void CParticleManager::manage(Event &e) {
	if (e.type == Event::KeyPressed && !mControlled.expired()) {
		switch (e.key.code) {
			case Keyboard::Numpad2:
			case Keyboard::S:
				mControlled.lock()->move(0, 3);
				break;
			case Keyboard::Numpad4:
			case Keyboard::A:
				mControlled.lock()->move(-3, 0);
				break;
			case Keyboard::Numpad6:
			case Keyboard::D:
				mControlled.lock()->move(3, 0);
				break;
			case Keyboard::Numpad8:
			case Keyboard::W:
				mControlled.lock()->move(0, -3);
				break;
			default:
				break;
		}
	}
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


ViewManager::ViewManager(
	RenderWindow &window, Parsys &system, weak_ptr<Particle> controlled
): mW{window}, mControlled{controlled}, mS{system} {
	mS.attach(this);
	centerMainView();
}

void ViewManager::manage(Event &e) {
}

void ViewManager::onChange(Observable const *o) {
	if (o == &mS) {
		centerMainView();
	}
}

void ViewManager::centerMainView() const {
	View v;
	shared_ptr<Particle> s;

	if (!mControlled.expired()) {
		s = mControlled.lock();

		v.setCenter(s->x(), s->y());
		v.setSize(mW.getSize().x, mW.getSize().y);
		mW.setView(v);
	}
}
