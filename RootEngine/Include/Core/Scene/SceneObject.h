#pragma once
#include "Core/RObject.h"


namespace Faia
{
    namespace Root
    {
        class SceneObject : public RObject
        {
        public:
            SceneObject();
            SceneObject(std::string name);
            void SetName(std::string name);
            std::string GetName();
        private:
            std::string mName;
        };
    }
}