#include "GameStateManager.h"
#include "StateTransition.h"

GameStateManager::GameStateManager()
{

}
 
GameStateManager::~GameStateManager() 
{
    for (auto state : m_stateStack)
    {
        state->Unload();
        delete state;
    }
    m_stateStack.clear();

    if (m_transition != nullptr)
    {
        delete m_transition;
    }
}

void GameStateManager::Update(float deltaTime)
{
    //Invoke chached commands
    for (auto cmd : m_commands)
    {
        cmd();
    }
    m_commands.clear();

    //Run Update
    for (auto state : m_stateStack)
    {
        state->Update(deltaTime);
    }

    if (m_transition != nullptr)
    {
        m_transition->Update(deltaTime);

        //Delete transition if finished.
        if (m_transition->IsComplete())
        {
            delete m_transition;
            m_transition = nullptr;
        }
    }
}

void GameStateManager::Draw()
{
    for (auto state : m_stateStack)
    {
        state->Draw();
    }

    if (m_transition != nullptr)
    {
        m_transition->Draw();
    }
}

void GameStateManager::EndDraw()
{
    for (auto state : m_stateStack)
    {
        state->EndDraw();
    }
}

void GameStateManager::SetState(const char* name, IGameState* state)
{
    m_commands.push_back([=]() {

        if (m_states[name] != nullptr)
        {
            m_states[name]->Unload();
            delete m_states[name];
        }

        m_states[name] = state;

        if (m_states[name] != nullptr)
        {
            m_states[name]->Load();
        }
    });
}

IGameState* GameStateManager::GetState(const char* name)
{
    return m_states[name];
}

void GameStateManager::PushState(const char* name)
{
    m_commands.push_back([=]() {
        m_stateStack.push_back(m_states[name]);
    });
}

void GameStateManager::PushState(const char* name, StateTransition* transition)
{
    m_transition = transition;
}

void GameStateManager::PopState()
{
    m_commands.push_back([=]() {
        m_stateStack.pop_back();
    });
}
