CC = g++
CFLAGS = -g -Wall -std=c++11
LIBS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lmpg123

default: beat-dodge

beat-dodge: main.o WindowManager.o SoundManager.o GameManager.o PlayState.o
	$(CC) $(CFLAGS) -o beat-dodge main.o WindowManager.o SoundManager.o GameManager.o PlayState.o $(LIBS)

main.o: main.cpp WindowManager.h SoundManager.h GameManager.h PlayState.h
	$(CC) $(CFLAGS) -c main.cpp

WindowManager.o: WindowManager.cpp WindowManager.h
	$(CC) $(CFLAGS) -c WindowManager.cpp

SoundManager.o: SoundManager.cpp SoundManager.h
	$(CC) $(CFLAGS) -c SoundManager.cpp

GameManager.o: GameManager.cpp GameManager.h GameState.h
	$(CC) $(CFLAGS) -c GameManager.cpp

PlayState.o: PlayState.cpp PlayState.h GameState.h WindowManager.h SoundManager.h GameManager.h
	$(CC) $(CFLAGS) -c PlayState.cpp