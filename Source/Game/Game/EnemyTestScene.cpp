#include "stdafx.h"
#include "EnemyTestScene.h"
#include <Engine\Component\Factory\ComponentFactory.h>
#include <Engine/Model/ModelInstance.h>
#include <Engine\Component\ModelComponent.h>
#include <Engine\Component\LightComponent.h>
#include <Engine\GameObject\GameObject.h>
#include <Engine\Model\AssimpModel.h>
#include <Engine/Effect/StandardEffect.h>
#include <Engine\Camera/Camera.h>
#include <Engine/Scripting/ScriptFile.h>

EnemyTestScene::EnemyTestScene()
{
	CreateFactories();

	myScript = Engine::GetResourceManager().Get<ScriptFile>("Assets/Scripts/Components/TestComponent.lua")->Execute();
}

EnemyTestScene::~EnemyTestScene()
{
}

void EnemyTestScene::Update(const Time & aDeltaTime)
{
	Scene::Update(aDeltaTime);
}

void EnemyTestScene::Render()
{
	Scene::Render();
}

void EnemyTestScene::CreateFactories()
{
	std::shared_ptr<GameObject> enemy = CreateGameObject();

	//GetComponentFactory<ModelComponent>()->CreateComponent();

	SharedPtrComponent<ModelComponent> prettyModel (GetComponentFactory<ModelComponent>()->CreateComponent());
	std::shared_ptr<AssimpModel> model = std::make_shared<AssimpModel>(myEffect, "models/Modelviewer_Exempelmodell/K11_1415.fbx");
	prettyModel->SetModel(model);

	SetCameraOrientation(model->GetBoundingBox().GetCenter() + Vector3f(0.f, 0.f, -model->GetBoundingBox().GetSize().z * 1.5f));
	
	enemy->AddComponent<ModelComponent>(prettyModel);
}
