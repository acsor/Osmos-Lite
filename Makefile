cc=g++
flags=-Wall


.PHONY: clear


ParticleGrid.o: ParticleGrid.cpp ParticleGrid.hpp
	$(cc) $(flags) $(libs) -c $(filter %.cpp, $^)


clear:
	rm -f *.out *.o
