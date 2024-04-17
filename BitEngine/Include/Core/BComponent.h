#pragma once

class BComponent
{
    friend class Scene;

protected:
    virtual void Init();
    virtual void Update(float deltaTime);
};