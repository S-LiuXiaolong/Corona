#include <iostream>
#include "PhysicsManager.h"

namespace Corona
{
    int PhysicsManager::Initialize()
    {
        // Build the broadphase
        m_btBroadphase = new btDbvtBroadphase();

        // Set up the collision configuration and dispatcher
        m_btCollisionConfiguration = new btDefaultCollisionConfiguration();
        m_btDispatcher = new btCollisionDispatcher(m_btCollisionConfiguration);

        // The actual physics solver
        m_btSolver = new btSequentialImpulseConstraintSolver;

        // The world
        m_btDynamicsWorld = new btDiscreteDynamicsWorld(m_btDispatcher, m_btBroadphase, m_btSolver, m_btCollisionConfiguration);
        m_btDynamicsWorld->setGravity(btVector3(0.0f, 0.0f, -9.8f));

        return 0;
    }
}