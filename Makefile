all: compile link

compile:
	g++ -Iinclude -c main.cpp src/TileMap.cpp src/Player.cpp src/LetterBox.cpp

link:
	g++ main.o TileMap.o Player.o LetterBox.o -o main -Llib -lsfml-graphics -lsfml-window -lsfml-system
