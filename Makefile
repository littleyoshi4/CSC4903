﻿AutomatedMakefile = am
CC = g++ 

GAME_PATH=/home/daniel/Downloads

INC_DIRS=-I./ -I$(GAME_PATH)/ogre_src_v1-8-1/OgreMain/include/ -I$(GAME_PATH)/ogre_src_v1-8-1/include/ -I$(GAME_PATH)/boost_1_57_0/ -I$(GAME_PATH)/tinyxml -I$(GAME_PATH)/ogre-deps/src/ois/includes -I$(GAME_PATH)/Object-oriented-Input-System--OIS- -I$(GAME_PATH)/Object-oriented-Input-System--OIS-/includes -I$(GAME_PATH)
LIB_DIRS=-L./ -L$(GAME_PATH)/Ogre_1.8.1/lib/ -L$(GAME_PATH)/boost_1.57.0/stage/lib/ -L$(GAME_PATH)/tinyxml_2.6.2 -L$(GAME_PATH)/ogre-deps/src/ois -L$(GAME_PATH)/Object-oriented-Input-System--OIS-/includes -L$(GAME_PATH)/Object-oriented-Input-System--OIS- -L$(GAME_PATH)/bass_2.4 -L$(GAME_PATH) -L$(GAME_PATH)/tinyxml
LIBS=-lboost_system-mt -lOgreMain tinyxml.a -lOIS 
#-lbass

COMPILE = $(CC) $(INC_DIRS) -c 
LINK = $(CC) $(LIB_DIRS) -o

GAME_FILES = Main.o GameManager.o RenderListener.o InputRenderListener.o AnimationRenderListener.o RenderManager.o LogManager.o GameResource.o PathResource.o MeshResource.o ResourceManager.o InputManager.o
QUAT_FILES = Quaternion.o

all:	Ogre

Ogre:   		$(GAME_FILES)
			$(LINK) Game.exe $(GAME_FILES) $(LIBS)

Quaternion:		$(QUAT_FILES)
			$(LINK) Quaternion.exe $(QUAT_FILES)

Main.o:			Main.cpp
			$(COMPILE) Main.cpp

GameManager.o:		GameManager.h GameManager.cpp
			$(COMPILE) GameManager.cpp

RenderManager.o:	RenderManager.h RenderManager.cpp
			$(COMPILE) RenderManager.cpp

RenderListener.o:	RenderListener.h RenderListener.cpp
			$(COMPILE) RenderListener.cpp

AnimationRenderListener.o:	AnimationRenderListener.h AnimationRenderListener.cpp
				$(COMPILE) AnimationRenderListener.cpp

InputRenderListener.o:		InputRenderListener.h InputRenderListener.cpp
				$(COMPILE) InputRenderListener.cpp

LogManager.o:		LogManager.h LogManager.cpp
			$(COMPILE) LogManager.cpp

Quaternion.o:		Quaternion.h Quaternion.cpp
			$(COMPILE) Quaternion.cpp

GameResource.o:		GameResource.h GameResource.cpp
			$(COMPILE) GameResource.cpp

PathResource.o:		PathResource.h PathResource.cpp
			$(COMPILE) PathResource.cpp

MeshResource.o:		MeshResource.h MeshResource.cpp
			$(COMPILE) MeshResource.cpp

ResourceManager.o:	ResourceManager.h ResourceManager.cpp
			$(COMPILE) ResourceManager.cpp

InputManager.o:		InputManager.h InputManager.cpp InputListener.h
			$(COMPILE) InputManager.cpp

AudioResource.o:	AudioResource.h AudioResource.cpp
			$(COMPILE) AudioResource.cpp

AudioPlayer.o:		AudioPlayer.h AudioPlayer.cpp
			$(COMPILE) AudioPlayer.cpp

AudioManager.o:		AudioManager.h AudioManager.cpp
			$(COMPILE) AudioManager.cpp

clean:	$(rm *.o) 
	$(rm *.gch)

