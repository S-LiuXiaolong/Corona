#include "Corona.h"

class Sandbox : public Corona::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}

};

Corona::Application* Corona::CreateApplication()
{
	return new Sandbox();
}