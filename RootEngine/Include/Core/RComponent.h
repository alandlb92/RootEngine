#pragma once

namespace Faia
{
    namespace Root
    {
        class RObject;
        struct RVector3D;

        class RComponent
        {
            friend class Scene;
            friend class RObject;

        protected:
            virtual void Init();
            virtual void Update(float deltaTime);
            RObject* mOwner;
        };
    }
}