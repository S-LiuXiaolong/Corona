#pragma once
#include <vector>
#include "IRuntimeModule.h"
#include "SceneManager.h"

namespace Corona 
{
    class IPhysicsManager : implements IRuntimeModule
    {
    public:
        virtual int Initialize() = 0;
        virtual void Finalize() = 0;
        virtual void Tick() = 0;

        // TODO: Temporary for testing
        // virtual void CreateRigidBody(SceneGeometryNode& node, const SceneObjectMesh& geometry) = 0;
        // virtual void DeleteRigidBody(SceneGeometryNode& node) = 0;
        virtual void CreateRigidBody(SceneNode& node, const SceneObjectMesh& geometry) = 0;
        virtual void DeleteRigidBody(SceneNode& node) = 0;

        virtual int CreateRigidBodies() = 0;
        virtual void ClearRigidBodies() = 0;

        virtual Matrix4X4f GetRigidBodyTransform(void* rigidBody) = 0;
        // virtual void UpdateRigidBodyTransform(SceneGeometryNode& node) = 0;
        virtual void UpdateRigidBodyTransform(SceneNode& node) = 0;

        virtual void ApplyCentralForce(void* rigidBody, Vector3f force) = 0;
    };

    extern IPhysicsManager* g_pPhysicsManager;
}

