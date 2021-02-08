// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::aiThreadFunc(AICore* core)
{
}


void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);
	eventSystem = new DebugEventSystem();
	aiCore = new AICore(eventSystem, "E:\\Coursework 2020-2021\\Honours-Project\\Coursework\\media\\rules.json", "E:\\Coursework 2020-2021\\Honours-Project\\Coursework\\media\\criteria.json");
	//profile = new DnDProfile(new ParameterSet);

	// Initalise scene variables.
	//eventSystem->addEvent("SYSTEM", "Hello", "main");
	lastEvent = eventSystem->getNextEvent();

	textureMgr->loadTexture(L"grass", L"res/grass.png");

	set = aiCore->generateParameterSet();
	profile = new DnDProfile(set);

	shader = new LightShader(renderer->getDevice(), hwnd);

	hiltMesh = new HiltMeshGenerator(renderer->getDevice(), renderer->getDeviceContext(), set);

	light = new Light;
	light->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDiffuseColour(0.75f, 0.75f, 0.75f, 1.0f);
	light->setDirection(1.0f, -0.0f, 0.0f);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}

bool App1::frame()
{
	timer->frame();

	deltaTime = timer->getTime();

	elapsedTime += deltaTime;
	if (elapsedTime >= eventDisplayTime)
	{
		elapsedTime = 0;
		lastEvent = eventSystem->getNextEvent();
	}
	
	bool result;

	result = BaseApplication::frame();

	//Camera controls

	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();

	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	hiltMesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"), light);
	shader->render(renderer->getDeviceContext(), hiltMesh->getIndexCount());

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);


	//Construct the event string
	const char* eventString = "";
	std::string st;
	st = "[";
	st += lastEvent.type;
	st += "] ";
	st += lastEvent.eventMessage;
	eventString = st.c_str();

	ImGui::Text(eventString);

	eventString = "";
	st = "Name: ";
	st += profile->getWeaponName();
	eventString = st.c_str();
	ImGui::Text(eventString);

	eventString = "";
	st = "Damage Bonus: ";

	if (profile->getDamageBonus() > 0)
	{
		st += "+";
	}

	st += to_string(profile->getDamageBonus());
	eventString = st.c_str();
	ImGui::Text(eventString);

	eventString = "";
	st = "Bonus Damage: ";
	
	if (profile->getEffectType() == "None")
	{
		st += "None";
	}
	else
	{
		st += profile->getEffectDice();
		st += " ";
		st += profile->getEffectType();
	}
	eventString = st.c_str();
	ImGui::Text(eventString);



	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

