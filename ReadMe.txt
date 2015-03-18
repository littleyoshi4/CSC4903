#######
What you need to implement constant control-stick input:
Required files:
RenderManager.h
RenderManager.cpp
InputManager.h
InputManager.cpp
GameManager.h
GameManager.cpp

In RenderManager.h:

Under Private:
   These are just helper functions I wrote that are only required in my implementation of the camera controls
   It is possible to create something without using these.
   float balanceDegree(float);
   float balanceRadian(float);
   float balance(float,float);

   These are required.
   void updateCamera();
   void updateCamera(double*);
   void setControlStickInput(double*);
   bool constantInput; //Not always needed, this was just a switch for testing purposes.
   double controllerStickInput[6];

Under Public:
   void joystickAxisMoved(double*);

In RenderManager.cpp:
   Just copy all the definitions of the functions that you copied from RenderManager.h
   void RenderManager::updateCamera() is where all the magic happens, feel free to re-map the inputs to do whatever you want

In InputManager.cpp:
   I modified joystickButtonMap(int button) to include BACK, START, HOME, left stick click and right stick click.
   
   The definition of bool InputManager::axisMoved (const OIS::JoyStickEvent& e, int axis)
   It includes some notes regarding regarding stick mapping.

In GameManager:
   I just added void GameManager::joystickAxisMoved(double amount[]) to the class as an overload for 
   the original joystickAxisMoved(std::string axis, int amount)

That should be everything to get basic stick implementation working. 
There is also the capability to have button/keyboard inputs which I implemented. 
Those functions should be defined in GameManager and RenderManager

Good luck!
#######

My attempt at a procedural dungeon game (with some issues that still need to be worked out)
Controls:
   	Left Control Stick: Moves the player around.
	Right Control Stick: Moves the camera around.
	Start: Closes the game.
	
	WASD/Arrow Keys: Also move the player around.
	Mouse: Also moves the camera around.

Issues:
If this doesn't run, you'll probably have to change line 490 of RenderManager.cpp to the correct plugin for your system. 
Also the MakeFile was changed to match my system setup, you'll have to change it to match yours. 
There are several issues in the dungeon spawnRooms function which I will try to work out any issues with in the next few days. 
