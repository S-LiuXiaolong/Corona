#include "BaseApplication.h"

// Parse command line, read configuration, initialize all submodules
int Corona::BaseApplication::Initialize()
{
    m_bQuit = false;

    return 0;
}

// Finalize all submodules and clean up all runtime temporary files.
void Corona::BaseApplication::Finalize()
{

}

// One cycle of the main loop
void Corona::BaseApplication::Tick()
{

}

bool Corona::BaseApplication::IsQuit()
{
    return m_bQuit;
}