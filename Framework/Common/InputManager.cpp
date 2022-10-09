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

	void InputManager::WKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] W Key Down!" << endl;
#endif
		// position.x = mRadius*sinf(mPhi)*cosf(mTheta);Y(-0.2f);
	}

	void InputManager::WKeyUp()
	{
#ifdef _DEBUG
		cerr << "[InputManager] W Key Up!" << endl;
#endif
	}

	void InputManager::SKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] S Key Down!" << endl;
#endif
		// position.x = mRadius*sinf(mPhi)*cosf(mTheta);Y(0.2f);
	}

	void InputManager::SKeyUp()
	{
#ifdef _DEBUG
		cerr << "[InputManager] S Key Up!" << endl;
#endif
	}

	void InputManager::AKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] A Key Down!" << endl;
#endif
		// position.x = mRadius*sinf(mPhi)*cosf(mTheta);X(0.2f);
	}

	void InputManager::AKeyUp()
	{
#ifdef _DEBUG
		cerr << "[InputManager] A Key Up!" << endl;
#endif
	}

	void InputManager::DKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] D Key Down!" << endl;
#endif
		// position.x = mRadius*sinf(mPhi)*cosf(mTheta);X(-0.2f);
	}

	void InputManager::DKeyUp()
	{
#ifdef _DEBUG
		cerr << "[InputManager] D Key Up!" << endl;
#endif
	}

	void InputManager::IKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] I Key Down!" << endl;
#endif
		g_pGraphicsManager->CameraRotateY(0.2f);
	}

	void InputManager::IKeyUp()
	{

	}

	void InputManager::KKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] K Key Down!" << endl;
#endif
		g_pGraphicsManager->CameraRotateY(-0.2f);
	}

	void InputManager::KKeyUp()
	{

	}

	void InputManager::JKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] J Key Down!" << endl;
#endif
		g_pGraphicsManager->CameraRotateX(0.2f);
	}

	void InputManager::JKeyUp()
	{

	}

	void InputManager::LKeyDown()
	{
#ifdef _DEBUG
		cerr << "[InputManager] L Key Down!" << endl;
#endif
		g_pGraphicsManager->CameraRotateX(-0.2f);
	}

	void InputManager::LKeyUp()
	{

	}

}


