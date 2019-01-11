#ifndef PARTICLE_GRID_H
#define PARTICLE_GRID_H

#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"

using namespace sf;
using namespace std;


class ParsysView: public Drawable, public Observer {
	private:
		Parsys &mSys;
		unordered_map<Particle, CircleShape> *mPviews{nullptr};

		weak_ptr<Particle> mControlled;
		// Variable mainly introduced for optimization.
		shared_ptr<Particle> mShared;

		View mutable mMainView, mMapView;
		Vector2f mPosition{0, 0};
		Vector2f mSize{512, 512};
		short int mZoom{0};

		RectangleShape mMainBackground, mMapBackground;

		void updateViews();
	public:
		static constexpr float ZOOM_IN = 10 / 11.0;
		static constexpr float ZOOM_OUT = 11 / 10.0;

		static const Color COLOR_BACKGROUND;
		static const Color COLOR_OUT_BACKGROUND;
		static const Color COLOR_BACKGROUND_MAP;
		static const Color COLOR_OUT_BACKGROUND_MAP;

		static const Color COLOR_MOVING;
		static const Color COLOR_OUT_MOVING;
		static const Color COLOR_GT;
		static const Color COLOR_OUT_GT;
		static const Color COLOR_LT;
		static const Color COLOR_OUT_LT;

		ParsysView(Parsys &s, weak_ptr<Particle> const &controlled);
		~ParsysView();

		void draw(RenderTarget& target, RenderStates states) const override;
		void onChange(Observable const *o) override;

		// TO-DO Shall I use the `set/get' prefix as in the SFML API?
		void position(float x, float y);
		Vector2f position() const;
		void size(float width, float height);
		Vector2f size() const;

		void zoomIn();
		void zoomOut();
		void zoomReset();
		short int zoom() const;
};


#endif
