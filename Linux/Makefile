#the world's laziest makefile.
# DONT DO THIS

all: fftOcean

fftOcean: main.cpp FFT.cpp obj.cpp complex.cpp objects.cpp ocean.cpp random.cpp shader.cpp skybox.cpp vector.cpp
	g++ -w -std=c++11 -o fftOcean.exe *.cpp -lgsl -lgslcblas -lSOIL -lglut -lGL -lm -lGLEW
