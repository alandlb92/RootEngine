#pragma once

namespace Faia
{
    namespace Root
    {
        class BObject;
        struct Vector3D;

        class BComponent
        {
            friend class Scene;
            friend class BObject;

        protected:
            virtual void Init();
            virtual void Update(float deltaTime);
            BObject* mOwner;
        };
    }
}