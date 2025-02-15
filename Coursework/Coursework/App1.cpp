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
	textureMgr->loadTexture(L"grass", L"res/grass.png");
	textureMgr->loadTexture(L"height", L"res/height.png");
	textureMgr->loadTexture(L"leather", L"res/leather.png");
	textureMgr->loadTexture(L"diamond", L"res/diamond.png");
	textureMgr->loadTexture(L"ruby", L"res/ruby.png");
	textureMgr->loadTexture(L"sapphire", L"res/sapphire.png");
	textureMgr->loadTexture(L"emerald", L"res/emerald.png");
	textureMgr->loadTexture(L"quartz", L"res/quartz.png");
	textureMgr->loadTexture(L"amethyst", L"res/amethyst.png");


	shader = new LightShader(renderer->getDevice(), hwnd);
	genTimer.frame();
	generateSet();
	genTimer.frame();
	genTime = genTimer.getTime();

	light = new Light;
	light->setAmbientColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDiffuseColour(0.75f, 0.75f, 0.75f, 1.0f);
	light->setDirection(1.0f, -0.0f, 0.0f);

	camera->setPosition(camera->getPosition().x, camera->getPosition().y + 25, camera->getPosition().z - 170);

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}

void App1::generateSet()
{
	set = aiCore->generateParameterSet();
	profile = new DnDProfile(set);

	hiltMesh = new HiltMeshGenerator(renderer->getDevice(), renderer->getDeviceContext(), set);
	hiltMesh->addTexture(textureMgr->getTexture(L"wood"));
	hiltMesh->addTexture(textureMgr->getTexture(L"metal"));
	hiltMesh->addTexture(textureMgr->getTexture(L"stone"));

	pommel = new PommelMeshGenerator(set, renderer->getDevice(), renderer->getDeviceContext());

	if (set->getWType() == 0)
	{
		crossguard = new CrossguardMeshGenerator(set, renderer->getDevice(), renderer->getDeviceContext());
		sword = new SwordBladeMesh(renderer->getDevice(), renderer->getDeviceContext(), set);
	}
	else if (set->getWType() == 2)
	{
		spear = new SpeartipMeshGenerator(renderer->getDevice(), renderer->getDeviceContext(), set);
	}
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
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"leather"), light);
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
		if (set->getHStyle() == 1 && set->getHWStyle() != 2)
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
	if (set->getPStyle() == 1)
	{
		float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;
		worldMatrix = XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), XMMatrixTranslation(0.0f, 0.0f, -140.0f - (radius * 0.9)));

		ID3D11ShaderResourceView* tex;
		switch (set->getMaterial())
		{
		case 0:
			tex = textureMgr->getTexture(L"wood");
			break;
		case 1:
			tex = textureMgr->getTexture(L"metal");
			break;
		case 2:
			tex = textureMgr->getTexture(L"stone");
			break;
		default:
			tex = textureMgr->getTexture(L"metal");
			break;
		}

		pommel ->getMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
		shader->render(renderer->getDeviceContext(), pommel->getMesh()->getIndexCount());
	}
	else if (set->getPStyle() == 2 || set->getPStyle() == 5)
	{
		float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;
		worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -140.0f - (radius * 0.5));

		ID3D11ShaderResourceView* tex;
		switch (set->getMaterial())
		{
		case 0:
			tex = textureMgr->getTexture(L"wood");
			break;
		case 1:
			tex = textureMgr->getTexture(L"metal");
			break;
		case 2:
			tex = textureMgr->getTexture(L"stone");
			break;
		default:
			tex = textureMgr->getTexture(L"metal");
			break;
		}

		pommel->getMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
		shader->render(renderer->getDeviceContext(), pommel->getMesh()->getIndexCount());
	}
	else if (set->getPStyle() == 3 || set->getPStyle() == 4)
	{
		float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;
		worldMatrix = XMMatrixMultiply(XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixTranslation(0.0f, (set->getHRadius() / DEBUG_SCALE_FACTOR) - (radius / 2.5), -140.0f));

		ID3D11ShaderResourceView* tex;
		switch (set->getMaterial())
		{
			case 0:
				tex = textureMgr->getTexture(L"wood");
				break;
			case 1:
				tex = textureMgr->getTexture(L"metal");
				break;
			case 2:
				tex = textureMgr->getTexture(L"stone");
				break;
			default:
				tex = textureMgr->getTexture(L"metal");
				break;
		}

		pommel->getMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
		shader->render(renderer->getDeviceContext(), pommel->getMesh()->getIndexCount());

		worldMatrix = XMMatrixMultiply(XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixTranslation(0.0f, -(set->getHRadius() / DEBUG_SCALE_FACTOR) + (radius / 2.5), -140.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(AI_DEG_TO_RAD(180)), worldMatrix);

		pommel->getMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
		shader->render(renderer->getDeviceContext(), pommel->getMesh()->getIndexCount());

		worldMatrix = XMMatrixMultiply(XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixTranslation(-(set->getHRadius() / DEBUG_SCALE_FACTOR) + (radius / 2.5), 0.0f, -140.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(AI_DEG_TO_RAD(90)), worldMatrix);

		pommel->getMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
		shader->render(renderer->getDeviceContext(), pommel->getMesh()->getIndexCount());

		worldMatrix = XMMatrixMultiply(XMMatrixScaling(1.0f, 1.0f, 1.0f), XMMatrixTranslation((set->getHRadius() / DEBUG_SCALE_FACTOR) - (radius / 2.5), 0.0f, -140.0f));
		worldMatrix = XMMatrixMultiply(XMMatrixRotationZ(AI_DEG_TO_RAD(270)), worldMatrix);

		pommel->getMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
		shader->render(renderer->getDeviceContext(), pommel->getMesh()->getIndexCount());


	}
	if (set->getPStyle() == 4 || set->getPStyle() == 5)
	{
		ID3D11ShaderResourceView* tex;
		switch (set->getGColour())
		{
		case 0:
			tex = textureMgr->getTexture(L"diamond");
			break;
		case 1:
			tex = textureMgr->getTexture(L"ruby");
			break;
		case 2:
			tex = textureMgr->getTexture(L"sapphire");
			break;
		case 3:
			tex = textureMgr->getTexture(L"emerald");
			break;
		case 4:
			tex = textureMgr->getTexture(L"quartz");
			break;
		case 5:
			tex = textureMgr->getTexture(L"amethyst");
			break;
		default:
			tex = textureMgr->getTexture(L"diamond");
			break;
		}

		float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;
		worldMatrix = XMMatrixMultiply(XMMatrixScaling(radius, radius, radius), XMMatrixTranslation(0.0f, 0.0f, -140.0f));

		pommel->getGemMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
		shader->render(renderer->getDeviceContext(), pommel->getGemMesh()->getIndexCount());
	}

	if (set->getWType() == 0)
	{
		ID3D11ShaderResourceView* tex;
		switch (set->getMaterial())
		{
		case 0:
			tex = textureMgr->getTexture(L"wood");
			break;
		case 1:
			tex = textureMgr->getTexture(L"metal");
			break;
		case 2:
			tex = textureMgr->getTexture(L"stone");
			break;
		default:
			tex = textureMgr->getTexture(L"metal");
			break;
		}

		if (set->getCShape() == 2)
		{
			float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;
			worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR));

			crossguard->getMesh()->sendData(renderer->getDeviceContext());
			shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
			shader->render(renderer->getDeviceContext(), crossguard->getMesh()->getIndexCount());

			worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR) + ((set->getCHeight() / DEBUG_SCALE_FACTOR)));

			sword->sendData(renderer->getDeviceContext());
			shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"metal"), light);
			shader->render(renderer->getDeviceContext(), sword->getIndexCount());
		}
		else if (set->getCShape() == 0)
		{
			float radius = set->getPRadius() / DEBUG_SCALE_FACTOR;
			worldMatrix = XMMatrixMultiply(XMMatrixScaling(set->getCRadius() / DEBUG_SCALE_FACTOR, 2 * (set->getCRadius() / DEBUG_SCALE_FACTOR), set->getCHeight() / DEBUG_SCALE_FACTOR), XMMatrixTranslation(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR) + (set->getCHeight() / DEBUG_SCALE_FACTOR)));

			crossguard->getMesh()->sendData(renderer->getDeviceContext());
			shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, tex, light);
			shader->render(renderer->getDeviceContext(), crossguard->getMesh()->getIndexCount());

			worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR) + ((set->getCHeight() / DEBUG_SCALE_FACTOR)));

			sword->sendData(renderer->getDeviceContext());
			shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"metal"), light);
			shader->render(renderer->getDeviceContext(), sword->getIndexCount());
		}
		else
		{
			worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR));

			sword->sendData(renderer->getDeviceContext());
			shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"metal"), light);
			shader->render(renderer->getDeviceContext(), sword->getIndexCount());
		}
		
	}
	else if (set->getWType() == 2)
	{
		worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -140.0f + (set->getHLength() / DEBUG_SCALE_FACTOR));

		spear->getCollarMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"metal"), light);
		shader->render(renderer->getDeviceContext(), spear->getCollarMesh()->getIndexCount());

		worldMatrix = XMMatrixTranslation(0.0f, 0.0f, -145.0f + (set->getHLength() / DEBUG_SCALE_FACTOR) + (set->getBLength() / DEBUG_SCALE_FACTOR));

		spear->getSpearBaseMesh()->sendData(renderer->getDeviceContext());
		shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"metal"), light);
		shader->render(renderer->getDeviceContext(), spear->getSpearBaseMesh()->getIndexCount());
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
	ImGui::Text("Generation Time: %.2f seconds", genTime);


	std::string a = "Combinations Tried: ";
	a += to_string(aiCore->getCombinationsTried());
	ImGui::Text(a.c_str());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	if (ImGui::Button("Regenerate"))
	{
		genTimer.frame();
		generateSet();
		genTimer.frame();
		genTime = genTimer.getTime();
	}


	//Construct the event string
	const char* eventString = "";
	std::string st;
	st = "[";
	st += lastEvent.type;
	st += "] ";
	st += lastEvent.eventMessage;
	eventString = st.c_str();

	//ImGui::Text(eventString);

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

