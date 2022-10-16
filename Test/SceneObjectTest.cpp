#include <iostream>
#include "SceneObject.h"
#include "SceneNode.h"

using namespace Corona;
using namespace std;
using namespace xg;

int32_t main(int32_t argc, char** argv)
{
    int32_t result = 0;
    std::shared_ptr<SceneObjectMesh>    soGeometry(new SceneObjectMesh());
    std::shared_ptr<SceneObjectOmniLight>    soOmniLight(new SceneObjectOmniLight());
    std::shared_ptr<SceneObjectSpotLight>    soSpotLight(new SceneObjectSpotLight());
    std::shared_ptr<SceneObjectOrthogonalCamera>      soOrthogonalCamera(new SceneObjectOrthogonalCamera());
    std::shared_ptr<SceneObjectPerspectiveCamera>     soPerspectiveCamera(new SceneObjectPerspectiveCamera());

    std::shared_ptr<SceneObjectPrimitive>         soPrimitive(new SceneObjectPrimitive());
    std::shared_ptr<SceneObjectMaterial>     soMaterial(new SceneObjectMaterial());

    soGeometry->AddPrimitive(soPrimitive);

    cout << *soGeometry << endl;
    cout << *soMaterial << endl;
    cout << *soOmniLight << endl;
    cout << *soSpotLight << endl;
    cout << *soOrthogonalCamera  << endl;
    cout << *soPerspectiveCamera << endl;

    SceneEmptyNode      snEmpty;
    std::shared_ptr<SceneGeometryNode>   snGeometry(new SceneGeometryNode());
    std::shared_ptr<SceneLightNode>     snLight(new SceneLightNode());
    std::shared_ptr<SceneCameraNode>     snCamera(new SceneCameraNode());

    snGeometry->AddSceneObjectRef(soGeometry->GetGuid());
    snLight->AddSceneObjectRef(soSpotLight->GetGuid());
    snCamera->AddSceneObjectRef(soOrthogonalCamera->GetGuid());

    snEmpty.AppendChild(std::move(snGeometry));
    snEmpty.AppendChild(std::move(snLight));
    snEmpty.AppendChild(std::move(snCamera));

    cout << snEmpty << endl;

    return result;
}

