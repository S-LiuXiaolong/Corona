#include "Corona.h"

class Sandbox : public Corona::Application
{
public:
	Sandbox(HINSTANCE hInstance);
	~Sandbox();

	virtual bool Initialize()override;

private:
	virtual void OnResize()override;
	virtual void Update(const GameTimer& gt)override;
	virtual void Draw(const GameTimer& gt)override;

};
