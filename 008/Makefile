build:
	g++ -w -std=c++14 -Wfatal-errors \
	-g ./src/*.cpp ./src/*.c \
	-o game \
	-lglfw \
	-lGL \
	-lGLU \
	-lGLEW;

test:
	g++ -w -std=c++14 -Wfatal-errors \
	-g ./src/*.cpp ./src/*.c \
	-o game \
	-lglfw \
	-lGL \
	-lGLU \
	-lGLEW \
	-lassimp;
	./game;

clean:
	rm ./game;
run:
	./game;
