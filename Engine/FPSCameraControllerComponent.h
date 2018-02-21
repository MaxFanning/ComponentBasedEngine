#ifndef INCLUDED_ENGINE_FPSCAMERACONTROLLERCOMPONENT_H
#define INCLUDED_ENGINE_FPSCAMERACONTROLLERCOMPONENT_H

#include "CameraComponent.h"
#include "InputSystem.h"

class TransformComponent;
class Terrain;

class FPSCameraControllerComponent : public CameraComponent
{
public:
	REGISTER_TYPE(FPSC);

	static Component* CreateFunc(const TiXmlNode* node);

	FPSCameraControllerComponent();
	~FPSCameraControllerComponent() override;

	void Update(f32 deltaTime);
	void Initialize();
	void Render();

	void SetPosition(const Math::Vector3& position);
	void SetDirection(const Math::Vector3& direction);
	void SetLookAt(const Math::Vector3& target);
	void SetMoveSpeed(const f32& speed);
	void SetTurnSpeed(const f32& speed);

	void Walk(f32 deltaTime);
	void Strafe(f32 deltaTime);
	void Rise(f32 deltaTime);

	void Yaw(s32 direction, f32 deltaTime);
	void Pitch(s32 direction, f32 deltaTime);

private:
	void PositionCheck();

	InputSystem* mInputSystem;
	TransformComponent* mTransform;
	Terrain* mTerrain;

	f32 mMoveSpeed;
	f32 mTurnSpeed;
	f32 mWidth;
	f32 mLength;

};

#endif
