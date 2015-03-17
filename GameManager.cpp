#include "GameManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "LogManager.h"
#include "InputManager.h"

#include <stdlib.h>  //atoi, atof (actual include not needed?)

void GameManager::loadResources(std::string scope_text)
{
   resource_manager->loadResources(scope_text);
}

void GameManager::checkForInput(float time_step)
{
   input_manager->checkForInput(time_step);
}

void GameManager::keyPressed(std::string game_key)
{
    render_manager->keyPressed(game_key);
}

void GameManager::keyReleased(std::string game_key)
{
    render_manager->keyReleased(game_key);
}

void GameManager::mousePressed(uint32 mouse_x, uint32 mouse_y, uint32 game_mouse)
{
   cout << "mouse_x: " << mouse_x << endl;
   cout << "mouse_y: " << mouse_y << endl;
   cout << "mouse button: " << game_mouse << endl;
}

void GameManager::mouseReleased(uint32 mouse_x, uint32 mouse_y, uint32 game_mouse)
{
}

void GameManager::mouseMoved(uint32 mouse_x, uint32 mouse_y, int mouse_rel_x, int mouse_rel_y)
{
   render_manager->mouseMoved(mouse_x, mouse_y, mouse_rel_x, mouse_rel_y);
}

void GameManager::joystickButtonPressed(std::string button)
{
    render_manager->joystickButtonPressed(button);
}

void GameManager::joystickAxisMoved(double amount[])
{
   render_manager->joystickAxisMoved(amount);
}

void GameManager::joystickAxisMoved(std::string axis, int amount)
{
   render_manager->joystickAxisMoved(axis, amount);
}

void GameManager::logProblem(std::string problem_message, int line_number)
{
   log_manager->logProblem(problem_message, line_number);
}

void GameManager::logComment(std::string comment_message)
{
   log_manager->logComment(comment_message);
}

void GameManager::initialiseRenderResourceGroup(std::string group_name)
{
   render_manager->initialiseResourceGroup(group_name);
}

void GameManager::loadRenderResourceGroup(std::string group_name)
{
   render_manager->loadResourceGroup(group_name);
}

void GameManager::unloadRenderResourceGroup(std::string group_name)
{
   render_manager->unloadResourceGroup(group_name);
}

void GameManager::addPathResource(std::string file_name, std::string path_type, std::string group_name)
{
   render_manager->addPathResource(file_name, path_type, group_name);
}
      
void GameManager::addMeshResource(std::string file_name, std::string mesh_str, std::string group_name)
{
   render_manager->addMeshResource(file_name, mesh_str, group_name);
}

//the problem is that in C++, it is not known when this initialization code will be called
//so we will use a function static variable that is initialized the first time the method is called
GameManager* GameManager::getGameManager(std::string scene_file_name)
{
   static GameManager game_manager(scene_file_name);  //initialized when method is called the first time
   return &game_manager;  //won't go out of scope as game_manager is static
}

void GameManager::init(std::string scene_file_name)
{
   log_manager = new LogManager("log.txt");
   render_manager = new RenderManager(this);  //calls render manager's init, sets up the frame listener
   input_manager = new InputManager(this);

   resource_manager = new ResourceManager(this);
//cout << "LMAO" << endl;
   resource_manager->loadFromXMLFile("resources.xml");

   render_manager->buildSceneFromXML(scene_file_name);
}

GameManager::GameManager(std::string scene_file_name)
{
   if (scene_file_name == "")
   {
      log_manager->logComment("No scene file specified.");
      exit(0);
   }

   init(scene_file_name);

   render_manager->startRendering();
}

GameManager::~GameManager()
{
cout << "GameManager destructor called" << endl;
   resource_manager->unloadResources();

   delete resource_manager;
   delete input_manager;
   delete render_manager;
   delete log_manager;
}

size_t GameManager::getRenderWindowHandle()
{
   return render_manager->getRenderWindowHandle();
}

int GameManager::getRenderWindowWidth()
{
   return render_manager->getRenderWindowWidth();
}

int GameManager::getRenderWindowHeight()
{
   return render_manager->getRenderWindowHeight();
}

int GameManager::parseInt(std::string& str)
{
   return atoi(str.c_str());
}

float GameManager::parseFloat(std::string& str)
{
   return atof(str.c_str());
}

void GameManager::parseFloats(std::string& str, float* values)
{
   int sz = str.size();

   int loc = 0;
   int token_index = 0;
   while (loc < sz)
   {
      int index = str.find(',', loc);

      if (index == -1)
      {
         index = sz-1;
      }

      std::string sub = str.substr(loc, index);
      values[token_index] = parseFloat(sub);

      loc = index + 1;
      token_index++;
   }
}

std::string GameManager::textFromChildNode(TiXmlNode* parent_node, const char* child_element_name)
{
   TiXmlNode* child_node = parent_node->FirstChild(child_element_name);
   if (child_node)
   {
      TiXmlElement* child_node_element = (TiXmlElement*) child_node->ToElement();
      std::string child_node_text = child_node_element->GetText();
      return child_node_text;
   }
   else
   {
      return "";
   }
}
