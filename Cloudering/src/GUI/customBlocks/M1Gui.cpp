#include "M1Gui.h"
#include "Block.h"
#include "BlockFactory.h"
#include "iostream"

std::vector<std::unique_ptr<Block>> M1Gui::GUIComponents;
std::vector<int> M1Gui::destroyQueue;

bool M1Gui::MovableCollapsingHeader(std::string &label, int id, bool* destroy, bool* isRenaming) {
    ImVec2 headerStart = ImGui::GetCursorScreenPos();
    float buttonSize = 20.0f;

    ImGui::SetCursorScreenPos(ImVec2(headerStart.x + ImGui::GetContentRegionAvail().x - buttonSize - 5, headerStart.y + 2));
    *destroy = ImGui::SmallButton("X");

    if (*destroy) {
        return false;
    }

    ImVec2 buttonMin = ImGui::GetItemRectMin();
    ImVec2 buttonMax = ImGui::GetItemRectMax();

    ImGui::SetCursorScreenPos(headerStart);

    bool hovered = ImGui::IsMouseHoveringRect(buttonMin, buttonMax);
    bool open = ImGui::CollapsingHeader(label.c_str(), (hovered ? ImGuiTreeNodeFlags_AllowItemOverlap : 0)); //todo: ImGuiTreeNodeFlags_DefaultOpen |

    int index = getIndex(id);

    if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
        ImGui::SetDragDropPayload("COMPONENT_MOVE", &index, sizeof(size_t)); // Send index as payload
        ImGui::Text("Move %s", label.c_str());
        ImGui::EndDragDropSource();
    }

    // Drag Target (Accept drag-and-drop to reorder)
    if (ImGui::BeginDragDropTarget()) {
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COMPONENT_MOVE")) {
            size_t draggedIndex = *(int*)payload->Data;
            if (draggedIndex != index) {
                auto element = std::move(GUIComponents[draggedIndex]);
                GUIComponents.erase(GUIComponents.begin() + draggedIndex);
                GUIComponents.insert(GUIComponents.begin() + index, std::move(element));
            }
        }
        ImGui::EndDragDropTarget();
    }

    //renaming
    if (isRenaming == nullptr) return open;

    char renameBuffer[64] = "New Name";
    if (ImGui::BeginPopupContextItem("HeaderContext"))
    {
        if (ImGui::MenuItem("Rename"))
        {
            *isRenaming = true;
            strncpy_s(renameBuffer, label.c_str(), sizeof(renameBuffer));
            renameBuffer[sizeof(renameBuffer) - 1] = '\0';
        }
        ImGui::EndPopup();
    }
    if (*isRenaming)
    {
        ImGui::SetNextItemWidth(200);
        ImGui::SetKeyboardFocusHere();
        if (ImGui::InputText("##RenameHeader", renameBuffer, sizeof(renameBuffer), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            label = renameBuffer;
            *isRenaming = false;
        }
        else if (!ImGui::IsItemActive() && ImGui::IsMouseClicked(0))
        {
            *isRenaming = false;
        }
    }
    return open;
}

int M1Gui::getIndex(int id)
{
    for (int i = 0; i < GUIComponents.size(); i++) {
        if (GUIComponents[i]->getId() == id) return i;
    }
    throw std::invalid_argument("couldn't find component, invalid id");
}

Json::Value M1Gui::ComponentsToJSON() {
    Json::Value json;

    for (int i = 0; i < GUIComponents.size(); i++) {
        int blockId = GUIComponents[i].get()->getId();
        json[blockId] = GUIComponents[i].get()->Serialize();
    }

    return json;
}

void M1Gui::JSONToComponents(Json::Value& components) {

    if (!components.isArray())
        return;

    for (Json::Value json : components) {
        if (!json.isObject() || !json.isMember("type")) {
            continue;
        }
        M1Gui::GUIComponents.push_back(BlockFactory::create( static_cast<EnumBlockType>(json["type"].asInt()) ));
        M1Gui::GUIComponents.back().get()->Deserialize(json);
    }
}

void M1Gui::DestroyQueuedComponents() {
    for (int id : destroyQueue) {
        for (int i = 0; i < GUIComponents.size(); i++) {
            if (GUIComponents[i].get()->getId() == id) {
                GUIComponents.erase(GUIComponents.begin() + i);
                break;
            }
        }
    }
}