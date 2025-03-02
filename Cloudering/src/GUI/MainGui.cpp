#include "MainGui.h"
#include "../tools/FileLoader.h"

MainGui::MainGui(Window &window) : window(window) {

    const char* glsl_version = "#version 330";
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding.x = 3;
    style.WindowPadding.y = 3;

    ImGui_ImplGlfw_InitForOpenGL(window.window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //frameBuffer = new FrameBuffer(rendererSize.x, rendererSize.y);
    //renderer = &Renderer::getInstance();
    //renderer->Init(frameBuffer, &camera);
}


void MainGui::MainLoop(double deltaTime) {
    InitializeNewFrame();

    RendererFrame();
    MainControlPanel();

    ShowSidebar( v );

    DrawImGuiUI();
}

void MainGui::ShowSidebar(std::vector<Component>& components) {
    ImGui::Begin("Inspector Sidebar", nullptr, ImGuiWindowFlags_AlwaysAutoResize);

    for (size_t i = 0; i < components.size(); ) {
        ImGui::PushID(static_cast<int>(i)); // Unique ID for each component

        // Get current position for the header
        ImVec2 headerStart = ImGui::GetCursorScreenPos();
        float buttonSize = 20.0f;

        // Manually position the "X" button before the header
        ImGui::SetCursorScreenPos(ImVec2(headerStart.x + ImGui::GetContentRegionAvail().x - buttonSize - 5, headerStart.y+2));
        bool remove = ImGui::SmallButton("X");

        // Get the rectangle bounds of the button
        ImVec2 buttonMin = ImGui::GetItemRectMin();
        ImVec2 buttonMax = ImGui::GetItemRectMax();

        // Move cursor back to the original position for header
        ImGui::SetCursorScreenPos(headerStart);

        // Block the collapsing header toggle if clicking on the button
        bool hovered = ImGui::IsMouseHoveringRect(buttonMin, buttonMax);
        bool open = ImGui::CollapsingHeader(components[i].name.c_str(), ImGuiTreeNodeFlags_DefaultOpen | (hovered ? ImGuiTreeNodeFlags_AllowItemOverlap : 0));

        if (remove) {
            components.erase(components.begin() + i);
            ImGui::PopID();
            continue; // Skip further processing for this item
        }

        // Drag Source (Make component draggable)
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            ImGui::SetDragDropPayload("COMPONENT_MOVE", &i, sizeof(size_t)); // Send index as payload
            ImGui::Text("Move %s", components[i].name.c_str());
            ImGui::EndDragDropSource();
        }

        // Drag Target (Accept drag-and-drop to reorder)
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("COMPONENT_MOVE")) {
                size_t draggedIndex = *(size_t*)payload->Data;
                if (draggedIndex != i) {
                    std::swap(components[draggedIndex], components[i]);
                }
            }
            ImGui::EndDragDropTarget();
        }

        if (open) {
            ImGui::Text("Component Content Goes Here...");
        }

        ImGui::PopID();
        ++i;
    }

    ImGui::End();
}



void MainGui::InitializeNewFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Create the docking environment
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
        ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;


    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::GetStyle().WindowRounding = 0.0f;

    if (ImGui::Begin("InvisibleWindowDock", nullptr, windowFlags)) {

        MainMenuBar();

        ImGuiID dockSpaceId = ImGui::GetID("InvisibleWindowDock");
        ImGui::DockSpace(dockSpaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

        static auto first_time = true;
        if (first_time)
        {
            first_time = false;

            ImGui::DockBuilderRemoveNode(dockSpaceId); // clear any previous layout
            ImGui::DockBuilderAddNode(dockSpaceId, ImGuiDockNodeFlags_DockSpace | ImGuiDockNodeFlags_NoUndocking);
            ImGui::DockBuilderSetNodeSize(dockSpaceId, ImGui::GetIO().DisplaySize);

            ImGuiID left_dock_id;
            ImGuiID right_dock_id;
            ImGui::DockBuilderSplitNode(dockSpaceId, ImGuiDir_Right, 0.2f, &right_dock_id, &left_dock_id);

            ImGui::DockBuilderDockWindow("Control Panel", right_dock_id);
            ImGui::DockBuilderDockWindow("Renderer", left_dock_id);
            ImGui::DockBuilderFinish(dockSpaceId);
        }
    }ImGui::End();
}


void MainGui::MainMenuBar() {
    if (ImGui::BeginMenuBar()) {
        static std::string scenario = "Scenario 1";
        if (ImGui::BeginMenu(scenario.c_str())) {
            if (ImGui::MenuItem("Scenario 1", nullptr)) {
                scenario = "Scenario 1";
            }
            if (ImGui::MenuItem("Scenario 2", nullptr)) {
                scenario = "Scenario 2";
            }
            if (ImGui::MenuItem("Scenario 3", nullptr)) {
                scenario = "Scenario 3";
            }
            ImGui::EndMenu();
        }
    }ImGui::EndMenuBar();
}

void MainGui::MainControlPanel() {
    if(ImGui::Begin("Control Panel")) {
        for (int i = 0; i < M1Gui::GUIComponents.size(); i++) {
            M1Gui::GUIComponents[i]->RenderUI();
        }

        if (ImGui::Button("Add Component"))
        {
            ImGui::OpenPopup("ComponentPopup"); // Open the popup
        }

        if (ImGui::BeginPopup("ComponentPopup"))
        {
            if (ImGui::MenuItem("Camera")) { M1Gui::GUIComponents.push_back(BlockFactory::create(CAMERA)); }
            if (ImGui::MenuItem("Shader")) { M1Gui::GUIComponents.push_back(BlockFactory::create(SHADER)); }
            ImGui::EndPopup();
        }
    } 
    ImGui::End();
}

void MainGui::RendererFrame() {
    ImGuiWindowClass window_class1;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class1);

    static ImVec2 lastRendererScale = ImVec2(0, 0);
    if (ImGui::Begin("Renderer")) {

    }
    ImGui::End();
}

void MainGui::DrawImGuiUI() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}