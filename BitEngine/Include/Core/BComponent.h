#pragma once
class BObject;

class BComponent
{
    friend class Scene;
    friend class BObject;

protected:
    virtual void Init();
    virtual void Update(float deltaTime);
    BObject* _owner;
};