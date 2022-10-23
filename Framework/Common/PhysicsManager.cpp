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
        m_btDynamicsWorld->setGravity(btVector3(0.0f, -9.8f, 0.0f));

        return 0;
    }

    void PhysicsManager::Finalize()
    {
        // Clean up
        ClearRigidBodies();

        delete m_btDynamicsWorld;
        delete m_btSolver;
        delete m_btDispatcher;
        delete m_btCollisionConfiguration;
        delete m_btBroadphase;
    }

    void PhysicsManager::Tick()
    {
        if (g_pSceneManager->IsSceneChanged())
        {
            ClearRigidBodies();
            CreateRigidBodies();
            g_pSceneManager->NotifySceneIsPhysicalSimulationQueued();
        }

        m_btDynamicsWorld->stepSimulation(1.0f / 60.0f, 10);
    }

    void PhysicsManager::CreateRigidBody(SceneNode& node, const SceneObjectMesh& geometry)
    {
        btRigidBody* rigidBody = nullptr;

        const float* param = geometry.CollisionParameters();

        switch(geometry.CollisionType())
        {
            case SceneObjectCollisionType::kSceneObjectCollisionTypeSphere:
                {
                    btSphereShape* sphere = new btSphereShape(param[0]);
                    m_btCollisionShapes.push_back(sphere);

                    const auto trans = node.GetCalculatedTransform();
                    btTransform startTransform;
                    startTransform.setIdentity();
                    startTransform.setOrigin(btVector3(trans->data[3][0], trans->data[3][1], trans->data[3][2]));
                    startTransform.setBasis(btMatrix3x3(trans->data[0][0], trans->data[1][0], trans->data[2][0],
                                                trans->data[0][1], trans->data[1][1], trans->data[2][1],
                                                trans->data[0][2], trans->data[1][2], trans->data[2][2]));
                    btDefaultMotionState* motionState = 
                        new btDefaultMotionState(
                                    startTransform
                                );
                    btScalar mass = 1.0f;
                    btVector3 fallInertia(0.0f, 0.0f, 0.0f);
                    sphere->calculateLocalInertia(mass, fallInertia);
                    btRigidBody::btRigidBodyConstructionInfo
                        rigidBodyCI(mass, motionState, sphere, fallInertia);
                    rigidBody = new btRigidBody(rigidBodyCI);
                    m_btDynamicsWorld->addRigidBody(rigidBody);
                }
                break;
            case SceneObjectCollisionType::kSceneObjectCollisionTypeBox:
                {
                    btBoxShape* box = new btBoxShape(btVector3(param[0], param[1], param[2]));
                    m_btCollisionShapes.push_back(box);

                    const auto trans = node.GetCalculatedTransform();
                    btTransform startTransform;
                    startTransform.setIdentity();
                    startTransform.setOrigin(btVector3(trans->data[3][0], trans->data[3][1], trans->data[3][2]));
                    startTransform.setBasis(btMatrix3x3(trans->data[0][0], trans->data[1][0], trans->data[2][0],
                                                trans->data[0][1], trans->data[1][1], trans->data[2][1],
                                                trans->data[0][2], trans->data[1][2], trans->data[2][2]));
                    btDefaultMotionState* motionState = 
                        new btDefaultMotionState(
                                    startTransform
                                );
                    btScalar mass = 0.0f;
                    btRigidBody::btRigidBodyConstructionInfo
                        rigidBodyCI(mass, motionState, box, btVector3(0.0f, 0.0f, 0.0f));
                    rigidBody = new btRigidBody(rigidBodyCI);
                    m_btDynamicsWorld->addRigidBody(rigidBody);
                }
                break;
            case SceneObjectCollisionType::kSceneObjectCollisionTypePlane:
                {
                    btStaticPlaneShape* plane = new btStaticPlaneShape(btVector3(param[0], param[1], param[2]), param[3]);
                    m_btCollisionShapes.push_back(plane);

                    const auto trans = node.GetCalculatedTransform();
                    btTransform startTransform;
                    startTransform.setIdentity();
                    startTransform.setOrigin(btVector3(trans->data[3][0], trans->data[3][1], trans->data[3][2]));
                    startTransform.setBasis(btMatrix3x3(trans->data[0][0], trans->data[1][0], trans->data[2][0],
                                                trans->data[0][1], trans->data[1][1], trans->data[2][1],
                                                trans->data[0][2], trans->data[1][2], trans->data[2][2]));
                    btDefaultMotionState* motionState = 
                        new btDefaultMotionState(
                                    startTransform
                                );
                    btScalar mass = 0.0f;
                    btRigidBody::btRigidBodyConstructionInfo
                        rigidBodyCI(mass, motionState, plane, btVector3(0.0f, 0.0f, 0.0f));
                    rigidBody = new btRigidBody(rigidBodyCI);
                    m_btDynamicsWorld->addRigidBody(rigidBody);
                }
                break;
            default:
                ;
        }

        node.LinkRigidBody(rigidBody);
    }

    void PhysicsManager::UpdateRigidBodyTransform(SceneNode& node)
    {
        const auto trans = node.GetCalculatedTransform();
        auto rigidBody = node.RigidBody();
        auto motionState = reinterpret_cast<btRigidBody*>(rigidBody)->getMotionState();
        btTransform _trans;
        _trans.setIdentity();
        _trans.setOrigin(btVector3(trans->data[3][0], trans->data[3][1], trans->data[3][2]));
        _trans.setBasis(btMatrix3x3(trans->data[0][0], trans->data[1][0], trans->data[2][0],
                                    trans->data[0][1], trans->data[1][1], trans->data[2][1],
                                    trans->data[0][2], trans->data[1][2], trans->data[2][2]));
        motionState->setWorldTransform(_trans);
    }

    void PhysicsManager::DeleteRigidBody(SceneNode& node)
    {
        btRigidBody* rigidBody = reinterpret_cast<btRigidBody*>(node.UnlinkRigidBody());
        if(rigidBody) {
            m_btDynamicsWorld->removeRigidBody(rigidBody);
            if (auto motionState = rigidBody->getMotionState())
                delete motionState;
            delete rigidBody;
            //m_btDynamicsWorld->removeCollisionObject(rigidBody);
        }
    }

    int PhysicsManager::CreateRigidBodies()
    {
        auto& scene = g_pSceneManager->GetSceneForPhysicalSimulation();

        // Geometries
        for (auto _it : scene.GeometryNodes)
        {
            auto pGeometryNode = _it.second.lock();
            if (pGeometryNode)
            {
                auto pGeometry = scene.GetGeometry(pGeometryNode->GetSceneObjectRef());
                assert(pGeometry);

                CreateRigidBody(*pGeometryNode, *pGeometry);
            }
        }

        return 0;
    }

    void PhysicsManager::ClearRigidBodies()
    {
        auto& scene = g_pSceneManager->GetSceneForPhysicalSimulation();

        // Geometries
        for (auto _it : scene.GeometryNodes)
        {
            auto pGeometryNode = _it.second.lock();
            if (pGeometryNode)
                DeleteRigidBody(*pGeometryNode);
        }

        for (auto shape : m_btCollisionShapes)
        {
            delete shape;
        }

        m_btCollisionShapes.clear();
    }

    Matrix4X4f PhysicsManager::GetRigidBodyTransform(void* rigidBody)
    {
        Matrix4X4f result;
        btTransform trans;
        reinterpret_cast<btRigidBody*>(rigidBody)->getMotionState()->getWorldTransform(trans);
        auto basis  = trans.getBasis();
        auto origin = trans.getOrigin();
        BuildIdentityMatrix(result);
        result.data[0][0] = static_cast<float>(basis[0][0]);
        result.data[1][0] = static_cast<float>(basis[0][1]);
        result.data[2][0] = static_cast<float>(basis[0][2]);
        result.data[0][1] = static_cast<float>(basis[1][0]);
        result.data[1][1] = static_cast<float>(basis[1][1]);
        result.data[2][1] = static_cast<float>(basis[1][2]);
        result.data[0][2] = static_cast<float>(basis[2][0]);
        result.data[1][2] = static_cast<float>(basis[2][1]);
        result.data[2][2] = static_cast<float>(basis[2][2]);
        result.data[3][0] = static_cast<float>(origin.getX());
        result.data[3][1] = static_cast<float>(origin.getY());
        result.data[3][2] = static_cast<float>(origin.getZ());

        return result;
    }

    void PhysicsManager::ApplyCentralForce(void* rigidBody, Vector3f force)
    {
        btRigidBody* _rigidBody = reinterpret_cast<btRigidBody*>(rigidBody);
        btVector3 _force(force[0], force[1], force[2]);
        _rigidBody->activate(true);
        _rigidBody->applyCentralForce(_force);
    }
}