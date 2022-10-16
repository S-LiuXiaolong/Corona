#include "SceneObject.h"

using namespace std;

namespace Corona
{
    ostream& operator<<(ostream& out, SceneObjectType type)
    {
        int32_t n = static_cast<int32_t>(type);
        n = endian_net_unsigned_int<int32_t>(n);
        char* c = reinterpret_cast<char*>(&n);
         
        for (size_t i = 0; i < sizeof(int32_t); i++) {
            out << *c++;
        }

        return out;
    }

    // ostream& operator<<(ostream& out, PrimitiveType type)
    // {
    //     int32_t n = static_cast<int32_t>(type);
    //     n = endian_net_unsigned_int<int32_t>(n);
    //     char* c = reinterpret_cast<char*>(&n);
         
    //     for (size_t i = 0; i < sizeof(int32_t); i++) {
    //         out << *c++;
    //     }

    //     return out;
    // }

	ostream& operator<<(ostream& out, const BaseSceneObject& obj)
	{
		out << "SceneObject" << endl;
		out << "-----------" << endl;
		out << "GUID: " << obj.m_Guid << endl;
		out << "Type: " << obj.m_Type << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectVertexArray& obj)
	{
		out << "Data Size in Bytes: 0x" << obj.m_szData << endl;
		// out << "Data: ";
		// for(size_t i = 0; i < obj.m_szData; i++)
		// {
		// 	out << *(reinterpret_cast<const float*>(obj.m_pData) + i) << ' ';;
		// }

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectIndexArray& obj)
	{
		out << "Data Size in Bytes: 0x" << obj.m_szData << endl;
		// out << "Data: ";
		// for(size_t i = 0; i < obj.m_szData; i++)
		// {
        //     out << "0x" << *(reinterpret_cast<const uint16_t*>(obj.m_pData) + i) << ' ';
		// }

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectPrimitive& obj)
	{
		out << static_cast<const BaseSceneObject&>(obj) << endl;
		out << "This primitive contains 0x" << obj.GetVertexCount() << " vertices." << endl;
		out << "This mesh contains 0x" << obj.GetIndexCount() << " indices." << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectTexture& obj)
	{
		out << static_cast<const BaseSceneObject&>(obj) << endl;
		out << "Name: " << obj.m_Name << endl;
		if (obj.m_pImage)
			out << "Image: " << *obj.m_pImage << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectMaterial& obj)
	{
		out << static_cast<const BaseSceneObject&>(obj) << endl;
		out << "Name: " << obj.m_Name << endl;
		out << "Color: " << *obj.ColorMap << endl;
		out << "PhysicsDescriptor: " << *obj.PhysicsDescriptorMap << endl;
		out << "Normal: " << *obj.NormalMap << endl;
		out << "Ambient Occlusion: " << *obj.AOMap << endl;
        out << "Emissive: " << *obj.Emissivemap << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectMesh& obj)
	{
		auto count = obj.m_Primitive.size();
		for(decltype(count) i = 0; i < count; i++) {
			out << "Mesh: " << *obj.m_Primitive[i] << endl;
		}

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectLight& obj)
	{
		out << static_cast<const BaseSceneObject&>(obj) << endl;
		out << "Color: " << obj.m_LightColor << endl;
		out << "Intensity: " << obj.m_fIntensity << endl;
        out << "Range: " << obj.m_fRange << " // 0.0 = infinite" << endl;
		out << "Cast Shadows: " << obj.m_bCastShadows << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectOmniLight& obj)
	{
		out << "Light Type: Omni" << endl;
		out << static_cast<const SceneObjectLight&>(obj) << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectSpotLight& obj)
	{
		out << "Light Type: Spot" << endl;
		out << static_cast<const SceneObjectLight&>(obj) << endl;
		out << "Inner Cone Angle: " << obj.m_fInnerConeAngle << endl;
		out << "Outer Cone Angle: " << obj.m_fOuterConeAngle << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectInfiniteLight& obj)
	{
		out << "Light Type: Infinite" << endl;
		out << static_cast<const SceneObjectLight&>(obj) << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectCamera& obj)
	{
		out << static_cast<const BaseSceneObject&>(obj) << endl;
		out << "Aspect: " << obj.m_fAspect << endl;
		out << "Near Clip Distance: " << obj.m_fNearClipDistance << endl;
		out << "Far Clip Distance: " << obj.m_fFarClipDistance << endl;

		return out;
	}

	ostream& operator<<(ostream& out, const SceneObjectOrthogonalCamera& obj)
	{
		out << "Camera Type: Orthogonal" << endl;
		out << static_cast<const SceneObjectCamera&>(obj) << endl;

		return out;
	}


	ostream& operator<<(ostream& out, const SceneObjectPerspectiveCamera& obj)
	{
		out << "Camera Type: Perspective" << endl;
		out << static_cast<const SceneObjectCamera&>(obj) << endl;
		out << "FOV: " << obj.m_fFov<< endl;

		return out;
	}

	// ostream& operator<<(ostream& out, const SceneObjectTransform& obj)
	// {
	// 	out << "Transform Matrix: " << obj.m_matrix << endl;
	// 	out << "Is Object Local: " << obj.m_bSceneObjectOnly << endl;

	// 	return out;
	// }

    // ostream& operator<<(ostream& out, const SceneObjectTrack& obj)
	// {
	// 	out << "Animation Track: " << endl;
	// 	out << "Time: " << obj.m_Time->GetCurveType() << endl;
	// 	out << "Value: " << obj.m_Value->GetCurveType() << endl;
	// 	out << "Transform: " << *obj.m_pTransform << endl;

	// 	return out;
	// }

    // ostream& operator<<(ostream& out, const SceneObjectAnimationClip& obj)
	// {
	// 	out << "Animation Clip: " << obj.m_nIndex << endl;
	// 	out << "Num of Track(s): " << obj.m_Tracks.size() << endl;
	// 	for (auto track : obj.m_Tracks)
	// 	{
	// 		out << *track;
	// 	}

	// 	return out;
	// }

}