#include <Andromeda/System/SWITCH/GameLoaderSwitch.h>
#include <Andromeda/Graphics/GL/RenderManagerGL3.h>
#include <Andromeda/Input/InputManager.h>
#include <Andromeda/Utils/Logger.h>

#include <iostream>

namespace Andromeda
{
	namespace System
	{
		GameLoaderSwitch::GameLoaderSwitch(GameManager* gameManager) : GameLoader(gameManager)
		{
			_renderManager = 0;
		}

		void GameLoaderSwitch::Run()
		{
			Utils::Logger::Instance()->Log("Start\n");
			
			//get render manager
			_renderManager = Graphics::RenderManager::Instance();
			
			if(!((Graphics::RenderManagerGL3*)_renderManager)->InitEgl(nwindowGetDefault()))
			{
				Utils::Logger::Instance()->Log("Failed to create gl window\n");
				return;
			}
			
			//set configuration 
			Utils::Logger::Instance()->Log("Config\n");
			_gameManager->Configure();
			
			Utils::Logger::Instance()->Log("Set window size\n");

			//get default monitor
			_renderManager->SetWindowSize(1280, 720);

			//set input manager
			Input::InputManager* intputManager = Input::InputManager::Instance();


			Utils::Logger::Instance()->Log("Init Render\n");			

			//create renderer			
			_renderManager->Init();
			
			
			Utils::Logger::Instance()->Log("Init Game Manager\n");

			//init game manager 
			_gameManager->Init();
			
			Utils::Logger::Instance()->Log("Start loop\n");

			//main loop
			while (_gameManager->Running() && appletMainLoop())
			{
				//update input manager
				//intputManager->Update();
				
				//Scan all the inputs. This should be done once for each frame
				hidScanInput();
				
				//u32 kDown = hidKeysDown(CONTROLLER_P1_AUTO);
				//if (kDown & KEY_DRIGHT)
				//	break;

				//game stuff
				_gameManager->HandleEvents();
				_gameManager->Update();
				_gameManager->Draw();

				//end frame
				_renderManager->SwapBuffers();
			}

			//clean game manager
			_gameManager->CleanUp();

			//finish render manager;
			_renderManager->Finish();

			//remove render manager
			delete _renderManager;

			//log exit
			Utils::Logger::Instance()->Log("Exit\n");
		}

	}
}