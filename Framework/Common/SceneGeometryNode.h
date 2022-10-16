#pragma once 
#include "BaseSceneNode.h"
#include "SceneObjectMesh.h"

namespace Corona
{
    class SceneGeometryNode : public SceneNode<SceneObjectMesh> 
    {
        protected:
            std::vector<std::string> m_Materials;
            void*       m_pRigidBody = nullptr;

        protected:
            virtual void dump(std::ostream& out) const 
            { 
                SceneNode::dump(out);
                out << "Material(s): " << std::endl;
                for (auto material : m_Materials) {
                    out << material << std::endl;
                }
            };

        public:
            // TODO: change here to fit in Diligent-like matetial
            void AddMaterial(const std::string& key) { m_Materials.push_back(key); };
            void AddMaterial(const std::string&& key) { m_Materials.push_back(std::move(key)); };

            std::string GetMaterialRef(const size_t index) 
            { 
                if (index < m_Materials.size())
                    return m_Materials[index]; 
                else
                    return std::string("default");
            };

            // void LinkRigidBody(void* rigidBody)
            // {
            //     m_pRigidBody = rigidBody;
            // }

            // void* UnlinkRigidBody()
            // {
            //     void* rigidBody = m_pRigidBody;
            //     m_pRigidBody = nullptr;

            //     return rigidBody;
            // }

            // void* RigidBody() { return m_pRigidBody; }
    };
}