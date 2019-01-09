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
