#include "stdafx.h"
#include "SoundManger.h"
#include "Audio/WwiseManager.h"


SoundManger::SoundManger()
{
	myWwiseManager = std::make_unique<CWwiseManager>();
}

SoundManger::~SoundManger()
{
}

bool SoundManger::Init(const char* aInitBank)
{
	bool success = myWwiseManager->Init(aInitBank);
	myWwiseManager->SetErrorCallBack(&SoundManger::ErrorCallback);
	return success;
}

void SoundManger::Update()
{
	myWwiseManager->Update();
}
bool SoundManger::LoadBank(const char* aBankPath)
{
	return myWwiseManager->LoadBank(aBankPath);
}

void SoundManger::UnLoadBank(const char* aBankPath)
{
	return myWwiseManager->UnLoadBank(aBankPath);
}

void SoundManger::PostEvent(const char* aEvent)
{
	return myWwiseManager->PostEvent(aEvent);
}

void SoundManger::ErrorCallback(const char* aError)
{
	std::string temp = aError;
	std::wstring errorMsg(temp.begin(), temp.end());
	OutputDebugString(errorMsg.c_str());
}
