#include <iostream>
#include <SFML/Graphics.hpp>
#include "Parsys.hpp"
#include "ParsysView.hpp"

using namespace std;
using namespace sf;


int main (int argc, char *argv[]) {
	Parsys p = UnirandParsysGen(200, 0, 400, 2).generate();
	Particle controlled{p, 0, 0, 1};

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
				case (Event::Closed):
					w.close();
					break;
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
