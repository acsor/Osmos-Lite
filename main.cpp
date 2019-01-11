#include <iostream>
#include <memory>
#include <cmath>
#include <valarray>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"
#include "ParsysView.hpp"
#include "EventManager.hpp"

using namespace std;
using namespace sf;


int main (int argc, char *argv[]) {
	VideoMode const desktopMode = VideoMode::getDesktopMode();
	RenderWindow w{desktopMode, "Osmos"};
	valarray<float> gameBounds{0, 1000};

	Parsys p = UnirandParsysGen(
		200, gameBounds[0], gameBounds[1], 4
	).generate();
    weak_ptr<Particle> controlled = *p.cbegin();

	ParsysView v{p, controlled};
	v.position(0, 0);
	v.size(
		abs(gameBounds[0] - gameBounds[1]), abs(gameBounds[0] - gameBounds[1])
	);

	WindowManager wManager{w};
	CParticleManager cManager{
		Vector2f{gameBounds[0], gameBounds[1]}, controlled
	};
	ViewManager vManager{w, v};
	Event e;

	w.setFramerateLimit(60);

	cout << "Starting up with " << p.size() << " particles." << endl;

	while (w.isOpen()) {
		while (w.pollEvent(e)) {
			wManager.manage(e);
			cManager.manage(e);
			vManager.manage(e);
		}

		w.clear(Color(0, 15, 59));
		w.draw(v);
		w.display();
	}

	cout << "Closing with " << p.size() << " particles." << endl;

	return EXIT_SUCCESS;
}
