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
}


void MainGui::MainLoop(double deltaTime) {
    InitializeNewFrame();

    RendererFrame();
    MainControlPanel();

    DrawImGuiUI();
    
    HandleShortcuts();
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
        static std::string scenario = "Select Scenario";
        if (ImGui::BeginMenu(scenario.c_str())) {
            if (ImGui::MenuItem("TEST Triangle")) {
                scenario = "TEST Triangle";
                ScenarioFactory::setCurrentScene(TEST);
            }
            if (ImGui::MenuItem("Dobashi 2000")) {
                scenario = "Dobashi 2000";
                ScenarioFactory::setCurrentScene(METABALLS);
            }
            if (ImGui::MenuItem("SkyDome")) {
                scenario = "SkyDome";
                ScenarioFactory::setCurrentScene(SKYDOME);
            }
            if (ImGui::MenuItem("Horizon Zero Dawn")) {
                scenario = "Horizon Zero Dawn";
                ScenarioFactory::setCurrentScene(HORIZON_0_DAWN);
            }
            ImGui::EndMenu();
        }

        if (ScenarioFactory::isSelectedScenario()) {
            ScenarioFactory::currentScenario.get()->RenderMenuItems();
        }

    }ImGui::EndMenuBar();
}

void MainGui::MainControlPanel() {
    if(ImGui::Begin("Control Panel")) {
        if(ScenarioFactory::isSelectedScenario())
            ScenarioFactory::currentScenario.get()->RenderControlerGUI();
    } 
    ImGui::End();
}

void MainGui::RendererFrame() {
    ImGuiWindowClass window_class1;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
    window_class1.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoTabBar;
    ImGui::SetNextWindowClass(&window_class1);

    if (ImGui::Begin("Renderer")) {
        ImVec2 size = ImGui::GetContentRegionAvail();
        MouseCameraHandeler();
        Renderer::getInstance().RenderScene(size.x, size.y);

        ImGui::Image(
        (ImTextureID)Renderer::getInstance().getTexture(),
        size
        );
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

void MainGui::MouseCameraHandeler()
{
    Camera& camera = Camera::getInstance();
    static bool isDragging = false;
    static POINT dragStartPos;

    POINT mousePos;
    if (!GetCursorPos(&mousePos)) {
        mousePos.x = mousePos.y = 0;
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Left))
    {
        if (!isDragging && !ImGui::IsAnyItemActive() &&
            ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem))
        {
            ShowCursor(FALSE);
            dragStartPos = mousePos;
            isDragging = true;
        }
        else if (isDragging) {
            camera.processMouseDelta(vec2(mousePos.x - dragStartPos.x, mousePos.y - dragStartPos.y) * .05f);
            SetCursorPos(dragStartPos.x, dragStartPos.y);
        }
    }
    else if (isDragging)
    {
        SetCursorPos(dragStartPos.x, dragStartPos.y);
        isDragging = false;
        ShowCursor(TRUE);
    }

    if (isDragging || ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
        camera.processMouseScroll(ImGui::GetIO().MouseWheel);
    }
    if (ImGui::IsKeyDown(ImGuiKey_W)) camera.processKeyboardInput(vec3(0, 0, 1));
    if (ImGui::IsKeyDown(ImGuiKey_A)) camera.processKeyboardInput(vec3(-1, 0, 0));
    if (ImGui::IsKeyDown(ImGuiKey_S)) camera.processKeyboardInput(vec3(0, 0, -1));
    if (ImGui::IsKeyDown(ImGuiKey_D)) camera.processKeyboardInput(vec3(1, 0, 0));
    if (ImGui::IsKeyDown(ImGuiKey_Space)) camera.processKeyboardInput(vec3(0, -1, 0));
    if (ImGui::IsKeyDown(ImGuiKey_C) || ImGui::IsKeyDown(ImGuiKey_LeftShift)) camera.processKeyboardInput(vec3(0, 1, 0));
}

void MainGui::HandleShortcuts() {
    ImGuiIO& io = ImGui::GetIO();

    if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S, false)) {
        //SaveProjectToJSON();
    }
}