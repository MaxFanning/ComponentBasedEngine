#ifndef INCLUDED_ENGINE_CAMERACOMPONENT_H
#define INCLUDED_ENGINE_CAMERACOMPONENT_H

#include "Component.h"
#include "EngineMath.h"

class CameraComponent : public Component
{
public:
	REGISTER_TYPE(CMRA);
	static CameraComponent* main;

	static Component* CreateFunc(const TiXmlNode* node);

	CameraComponent();
	~CameraComponent() override;

	void Setup(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane);

	void SetFOV(f32 fov);

	const Math::Vector3& GetPosition() const { return mPosition; }

	f32 GetFOV() const { return mFOV; }
	f32 GetAspectRatio() const { return mAspectRatio; }
	f32 GetNearPlane() const { return mNearPlane; }
	f32 GetFarPlane() const { return mFarPlane; }
	Math::Vector3 GetLook() const { return mLook; }
	Math::Vector3 GetOffset() const { return mOffset; }

	Math::Matrix GetViewMatrix() const;
	Math::Matrix GetProjectionMatrix() const;
	Math::Ray ScreenPointToRay(s32 screenX, s32 screenY, u32 screenWidth, u32 screenHeight) const;

	void SetOffset(const Math::Vector3& offset);

private:
	friend class FPSCameraControllerComponent;

	void Renormalize();

	Math::Vector3 mPosition;
	Math::Vector3 mOffset;
	Math::Vector3 mLook;
	Math::Vector3 mRight;

	f32 mFOV;
	f32 mAspectRatio;
	f32 mNearPlane;
	f32 mFarPlane;
};

#endif // #ifndef INCLUDED_ENGINE_CAMERACOMPONENT_H
