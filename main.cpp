#include <iostream>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"
#include "ParsysView.hpp"

using namespace std;
using namespace sf;


int main (int argc, char *argv[]) {
	Parsys p = UnirandParsysGen(200, 0, 400, 2).generate();
	Particle controlled{p, 0, 0, 1.5};

	RenderWindow w(VideoMode(640, 400), "Osmos");
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
					switch (e.key.code) {
						case Keyboard::Numpad2:
						case Keyboard::S:
                            controlled.shift(0, 0.5);
							break;
						case Keyboard::Numpad4:
						case Keyboard::A:
							controlled.shift(-0.5, 0);
							break;
						case Keyboard::Numpad6:
						case Keyboard::D:
							controlled.shift(0.5, 0);
							break;
						case Keyboard::Numpad8:
						case Keyboard::W:
							controlled.shift(0, -0.5);
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

	return EXIT_SUCCESS;
}
