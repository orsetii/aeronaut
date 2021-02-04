#include "gui.h"


Gui::Gui (int w, int h) {
	width = w;
	height = h;
}


int Gui::Run() {
	// Setup SDL
	// (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
	// depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// set OpenGL attributes
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	SDL_GL_SetAttribute(
	SDL_GL_CONTEXT_PROFILE_MASK,
	SDL_GL_CONTEXT_PROFILE_CORE
	);

	std::string glsl_version = "";
#ifdef __APPLE__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 150";
	SDL_GL_SetAttribute( // required on Mac OS
	SDL_GL_CONTEXT_FLAGS,
	SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG
	);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#elif __linux__
	// GL 3.2 Core + GLSL 150
	glsl_version = "#version 150";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
	// GL 3.0 + GLSL 130
	glsl_version = "#version 130";
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

	SDL_WindowFlags window_flags = (SDL_WindowFlags)(
	SDL_WINDOW_OPENGL
	| SDL_WINDOW_RESIZABLE
	| SDL_WINDOW_ALLOW_HIGHDPI
	);

	SDL_Window *window = SDL_CreateWindow(
		"Aeronaut C2 Server GUI" VERSION,
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width,
		height,
		window_flags
	);
	// limit to which minimum size user can resize the window
	SDL_SetWindowMinimumSize(window, 800, 600);

	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);

	// enable VSync
	SDL_GL_SetSwapInterval(1);
	// Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
	bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
	bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
	bool err = gladLoadGL() == 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD2)
	bool err = gladLoadGL(glfwGetProcAddress) == 0; // glad2 recommend using the windowing library loader instead of the (optionally) bundled one.
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING2)
	bool err = false;
	glbinding::Binding::initialize();
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLBINDING3)
	bool err = false;
	glbinding::initialize([](const char* name) { return (glbinding::ProcAddress)glfwGetProcAddress(name); });
#else
	bool err = false; // If you use IMGUI_IMPL_OPENGL_LOADER_CUSTOM, your loader is likely to requires some form of initialization.
#endif
	if(err) {
		fprintf(stderr, "Failed to initialize OpenGL loader!\n");
		return 1;
	}


	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.

	// Setup Io and add our flags.
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

	// Setup GUI style from "imgui-style.h"
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL3_Init(glsl_version.c_str());

	// colors are set in RGBA, but as float
	ImVec4 background = ImVec4(35/255.0f, 35/255.0f, 35/255.0f, 1.00f);

	glClearColor(background.x, background.y, background.z, background.w);

	// Setup default font
	//io.Fonts->AddFontDefault();

	// The Main GUI Event Loop
	// Polls and handles events.
	bool done = false;
	while (!done) {

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		ImGuiWindowFlags imgui_window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration
			| ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus   ;
		// Enable Dockspace at start
		static bool dockspaceOpen = true;

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
		    // without it you won't have keyboard input and other things
		    ImGui_ImplSDL2_ProcessEvent(&event);
		    // you might also want to check io.WantCaptureMouse and io.WantCaptureKeyboard
		    // before processing events
		    
		    switch (event.type)
		    {
			    case SDL_QUIT:
				done = true;
				break;

			    case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_RESIZED:
				    width = event.window.data1;
				    height = event.window.data2;
				    // std::cout << "[INFO] Window size: "
				    //           << width
				    //           << "x"
				    //           << height
				    //           << std::endl;
				    glViewport(0, 0, width, height);
				    break;
				}
				break;

			    case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				   done = true;	
				   break;
				}
				break;
			}
		}


		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplSDL2_NewFrame(window);
		ImGui::NewFrame();

		{

			int sdl_width = 0, sdl_height = 0;
			SDL_GetWindowSize(window, &sdl_width, &sdl_height);
			int controls_width = sdl_width;
			if ((controls_width /= 3) < 500) { controls_width = 500; }
			// make main widget width to be 1:1 of the main window dimensions

			// position the controls widget in the top-right corner with some margin
			ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
			ImGui::SetNextWindowSize(
			ImVec2(static_cast<float>(sdl_width), static_cast<float>(sdl_height)),
				ImGuiCond_Always
			);
			// here we set the calculated width and also make the height to be
			// be the height of the main window also with some margin
		}

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, imgui_window_flags);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("DockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), 0);
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows, 
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
				if (ImGui::MenuItem("New", "Ctrl+N"));

				if (ImGui::MenuItem("Open...", "Ctrl+O"));

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"));

				if (ImGui::MenuItem("Exit")) break;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}// a window is defined by Begin/End pair
       
		// Note ImGui::Text("Draw Calls: %d", stats.DrawCalls) can do printf stuff.

		ImGui::End();

		// a window is defined by Begin/End pair
		{
		    static int counter = 0;
		    // get the window size as a base for calculating widgets geometry
				    // create a window and append into it
		    ImGui::Begin("Example", NULL, ImGuiWindowFlags_None);

		    ImGui::Dummy(ImVec2(0.0f, 1.0f));
		    ImGui::TextColored(ImVec4(1.0f, 0.0f, 1.0f, 1.0f), "Platform");
		    ImGui::Text("%s", SDL_GetPlatform());
		    ImGui::Text("CPU cores: %d", SDL_GetCPUCount());
		    ImGui::Text("RAM: %.2f GB", SDL_GetSystemRAM() / 1024.0f);
		    
		    // buttons and most other widgets return true when clicked/edited/activated
		    if (ImGui::Button("Counter button"))
		    {
			std::cout << "counter button clicked\n";
			counter++;
		    }
		    ImGui::SameLine();
		    ImGui::Text("counter = %d", counter);

		    ImGui::End();
		}

		// Redraw Background
		SDL_GetWindowSize(window, &width, &height);


		// Rendering
		ImGui::Render();
		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		// Update and Render additional Platform Windows
		// (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
		//  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
			SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
		}

		SDL_GL_SwapWindow(window);


	}

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


void setImGuiStyle() {
    ImGuiStyle &style = ImGui::GetStyle();

    // https://github.com/ocornut/imgui/issues/707#issuecomment-415097227
    style.Colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.06f, 0.06f, 0.06f, 0.94f);
    style.Colors[ImGuiCol_ChildBg]                = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
    style.Colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.20f, 0.21f, 0.22f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.40f, 0.40f, 0.40f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.18f, 0.18f, 0.18f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
    style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.94f, 0.94f, 0.94f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.86f, 0.86f, 0.86f, 1.00f);
    style.Colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.46f, 0.47f, 0.48f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_Header]                 = ImVec4(0.70f, 0.70f, 0.70f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.70f, 0.70f, 0.70f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]           = ImVec4(0.48f, 0.50f, 0.52f, 1.00f);
    style.Colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.72f, 0.72f, 0.72f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);
    style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.73f, 0.60f, 0.15f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.87f, 0.87f, 0.87f, 0.35f);
    style.Colors[ImGuiCol_ModalWindowDimBg]   = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavHighlight]           = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);

    style.WindowPadding            = ImVec2(8, 6);
    style.WindowRounding           = 0.0f;
    style.FramePadding             = ImVec2(5, 7);
    //style.FrameRounding            = 0.0f;
    style.ItemSpacing              = ImVec2(5, 5);
    // style.ItemInnerSpacing         = ImVec2(1, 1);
    // style.TouchExtraPadding        = ImVec2(0, 0);
    // style.IndentSpacing            = 6.0f;
    // style.ScrollbarSize            = 12.0f;
    // style.ScrollbarRounding        = 16.0f;
    // style.GrabMinSize              = 20.0f;
    // style.GrabRounding             = 2.0f;
    // style.WindowTitleAlign.x = 0.50f;
    // style.FrameBorderSize = 0.0f;
    // style.WindowBorderSize = 1.0f;
}
