cc=g++
flags=-Wall
libs=-lsfml-graphics -lsfml-window -lsfml-system


.PHONY: clear


ParticleGrid.o: ParticleGrid.cpp ParticleGrid.hpp
	$(cc) $(flags) $(libs) -c $(filter %.cpp, $^)

ParticleGraph.o: ParticleGraph.cpp ParticleGraph.hpp
	$(cc) $(flags) -c $(filter %.cpp, $^)

test.out: test-main.o test-particle-graph.o ParticleGraph.o
	$(cc) $(flags) -o test.out $(filter %.cpp %.o, $^)

test-main.o: test/test-main.cpp
	$(cc) $(flags) -c $(filter %.cpp, $^)

test-particle-graph.o: test/test-particle-graph.cpp
	$(cc) $(flags) -c $(filter %.cpp, $^)


clear:
	rm -f *.out *.o
