#include "InputManager.h"
#include "GameManager.h"

#include <sstream>
#include <iostream>

using namespace std;

//called on each and every frame!
void InputManager::checkForInput(float time_step)
{
/*
   OIS::MouseState mouse_state = mouse_ois->getMouseState();
   bool left_mouse_button_down = mouse_state.buttonDown(OIS::MB_Left);
   //check unbuffered input (i.e. holding the left mouse button down)
   //generates a bunch of events even for a quick mouse press and release
   if (left_mouse_button_down)
   {
      OIS::MouseEvent e(mouse_ois, mouse_state);
      mousePressed(e, OIS::MB_Left);
   }
*/ 
   //check buffered input
   if (mouse_ois) mouse_ois->capture();
   if (keyboard_ois) keyboard_ois->capture();
   if (joystick_ois) joystick_ois->capture();
}

//callback
bool InputManager::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
   e.state.width = window_width;
   e.state.height = window_height;

   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();

      uint32 game_mouse = mouseMap(id);
      uint32 x_loc = e.state.X.abs;
      uint32 y_loc = e.state.Y.abs;
      listener->mousePressed(x_loc, y_loc, game_mouse);  //absolute location of mouse when pressed and which button was pressed
   }
   delete iter;

   return true;
}

//callback
bool InputManager::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID id)
{
   e.state.width = window_width;
   e.state.height = window_height;

   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();

      uint32 game_mouse = mouseMap(id);
      uint32 x_loc = e.state.X.abs;
      uint32 y_loc = e.state.Y.abs;
      listener->mouseReleased(x_loc, y_loc, game_mouse);  //absolute location of mouse when released and which button was released
   }
   delete iter;

   return true;
}

//callback
bool InputManager::mouseMoved(const OIS::MouseEvent& e)
{
   e.state.width = window_width;
   e.state.height = window_height;

   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();
      uint32 x_loc = e.state.X.abs;
      uint32 y_loc = e.state.Y.abs;
      int x_rel = (int) e.state.X.rel;
      int y_rel = (int) e.state.Y.rel;
      listener->mouseMoved(x_loc, y_loc, x_rel, y_rel);  //current location of the mouse (absolute coords and relative coords)
   }
   delete iter;

   return true;
}

//callback
bool InputManager::keyPressed(const OIS::KeyEvent& e)
{
   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();

   while(iter->hasNext())
   {
      InputListener* listener = iter->next();
      listener->keyPressed(keyMap(e));
   }
   delete iter;

   return true;
}

//callback
bool InputManager::keyReleased(const OIS::KeyEvent& e)
{
   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();
      listener->keyReleased(keyMap(e));
   }
   delete iter;

   return true;
}

InputManager::InputManager(GameManager* game_manager)
{
   input_manager_ois = NULL;
   keyboard_ois = NULL;
   mouse_ois = NULL;
   joystick_ois = NULL;

   input_listeners = new ListArray<InputListener>();

   init(game_manager);

   addListener(game_manager);

   window_width = game_manager->getRenderWindowWidth();
   window_height = game_manager->getRenderWindowHeight();
}

InputManager::~InputManager()
{
   free();

   input_manager_ois = NULL;
   keyboard_ois = NULL;
   mouse_ois = NULL;
   joystick_ois = NULL;

   //the individual input listeners are not deleted here, just the ListArray
   delete input_listeners;
}

void InputManager::addListener(InputListener* listener)
{
   input_listeners->add(listener);
}

void InputManager::free()
{
   if (input_manager_ois)
   {
      input_manager_ois->destroyInputSystem(input_manager_ois);
   }
}

void InputManager::init(GameManager* game_manager)
{
   try
   { 
      OIS::ParamList p1;
      std::ostringstream windowHndStr;
      size_t window_handle = game_manager->getRenderWindowHandle();

      size_t handle = window_handle;
      windowHndStr << handle;
      p1.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));
      input_manager_ois = OIS::InputManager::createInputSystem(p1);
      
      if (input_manager_ois->getNumberOfDevices(OIS::OISKeyboard) > 0)
      {
         keyboard_ois = static_cast<OIS::Keyboard*>(input_manager_ois->createInputObject(OIS::OISKeyboard, true));
         keyboard_ois->setEventCallback(this);  //forwards the information to all registered listeners
      }

      if (input_manager_ois->getNumberOfDevices(OIS::OISMouse) > 0)
      {
         mouse_ois = static_cast<OIS::Mouse*>(input_manager_ois->createInputObject(OIS::OISMouse, true));
         mouse_ois->setEventCallback(this);  //forwards the information to all registered listeners
      }

      if (input_manager_ois->getNumberOfDevices(OIS::OISJoyStick) > 0)
      {
         joystick_ois = static_cast<OIS::JoyStick*>(input_manager_ois->createInputObject(OIS::OISJoyStick, true));
         joystick_ois->setEventCallback(this);  //forwards the information to all registered listeners
      }
   }

   catch(std::exception& e)  //may be possible to get a good error message from ois this way
   {
      game_manager->logComment(e.what());
      ASSERT(false);
   }

   catch(...)  //catch any other type of exception thrown by OIS, won't be able to access the actual error message
   {
      game_manager->logComment("Input Manager Initialization Error");
      ASSERT(false);
   }
}

std::string InputManager::keyMap(const OIS::KeyEvent& e)
{
   std::string game_key = "INVALID_KEY";
   OIS::KeyCode key_code = e.key;

   if (key_code == OIS::KC_ESCAPE)
   {
      game_key = "ESCAPE";
   }

   else if (key_code == OIS::KC_RIGHT)
   {
      game_key = "RIGHT";
   }
   else if (key_code == OIS::KC_LEFT)
   {
      game_key = "LEFT";
   }
   else if (key_code == OIS::KC_UP)
   {
      game_key = "UP";
   }
   else if (key_code == OIS::KC_DOWN)
   {
      game_key = "DOWN";
   }

   else if (key_code == OIS::KC_W)
   {
      game_key = "W";
   }
   else if (key_code == OIS::KC_A)
   {
      game_key = "A";
   }
   else if (key_code == OIS::KC_S)
   {
      game_key = "S";
   }
   else if (key_code == OIS::KC_D)
   {
      game_key = "D";
   }

   return game_key;
}

uint32 InputManager::mouseMap(const OIS::MouseButtonID id)
{
   uint32 game_mouse = 0;
   uint32 mouse_code = id;

   if (mouse_code == OIS::MB_Left)
   {
      game_mouse = 1;
   }
   else if (mouse_code == OIS::MB_Right)
   {
      game_mouse = 2;
   }

   return game_mouse;
}

std::string InputManager::joystickButtonMap(int button)
{
   if (button == 0)
   {
      return "A";
   }

   else if (button == 1)
   {
      return "B";
   }

   else if (button == 2)
   {
      return "X";
   }

   else if (button == 3)
   {
      return "Y";
   }

   else if (button == 4)
   {
      return "L";
   }

   else if (button == 5)
   {
      return "R";
   }

   else if (button == 6)
   {
       return "BACK";
   }

   else if (button == 7)
   {
      return "START";
   }

   else if (button == 8)
   {
       return "HOME"; //The center button on the controller
   }

   else if (button == 9)
   {
      return "LS_CLICK";
   }

   else if (button == 10)
   {
      return "RS_CLICK";
   }

   else
   {
       return "Unknown";
   }
}

bool InputManager::buttonPressed (const OIS::JoyStickEvent& e, int button)
{
   //notify each of the listeners
   ListArrayIterator<InputListener>* iter = input_listeners->iterator();
   while(iter->hasNext())
   {
      InputListener* listener = iter->next();
      listener->joystickButtonPressed(joystickButtonMap(button));
   }
   delete iter;

   return true;
}
bool InputManager::buttonReleased (const OIS::JoyStickEvent& e, int button){return true;}


/*
std::string InputManager::joystickAxisMap(int axis)
{
    if (axis == 0)
    {
	return "L_NS";
    }

    else if (axis == 1)
    {
	return "L_EW";
    }

    else if (axis == 2)
    {
	return "R_NS";
    }

    else if (axis == 3)
    {
	return "R_EW";
    }
    else if (axis == 4)
    {
	return "L_TR";
    }
    else if (axis == 5)
    {
	return "R_TR";
    }
}
*/

bool InputManager::axisMoved (const OIS::JoyStickEvent& e, int axis)
{
    //axis 1,0 = left stick
    //axis 4,3 = right stick
    //axis 2 = left trigger
    //axis 5 = right trigger

   
    double amount[6] = {e.state.mAxes[1].abs,e.state.mAxes[0].abs,  // left stick north/south, east/west
		     e.state.mAxes[4].abs,e.state.mAxes[3].abs,  // right stick north/south, east/west
		     e.state.mAxes[2].abs,e.state.mAxes[5].abs}; // left trigger and right trigger

    /* 
    cout << amount[0] << " " << amount[1] << "\n" 
	 << amount[2] << " " << amount[3] << "\n"
	 << amount[4] << " " << amount[5] << endl;
    */

    //dead zone guess
    //if (abs(amount[axis]) < 10000) return true;
    
    //I added this bit which allows for multiple axis manipulations within a single frame
    //however it does seem as though it will be a bit slow in the future so I'll have to work on it a bit
    //but this is a temporary fix for now. 
    
    //Notify each of the listeners
    ListArrayIterator<InputListener>* iter = input_listeners->iterator();
    while(iter->hasNext())
    {
	InputListener* listener = iter->next();
	listener->joystickAxisMoved(amount);
    }
    delete iter;	
    return true;
}

bool InputManager::sliderMoved (const OIS::JoyStickEvent& e, int index){return true;}
bool InputManager::povMoved (const OIS::JoyStickEvent& e, int index){return true;}
bool InputManager::vector3Moved (const OIS::JoyStickEvent& e, int index){return true;}
