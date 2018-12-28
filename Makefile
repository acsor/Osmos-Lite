cc=g++
flags=-Wall
libs=-lsfml-graphics -lsfml-window -lsfml-system


.PHONY: clear


ParticleGrid.o: ParticleGrid.cpp ParticleGrid.hpp
	$(cc) $(flags) $(libs) -c $(filter %.cpp, $^)

Parsys.o: Parsys.cpp Parsys.hpp
	$(cc) $(flags) -c $(filter %.cpp, $^)

ParsysView.o: ParsysView.cpp ParsysView.hpp
	$(cc) $(flags) -c $(filter %.cpp, $^)

Observer.o: Observer.hpp Observer.cpp
	$(cc) $(flags) -c  $(filter %.cpp, $^)

test.out: test-main.o test-particle-system.o Parsys.o Observer.o
	$(cc) $(flags) -o test.out $(filter %.cpp %.o, $^)

test-main.o: test/test-main.cpp
	$(cc) $(flags) -c $(filter %.cpp, $^)

test-particle-system.o: test/test-particle-system.cpp
	$(cc) $(flags) -c $(filter %.cpp, $^)


clear:
	rm -f *.out *.o
