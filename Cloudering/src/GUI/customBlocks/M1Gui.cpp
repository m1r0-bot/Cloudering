#include "M1Gui.h"
#include "Block.h"

std::vector<std::unique_ptr<Block>> M1Gui::GUIComponents;

bool M1Gui::MovableCollapsingHeader(std::string label, int id, bool* destroy) {
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
    bool open = ImGui::CollapsingHeader((label + " " + std::to_string(id)).c_str(), (hovered ? ImGuiTreeNodeFlags_AllowItemOverlap : 0)); //todo: ImGuiTreeNodeFlags_DefaultOpen |

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
    return open;
}

int M1Gui::getIndex(int id)
{
    for (int i = 0; i < GUIComponents.size(); i++) {
        if (GUIComponents[i]->getId() == id) return i;
    }
    throw std::invalid_argument("couldn't find component, invalid id");
}