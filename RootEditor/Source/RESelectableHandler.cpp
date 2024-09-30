#include "RESelectableHandler.h"
namespace Faia
{
    namespace Editor
    {
        std::unique_ptr<RESelectableHandler> gRESelectableHandler;
        std::unique_ptr<RESelectableHandler>& GetSelectableHandler()
        {
            if (!gRESelectableHandler)
            {
                gRESelectableHandler = std::make_unique<RESelectableHandler>();
            }

            return gRESelectableHandler;
        }

        void RESelectableProvider::SelectObject(Root::SceneObject* obj)
        {
            GetSelectableHandler()->SetSelectedObject(obj);
        }

        Root::SceneObject* RESelectableProvider::GetSelectedObject()
        {
            return GetSelectableHandler()->GetSelectedObject();
        }
        
        void RESelectableHandler::SetSelectedObject(Root::SceneObject* obj)
        {
            //Implement Commands pool in some place
            mSelectedObject = obj;
        }

        Root::SceneObject* RESelectableHandler::GetSelectedObject()
        {
            return mSelectedObject;
        }
    }
}