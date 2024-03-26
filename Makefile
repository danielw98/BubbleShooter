build:
	-mkdir obj 2> /dev/null
	g++ -c -o obj/Game.o src/Game.cpp
	g++ -c -o obj/Entity.o src/Entity.cpp
	g++ -c -o obj/Bubble.o src/Bubble.cpp
	g++ -c -o obj/GUIManager.o src/GUIManager.cpp
	g++ -c -o obj/FontRenderer.o src/FontRenderer.cpp
	g++ -c -o obj/TextureManager.o src/TextureManager.cpp
	g++ -c -o obj/LevelManager.o src/LevelManager.cpp
	g++ -c -o obj/SoundManager.o src/SoundManager.cpp
	g++ -c -o obj/MatchFinder.o src/MatchFinder.cpp
	g++ -c -o obj/main.o main.cpp
	g++ -o BubbleShooter obj/Game.o obj/Entity.o obj/Bubble.o obj/GUIManager.o obj/FontRenderer.o obj/TextureManager.o obj/LevelManager.o obj/SoundManager.o obj/MatchFinder.o obj/main.o -lSDL2 -lSDL2_image -lSDL2_ttf -g -Wall -Wextra -pedantic

clean:
	-rm -r obj 2> /dev/null
	-rm BubbleShooter 2> /dev/null
