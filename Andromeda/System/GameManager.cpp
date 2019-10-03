#include <Andromeda/System/GameManager.h>
#include <Andromeda/System/GameState.h>
#include <Andromeda/Utils/SimpleProfiler.h>

namespace Andromeda
{
	namespace System
	{
		GameManager::GameManager()
		{
			_running = true;
		}

		GameManager::~GameManager()
		{
		}

		void GameManager::ChangeState(GameState* state)
		{
			// cleanup the current state
			if (!_states.empty())
			{
				_states.back()->CleanUp();
				_states.pop_back();
			}

			// store and init the new state
			_states.push_back(state);
			_states.back()->Init();
		}

		void GameManager::PushState(GameState* state)
		{
			// pause current state
			if (!_states.empty())
			{
				_states.back()->Pause();
			}

			// store and init the new state
			_states.push_back(state);
			_states.back()->Init();
		}

		void GameManager::PopState()
		{
			// cleanup the current state
			if (!_states.empty())
			{
				_states.back()->CleanUp();
				_states.pop_back();
			}

			// resume previous state
			if (!_states.empty())
			{
				_states.back()->Resume();
			}
		}

		void GameManager::GamePause()
		{
			_states.back()->GamePause();
		}

		void GameManager::GameResume()
		{
			_states.back()->GameResume();
		}

		void GameManager::HandleEvents()
		{
			Utils::SimpleProfiler::Instance()->BeginSection("HandleEvents");
			{
				_states.back()->HandleEvents(this);
			}
			Utils::SimpleProfiler::Instance()->EndSection();
		}

		void GameManager::Update()
		{
			Utils::SimpleProfiler::Instance()->BeginSection("Update");
			{
				_states.back()->Update(this);
			}
			Utils::SimpleProfiler::Instance()->EndSection();
		}

		void GameManager::Draw()
		{
			Utils::SimpleProfiler::Instance()->BeginSection("Draw");
			{
				_states.back()->Draw(this);
			}
			Utils::SimpleProfiler::Instance()->EndSection();
		}
	}
}

