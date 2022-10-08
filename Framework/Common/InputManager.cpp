#include <iostream>
#include "InputManager.h"
#include "GraphicsManager.h"
#include "geommath.h"

using namespace std;

namespace Corona
{
    int InputManager::Initialize()
    {
        return 0;
    }

    void InputManager::Finalize()
    {
    }

    void InputManager::Tick()
    {
    }

    void InputManager::UpArrowKeyDown()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Up Arrow Key Down!" << endl;
    #endif
        g_pGraphicsManager->WorldRotateX(PI / 60.0f);
    }

    void InputManager::UpArrowKeyUp()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Up Arrow Key Up!" << endl;
    #endif
    }

    void InputManager::DownArrowKeyDown()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Down Arrow Key Down!" << endl;
    #endif
        g_pGraphicsManager->WorldRotateX(-PI / 60.0f);
    }

    void InputManager::DownArrowKeyUp()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Down Arrow Key Up!" << endl;
    #endif
    }

    void InputManager::LeftArrowKeyDown()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Left Arrow Key Down!" << endl;
    #endif
        g_pGraphicsManager->WorldRotateY(-PI / 60.0f);
    }

    void InputManager::LeftArrowKeyUp()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Left Arrow Key Up!" << endl;
    #endif
    }

    void InputManager::RightArrowKeyDown()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Right Arrow Key Down!" << endl;
    #endif
        g_pGraphicsManager->WorldRotateY(PI / 60.0f);
    }

    void InputManager::RightArrowKeyUp()
    {
    #ifdef _DEBUG
        cerr << "[InputManager] Right Arrow Key Up!" << endl;
    #endif
    }

}


