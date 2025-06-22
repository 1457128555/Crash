#include "CrashPCH.h"
#include "EditorActor.h"
#include "EditorActorComp.h"

#include "EditorDeclare.hpp"

#include "CrashActor.h"

using namespace Crash;

void EditorActor::Render(Actor* _ptr)
{
    const std::string& actName= _ptr->getName();
    if (ImGui::TreeNode(actName.c_str()))
    {
        //  Right click menu
        if (ImGui::BeginPopupContextItem())
        {
            //  
            if (ImGui::MenuItem("Rename"))
            {

            }

            if (ImGui::MenuItem("Add Actor"))
                _ptr->addActor(std::make_shared<Actor>());
            
            ImGui::EndPopup();
        }


        //  Comps
        const std::vector<ActorCompSrdPtr>& comps = _ptr->getActorComps();
        for(auto && comp : comps)
            EditorActorComp::Render(comp.get());

        //  Actors
        const std::vector<ActorSrdPtr>& actors = _ptr->getActors();
        for(auto && actor : actors)
            EditorActor::Render(actor.get());

        ImGui::TreePop();
    }
}