#include "stdafx.h"
#include "FreeSpaceCameraController.h"

FreeSpaceCameraController::FreeSpaceCameraController(const float aMovementSpeed, const float aRotationSpeed)
{
	myDriftMovementSpeed = aMovementSpeed;
	myDriftRotationSpeed = aRotationSpeed;

	myCurrentMovementSpeed = aMovementSpeed;
	myCurrentRotationSpeed = aRotationSpeed;

	myBoostMovementSpeed = myDriftMovementSpeed * 7.5f;
	myBoostRotationSpeed = myDriftRotationSpeed / 2.f;

	myRotateLeft = false;
	myRotateRight = false;
	myMoveLeft = false;
	myMoveRight = false;
	myMoveForward = false;
	myMoveBackward = false;

	myPitchForward = false;
	myPitchBackward = false;
	myRollLeft = false;
	myRollRight = false;
	myMoveDown = false;
	myMoveUp = false;

	myBoosting = false;
}

FreeSpaceCameraController::~FreeSpaceCameraController()
{
}

CameraControllerResult FreeSpaceCameraController::Update(const Time & aDeltaTime, Camera & aCamera)
{
	UpdateAcceleration(aDeltaTime);

	if (myRollLeft)
	{
		aCamera.GetOrientation().RotateZ(-myDriftRotationSpeed * aDeltaTime.InSeconds());
	}
	if (myRollRight)
	{
		aCamera.GetOrientation().RotateZ(myDriftRotationSpeed * aDeltaTime.InSeconds());
	}

	if (myMoveLeft)
	{
		aCamera.AddPosition(aCamera.GetOrientation().GetLeft() * myCurrentMovementSpeed * aDeltaTime.InSeconds());
	}
	if (myMoveRight)
	{
		aCamera.AddPosition(aCamera.GetOrientation().GetRight() * myCurrentMovementSpeed * aDeltaTime.InSeconds());
	}

	if (myMoveForward)
	{
		aCamera.AddPosition(aCamera.GetOrientation().GetForward() * myCurrentMovementSpeed * aDeltaTime.InSeconds());
	}
	if (myMoveBackward)
	{
		aCamera.AddPosition(aCamera.GetOrientation().GetBackward() * myCurrentMovementSpeed * aDeltaTime.InSeconds());
	}

	if (myMoveUp)
	{
		aCamera.AddPosition(aCamera.GetOrientation().GetUpward() * myCurrentMovementSpeed * aDeltaTime.InSeconds());
	}
	if (myMoveDown)
	{
		aCamera.AddPosition(aCamera.GetOrientation().GetDownward() * myCurrentMovementSpeed * aDeltaTime.InSeconds());
	}

	if (myPitchForward)
	{
		aCamera.GetOrientation().RotateX(myCurrentRotationSpeed * aDeltaTime.InSeconds());
	}
	if (myPitchBackward)
	{
		aCamera.GetOrientation().RotateX(-myCurrentRotationSpeed * aDeltaTime.InSeconds());
	}
	if (myRotateLeft)
	{
		aCamera.GetOrientation().RotateY(myCurrentRotationSpeed * aDeltaTime.InSeconds());
	}
	if (myRotateRight)
	{
		aCamera.GetOrientation().RotateY(-myCurrentRotationSpeed * aDeltaTime.InSeconds());
	}

	return CameraControllerResult::eKeepControl;
}

ReceiveResult FreeSpaceCameraController::Receive(const AnyKeyDownMessage & aMessage)
{
	switch (aMessage.key)
	{
	case KeyboardKey::eQ:
		myRollLeft = true;
		break;
	case KeyboardKey::eE:
		myRollRight = true;
		break;

	case KeyboardKey::eA:
		myMoveLeft = true;
		break;
	case KeyboardKey::eD:
		myMoveRight = true;
		break;
	case KeyboardKey::eW:
		myMoveForward = true;
		break;
	case KeyboardKey::eS:
		myMoveBackward = true;
		break;

	case KeyboardKey::eLeft:
		myRotateLeft = true;
		break;
	case KeyboardKey::eRight:
		myRotateRight = true;
		break;
	case KeyboardKey::eUp:
		myPitchForward = true;
		break;
	case KeyboardKey::eDown:
		myPitchBackward = true;
		break;

	case KeyboardKey::eControl:
		myMoveDown = true;
		break;
	case KeyboardKey::eSpace:
		myMoveUp = true;
		break;

	case KeyboardKey::eShift:
		SetIsBoosting(true);
		break;

	case KeyboardKey::eAlt:
		Engine::GetLogger().LogInfo("eAlt pressed");
		break;
	}
	return ReceiveResult::eContinue;
}

ReceiveResult FreeSpaceCameraController::Receive(const AnyKeyUpMessage & aMessage)
{
	switch (aMessage.key)
	{
	case KeyboardKey::eQ:
		myRollLeft = false;
		break;
	case KeyboardKey::eE:
		myRollRight = false;
		break;

	case KeyboardKey::eA:
		myMoveLeft = false;
		break;
	case KeyboardKey::eD:
		myMoveRight = false;
		break;
	case KeyboardKey::eW:
		myMoveForward = false;
		break;
	case KeyboardKey::eS:
		myMoveBackward = false;
		break;

	case KeyboardKey::eLeft:
		myRotateLeft = false;
		break;
	case KeyboardKey::eRight:
		myRotateRight = false;
		break;
	case KeyboardKey::eUp:
		myPitchForward = false;
		break;
	case KeyboardKey::eDown:
		myPitchBackward = false;
		break;

	case KeyboardKey::eControl:
		myMoveDown = false;
		break;
	case KeyboardKey::eSpace:
		myMoveUp = false;
		break;

	case KeyboardKey::eShift:
		SetIsBoosting(false);
		break;
	}
	return ReceiveResult::eContinue;
}

void FreeSpaceCameraController::SetIsBoosting(const bool aIsBoosting)
{
	myBoosting = aIsBoosting;

	//Uncomment movement speed change below to disable gradual acceleration in favor of instant boost speed on/off
	if (aIsBoosting == true)
	{
		//myCurrentMovementSpeed = myBoostMovementSpeed;
		myCurrentRotationSpeed = myBoostRotationSpeed;
	}
	else
	{
		//myCurrentMovementSpeed = myDriftMovementSpeed;
		myCurrentRotationSpeed = myDriftRotationSpeed;
	}
}

void FreeSpaceCameraController::UpdateAcceleration(const Time & aDeltaTime)
{
	const float AccelerationSpeed = (myBoostMovementSpeed - myDriftMovementSpeed) / 1.f;
	const float DecelerationSpeed = AccelerationSpeed * 2.f;

	if (myBoosting == true)
	{
		if (myCurrentMovementSpeed < myBoostMovementSpeed)
		{
			myCurrentMovementSpeed += AccelerationSpeed * aDeltaTime.InSeconds();

			if (myCurrentMovementSpeed > myBoostMovementSpeed)
				myCurrentMovementSpeed = myBoostMovementSpeed;
		}
	}
	else
	{
		if (myCurrentMovementSpeed > myDriftMovementSpeed)
		{
			myCurrentMovementSpeed -= DecelerationSpeed * aDeltaTime.InSeconds();

			if (myCurrentMovementSpeed < myDriftMovementSpeed)
				myCurrentMovementSpeed = myDriftMovementSpeed;
		}
	}
}
