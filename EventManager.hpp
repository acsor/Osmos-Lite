#ifndef EVENT_MANAGER_HPP
#define EVENT_MANAGER_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"
#include "ParsysView.hpp"

using namespace std;
using namespace sf;


class EventManager {
	public:
		~EventManager();
        void virtual manage(Event &e) = 0;
};


class CParticleManager: public EventManager {
	private:
		Vector2f mBounds;
		weak_ptr<Particle> mControlled;
		// Class variable only for performance's sake
		Particle mCopy{0, 0, 1};

		bool withinBounds(Particle p) const;

		static constexpr float STEP_SIZE = 10;
	public:
		CParticleManager(Vector2f bounds, weak_ptr<Particle> controlled);
		void manage(Event &e) override;
};

class WindowManager: public EventManager {
	private:
		RenderWindow &mW;
	public:
		WindowManager(RenderWindow &window);
		void manage(Event &e) override;
};

class ViewManager: public EventManager {
	private:
		RenderWindow &mW;
		ParsysView &mSysview;
	public:
		short int mMaxZoom{8}, mMinZoom{-5};

		ViewManager(RenderWindow &window, ParsysView &sysview);
		void manage(Event &e) override;
};


#endif
