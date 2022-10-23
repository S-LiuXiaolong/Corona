#include "IPhysicsManager.h"
#define BT_USE_DOUBLE_PRECISION 1
#include "bullet3/btBulletDynamicsCommon.h"

namespace Corona
{
    class PhysicsManager : public IPhysicsManager
    {
    public:
        virtual int Initialize();
        virtual void Finalize();
        virtual void Tick();

        virtual void CreateRigidBody(SceneNode& node, SceneObjectMesh& geometry);
        virtual void DeleteRigidBody(SceneNode& node);

        virtual int CreateRigidBodies();
        virtual void ClearRigidBodies();

        Matrix4X4f GetRigidBodyTransform(void* rigidBody);
        void UpdateRigidBodyTransform(SceneNode& node);

        void ApplyCentralForce(void* rigidBody, Vector3f force);

    protected:
        btBroadphaseInterface* m_btBroadphase;
        btDefaultCollisionConfiguration* m_btCollisionConfiguration;
        btCollisionDispatcher* m_btDispatcher;
        btSequentialImpulseConstraintSolver* m_btSolver;
        btDiscreteDynamicsWorld* m_btDynamicsWorld;

        std::vector<btCollisionShape*> m_btCollisionShapes;
    };
}