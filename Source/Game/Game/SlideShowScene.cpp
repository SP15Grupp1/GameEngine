#include "stdafx.h"
#include "SlideShowScene.h"
#include <Engine\Model\AssimpModel.h>
#include <Engine\Model\ModelInstance.h>
#include <Engine\Camera\Camera.h>
#include <Engine\Effect\StandardEffect.h>

SlideShowScene::SlideShowScene()
	: Scene("skybox.dds")
{
	auto cube = std::make_shared<SB::AssimpModel>("models/tga_companioncube/companion.fbx");
	//myObjects.push_back(std::make_shared<ModelInstance>(cube));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/crystalAsteroid/crystalAsteroidHigh.fbx")));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/pillar/pillarShortObelisk.fbx")));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/Asteroid/AsteroidPillarHuge1.fbx")));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/Asteroid/AsteroidPillarLarge1.fbx")));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/Asteroid/AsteroidPillarLarge2.fbx")));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/Asteroid/AsteroidPillarLarge3.fbx")));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/Pistol/gunMesh.fbx")));
	//myObjects.push_back(std::make_shared<ModelInstance>(std::make_shared<AssimpModel>("models/m4a1/m4a1_s.fbx")));

	myCurrentModel = 0;
	myTimePerModel = SB::Time::FromSeconds(5.0f);
	myModelTransitionTime = SB::Time::FromSeconds(1.f);
}


SlideShowScene::~SlideShowScene()
{
}

void SlideShowScene::Update(const SB::Time &aDeltaTime)
{
//	myTime += aDeltaTime;
//	myCurrentModelTime += aDeltaTime;
//
//	if (myCurrentModelTime > myTimePerModel)
//	{
//		NextModel();
//	}
//
//	Vector3f nextModelPosition;
//	for (size_t i = 0; i < myObjects.size(); i++)
//	{
//		myObjects[i]->SetMatrix(Matrix44f::CreateTranslation(-myObjects[i]->GetModel()->GetBoundingBox().GetCenter())
//			* Matrix44f::CreateRotateAroundY(myTime.InSeconds())
//			* Matrix44f::CreateTranslation(nextModelPosition));
//
//		nextModelPosition.x += myObjects[i]->GetBoundingBox().GetMaximumRadius() * 1.05f;
//
//		if (i + 1 < myObjects.size())
//		{
//			nextModelPosition.x += myObjects[i + 1]->GetBoundingBox().GetMaximumRadius() * 1.05f;
//		}
//	}
//
//	BoundingBoxf currentObjectBoundingBox = myObjects[myCurrentModel]->GetBoundingBox();
//
//	float progress = std::min(1.f, myCurrentModelTime.InSeconds() / myModelTransitionTime.InSeconds());
//	progress = (1.f + std::sinf(-HalfPi + progress * Pi)) / 2.f;
//
//	Vector3f cameraTo = currentObjectBoundingBox.GetCenter() + Vector3f(0.f, 0.f, -currentObjectBoundingBox.GetMaximumRadius() * 1.5f);
//	myCamera->SetPosition(myCameraFrom + (cameraTo - myCameraFrom) * progress);
//
	SB::Scene::Update(aDeltaTime);
}

void SlideShowScene::NextModel()
{
	//myCameraFrom = myCamera->GetPosition();
	//myCurrentModel = (myCurrentModel + 1) % myObjects.size();
	//myCurrentModelTime = 0.f;
}
