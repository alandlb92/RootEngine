#include "Core/Scene/SceneObject.h"

namespace Faia
{
    namespace Root
    {
        SceneObject::SceneObject()
        {
            mName = "New object";
        }
        
        SceneObject::SceneObject(std::string name)
        {
            mName = name;
        }

        void SceneObject::SetName(std::string name)
        {
            mName = name;
        }
        
        std::string SceneObject::GetName()
        {
            return mName;
        }
    }
}