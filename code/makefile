all: Progetto Clean

Progetto: main.o Room.o Entity.o Wall.o Door.o Enemy.o Player.o Artifact.o Bullet.o Level.o Game.o
	gcc -o Progetto main.o Room.o Entity.o Wall.o Door.o Enemy.o Player.o Artifact.o Bullet.o Level.o Game.o -lncurses -lstdc++

Game.o: Game.cpp Game.hpp
	gcc -c Game.cpp Game.hpp

Level.o: Level.cpp Level.hpp
	gcc -c Level.cpp Level.hpp

Bullet.o: Bullet.cpp Bullet.hpp
	gcc -c Bullet.cpp Bullet.hpp

Artifact.o: Artifact.cpp Artifact.hpp
	gcc -c Artifact.cpp Artifact.hpp

Player.o: Player.cpp Player.hpp
	gcc -c Player.cpp Player.hpp

Enemy.o: Enemy.cpp Enemy.hpp
	gcc -c Enemy.cpp Enemy.hpp

Door.o: Door.cpp Door.hpp
	gcc -c Door.cpp Door.hpp

Wall.o: Wall.cpp Wall.hpp
	gcc -c Wall.cpp Wall.hpp

Entity.o: Entity.cpp Entity.hpp
	gcc -c Entity.cpp Entity.hpp
	
Room.o: Room.cpp Room.hpp
	gcc -c Room.cpp Room.hpp

main.o: main.cpp Level.hpp
	gcc -c main.cpp

Clean:
	rm -f *.o
	rm -f *.gch
	rm -f *~

