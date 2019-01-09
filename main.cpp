#include <iostream>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"
#include "ParsysView.hpp"
#include "EventManager.hpp"

using namespace std;
using namespace sf;


int main (int argc, char *argv[]) {
	VideoMode const desktopMode = VideoMode::getDesktopMode();

	Parsys p = UnirandParsysGen(
		300, 0, max(desktopMode.width, desktopMode.height), 4
	).generate();
    weak_ptr<Particle> controlled = *p.cbegin();

	RenderWindow w(desktopMode, "Osmos");
	WindowManager wManager{w};
	CParticleManager cManager{controlled};
	Event e;

	ParsysView const v{p, controlled};

	// TO-DO Find out why there's the need to notify an additional time.
	p.notify();
	w.setFramerateLimit(60);

	cout << "Starting up with " << p.size() << " particles." << endl;

	while (w.isOpen()) {
		while (w.pollEvent(e)) {
			wManager.manage(e);
			cManager.manage(e);
		}

		w.clear();
		w.draw(v);
		w.display();
	}

	cout << "Closed with " << p.size() << " particles." << endl;

	return EXIT_SUCCESS;
}
