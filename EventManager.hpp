#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"

using namespace std;
using namespace sf;


class EventManager {
	public:
		~EventManager();
        void virtual manage(Event &e) = 0;
};

class CParticleManager: public EventManager {
	private:
		weak_ptr<Particle> mControlled;
	public:
		CParticleManager(weak_ptr<Particle> controlled);
		void manage(Event &e) override;
};

class WindowManager: public EventManager {
	private:
		RenderWindow &mW;
	public:
		WindowManager(RenderWindow &window);
		void manage(Event &e) override;
};


#endif
