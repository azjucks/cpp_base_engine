#include <application.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <functional>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <time_manager.h>
#include <input.h>

#include <renderer.h>

// Constructor
Application::Application(unsigned int scr_width, unsigned int scr_height)
{
	this->scr_width = scr_width;
	this->scr_height = scr_height;

	window = nullptr;

	graph = Graph::instance();
}

Application::~Application()
{
	graph->kill();
	PhysicsEngine::instance()->kill();
	ResourcesManager::instance()->kill();
	TimeManager::instance()->kill();
	Input::instance()->kill();
	Renderer::instance()->kill();

	Log::instance()->save_logs("logs/");
	Log::instance()->kill();
}


// ImGui Frame handlers
// --------------------
static void newFrame()
{
	glfwPollEvents();
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

static void endFrame(GLFWwindow* window)
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	glfwSwapBuffers(window);
}

static void closeImGui()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// Window & ImGui initialization
// -----------------------------
int Application::initialize()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation
	// --------------------
	window = glfwCreateWindow(scr_width, scr_height, "PlatformerGL - PACA - Guillaume CHICLET - Kevin PAWLONKA", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// Setup Dear ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking

	//ImGui::StyleColorsClassic();
	ImGui::StyleColorsDark();
	ImGuiStyle& style = ImGui::GetStyle();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	return 0;
}

// Application rendering loop
// --------------------------
void Application::renderLoop()
{
	// Time
	// ----
	
	Input::instance()->initialize(window);


	glBindVertexArray(0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		newFrame();

		TimeManager::instance()->updateTime();

		// input
		// -----
		//CameraInputs cameraInput = {};
		//processInput(window, mouseInput, cameraInput, deltaTime);
		Input::instance()->update();

		// render
		// ------
		graph->update();

		endFrame(window);
	}

	closeImGui();

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
}

/*
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window, MouseInput& mouseInput, CameraInputs& camInputs, const float deltaTime)
{
	//if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	//	glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		mouseInput.mouseCaptured = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS)
	{
		mouseInput.mouseCaptured = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		double newMouseX, newMouseY;
		glfwGetCursorPos(window, &newMouseX, &newMouseY);
		mouseInput.mouseX = newMouseX;
		mouseInput.mouseY = newMouseY;
	}

	int width, height;
	glfwGetWindowSize(window, &width, &height);
	camInputs.aspect = (float)width / (float)height;

	camInputs.deltaX = 0;
	camInputs.deltaY = 0;

	if (mouseInput.mouseCaptured)
	{ // Camera inputs
		double newMouseX, newMouseY;
		float mouseDeltaX = 0.0;
		float mouseDeltaY = 0.0;

		glfwGetCursorPos(window, &newMouseX, &newMouseY);
		mouseDeltaX = (float)(newMouseX - mouseInput.mouseX);
		mouseDeltaY = (float)(newMouseY - mouseInput.mouseY);
		mouseInput.mouseX = newMouseX;
		mouseInput.mouseY = newMouseY;

		camInputs.deltaX = mouseDeltaX;
		camInputs.deltaY = mouseDeltaY;

		camInputs.moveLeft = glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A);
		camInputs.moveRight = glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D);
		camInputs.moveForward = glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W);
		camInputs.moveBackward = glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S);
		camInputs.moveUp = glfwGetKey(window, GLFW_KEY_SPACE);
		camInputs.moveDown = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL);
		camInputs.moveFaster = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
	}
}
*/

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}