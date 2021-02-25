// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

#define DEBUG_SCALE_FACTOR 25.0f

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

	textureMgr->loadTexture(L"wood", L"res/wood.png");
	textureMgr->loadTexture(L"metal", L"res/metal.png");
	textureMgr->loadTexture(L"stone", L"res/stone.png");
	textureMgr->loadTexture(L"bunny", L"res/bunny.png");


	set = aiCore->generateParameterSet();
	profile = new DnDProfile(set);

	shader = new LightShader(renderer->getDevice(), hwnd);

	hiltMesh = new HiltMeshGenerator(renderer->getDevice(), renderer->getDeviceContext(), set);
	hiltMesh->addTexture(textureMgr->getTexture(L"wood"));
	hiltMesh->addTexture(textureMgr->getTexture(L"metal"));
	hiltMesh->addTexture(textureMgr->getTexture(L"stone"));

	pommel = new PommelMeshGenerator(set, renderer->getDevice(), renderer->getDeviceContext());

	light = new Light;
	light->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDiffuseColour(0.75f, 0.75f, 0.75f, 1.0f);
	light->setDirection(1.0f, -0.0f, 0.0f);

	camera->setPosition(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z - 150);

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

	//Camera controls
	if (input->isKeyDown(VK_DOWN))
	{
		camera->setPosition(camera->getPosition().x, camera->getPosition().y - 0.5f, camera->getPosition().z);
	}
	else if (input->isKeyDown(VK_UP))
	{
		camera->setPosition(camera->getPosition().x, camera->getPosition().y + 0.5f, camera->getPosition().z);
	}
	else if (input->isKeyDown(VK_LEFT))
	{
		camera->setPosition(camera->getPosition().x - 0.5f, camera->getPosition().y, camera->getPosition().z);
	}
	else if (input->isKeyDown(VK_RIGHT))
	{
		camera->setPosition(camera->getPosition().x + 0.5f, camera->getPosition().y, camera->getPosition().z);
	}
	else if (input->isKeyDown(VK_NUMPAD7))
	{
		camera->setPosition(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z + 1.0f);
	}
	else if (input->isKeyDown(VK_NUMPAD9))
	{
		camera->setPosition(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z - 1.0f);
	}
	else if (input->isKeyDown(VK_NUMPAD1))
	{
		camera->setRotation(camera->getRotation().x, camera->getRotation().y - 1, camera->getRotation().z);
	}
	else if (input->isKeyDown(VK_NUMPAD3))
	{
		camera->setRotation(camera->getRotation().x, camera->getRotation().y + 1, camera->getRotation().z);
	}
	else if (input->isKeyDown(VK_NUMPAD2))
	{
		camera->setRotation(camera->getRotation().x + 1, camera->getRotation().y, camera->getRotation().z);
	}
	else if (input->isKeyDown(VK_NUMPAD8))
	{
		camera->setRotation(camera->getRotation().x - 1, camera->getRotation().y, camera->getRotation().z);
	}
	else if (input->isKeyDown(VK_NUMPAD4))
	{
		camera->setRotation(camera->getRotation().x, camera->getRotation().y, camera->getRotation().z + 1);
	}
	else if (input->isKeyDown(VK_NUMPAD6))
	{
		camera->setRotation(camera->getRotation().x, camera->getRotation().y, camera->getRotation().z - 1);
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

	//Render the hilt
	hiltMesh->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, hiltMesh->getTexture(), light);
	shader->render(renderer->getDeviceContext(), hiltMesh->getIndexCount());

	if (set->getHWStyle() >= 1 && set->getHStyle() != 1)
	{
		hiltMesh->getWrapMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bunny"), light);
		shader->render(renderer->getDeviceContext(), hiltMesh->getWrapMesh()->getIndexCount());

		hiltMesh->getCapMesh(0)->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, hiltMesh->getTexture(), light);
		shader->render(renderer->getDeviceContext(), hiltMesh->getCapMesh(0)->getIndexCount());

		hiltMesh->getCapMesh(1)->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, hiltMesh->getTexture(), light);
		shader->render(renderer->getDeviceContext(), hiltMesh->getCapMesh(1)->getIndexCount());

	}
	else
	{
		//Render the deform meshes
		if (set->getHStyle() == 1)
		{
			hiltMesh->getDeformMesh(0)->sendData(renderer->getDeviceContext());
			shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bunny"), light);
			shader->render(renderer->getDeviceContext(), hiltMesh->getDeformMesh(0)->getIndexCount());

			hiltMesh->getDeformMesh(1)->sendData(renderer->getDeviceContext());
			shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bunny"), light);
			shader->render(renderer->getDeviceContext(), hiltMesh->getDeformMesh(1)->getIndexCount());

		}
	}

	//Render the pommel
	if (set->getPStyle() == 1 || set->getPStyle() == 3)
	{
		float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;
		worldMatrix = XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), XMMatrixTranslation(0.0f, 0.0f, -140.0f - (radius / 2)));

		pommel ->getMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"bunny"), light);
		shader->render(renderer->getDeviceContext(), pommel->getMesh()->getIndexCount());
	}

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

