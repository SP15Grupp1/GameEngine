#include "stdafx.h"
#include "SprintReviewScene.h"
#include <Engine\Component\Factory\ComponentFactory.h>
#include <Engine/Model/ModelInstance.h>
#include <Engine\Component\ModelComponent.h>
#include <Engine\Component\LightComponent.h>
#include <Engine\GameObject\GameObject.h>
#include <Engine\Model\AssimpModel.h>
#include <Engine/Effect/StandardEffect.h>
#include <Engine\Camera/Camera.h>



SprintReviewScene::SprintReviewScene()
{
	CreateFactories();
}

SprintReviewScene::~SprintReviewScene()
{
}

void SprintReviewScene::Update(const Time & aDeltaTime)
{
	Scene::Update(aDeltaTime);
}

void SprintReviewScene::Render()
{
	Scene::Render();
}

void SprintReviewScene::CreateFactories()
{
	PreCreateComponentFactory<ModelComponent>();
	PreCreateComponentFactory<LightComponent>();

	std::shared_ptr<GameObject> enemy = std::make_shared<GameObject>();

	SharedPtrComponent<ModelComponent> prettyModel(GetComponentFactory<ModelComponent>()->CreateComponent());
	std::shared_ptr<AssimpModel> model = std::make_shared<AssimpModel>(myEffect, "models/Modelviewer_Exempelmodell/K11_1415.fbx");
	prettyModel->SetModel(model);

	SharedPtrComponent<ModelComponent> moarModel(GetComponentFactory<ModelComponent>()->CreateComponent());
	std::shared_ptr<AssimpModel> actualModel = std::make_shared<AssimpModel>(myEffect, "models/Stefan/testSpheres.fbx");
	moarModel->SetModel(actualModel);

	enemy->AddComponent<ModelComponent>(prettyModel);
	enemy->AddComponent<ModelComponent>(moarModel);

	myObjects.Add(enemy);

	SetCameraOrientation(model->GetBoundingBox().GetCenter() + Vector3f(0.f, 0.f, -model->GetBoundingBox().GetSize().z * 1.5f));
}