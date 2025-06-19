#include "CrashPCH.h"
#include "EditorActorComp.h"
#include "EditorDeclare.hpp"

#include "CrashActorComp.h"

using namespace Crash;

void EditorActorComp::Render(ActorComp* _ptr)
{
    const std::string& compName= _ptr->getName();
    if (ImGui::TreeNode(compName.c_str()))
    {
        

        ImGui::TreePop();
    }
}