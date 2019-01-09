#include <iostream>
#include <memory>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"
#include "ParsysView.hpp"

using namespace std;
using namespace sf;


int main (int argc, char *argv[]) {
	VideoMode const desktopMode = VideoMode::getDesktopMode();

	Parsys p = UnirandParsysGen(
		300, 0, max(desktopMode.width, desktopMode.height), 4
	).generate();
    weak_ptr<Particle> controlled = *p.cbegin();

	RenderWindow w(desktopMode, "Osmos");
	ParsysView const v{p, controlled};
	Event e;

	// TO-DO Find out why there's the need to notify an additional time.
	p.notify();
	w.setFramerateLimit(60);

	cout << "Starting up with " << p.size() << " particles." << endl;

	while (w.isOpen()) {
		while (w.pollEvent(e)) {
			switch (e.type) {
				case Event::Closed:
					w.close();
					break;
				case Event::KeyPressed:
					if (controlled.expired())
						break;

					switch (e.key.code) {
						case Keyboard::Numpad2:
						case Keyboard::S:
							controlled.lock()->move(0, 3);
							break;
						case Keyboard::Numpad4:
						case Keyboard::A:
							controlled.lock()->move(-3, 0);
							break;
						case Keyboard::Numpad6:
						case Keyboard::D:
							controlled.lock()->move(3, 0);
							break;
						case Keyboard::Numpad8:
						case Keyboard::W:
							controlled.lock()->move(0, -3);
							break;
						default:
							break;
					}
				default:
					break;
			}
		}

		w.clear();
		w.draw(v);
		w.display();
	}

	cout << "Closed with " << p.size() << " particles." << endl;

	return EXIT_SUCCESS;
}
