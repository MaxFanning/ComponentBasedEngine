//====================================================================================================
// Filename:	Camera.cpp
// Created by:	Peter Chan
//====================================================================================================

//====================================================================================================
// Includes
//====================================================================================================

#include "Precompiled.h"

#include "Camera.h"

//====================================================================================================
// Statics
//====================================================================================================

Camera* Camera::main = nullptr;

//====================================================================================================
// Class Definitions
//====================================================================================================

Camera::Camera()
	: mPosition(0.0f, 0.0f, 0.0f)
	, mLook(0.0f, 0.0f, 1.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mFOV(60.0f * Math::kDegToRad)
	, mAspectRatio(4.0f / 3.0f)
	, mNearPlane(0.01f)
	, mFarPlane(1000.0f)
{
	if (main == nullptr)
	{
		main = this;
	}
}

//----------------------------------------------------------------------------------------------------

Camera::~Camera()
{
	if (main == this)
	{
		main = nullptr;
	}
}

//----------------------------------------------------------------------------------------------------

void Camera::Setup(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane)
{
	mFOV = fov;
	mAspectRatio = aspectRatio;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetPosition(const Math::Vector3& position)
{
	mPosition = position;
}

//----------------------------------------------------------------------------------------------------

void Camera::SetDirection(const Math::Vector3& direction)
{
	mLook = direction;
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::SetLookAt(const Math::Vector3& target)
{
	mLook = target - mPosition;
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::Walk(f32 distance)
{
	mPosition = mPosition + (mLook * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Strafe(f32 distance)
{
	mPosition = mPosition + (mRight * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Rise(f32 distance)
{
	mPosition = mPosition + (Math::Vector3::YAxis() * distance);
}

//----------------------------------------------------------------------------------------------------

void Camera::Yaw(f32 degree)
{
	const Math::Matrix matRotate = Math::Matrix::RotationY(degree * Math::kDegToRad);
	mLook = Math::TransformNormal(mLook, matRotate);
	Renormalize();
}

//----------------------------------------------------------------------------------------------------

void Camera::Pitch(f32 degree)
{
	const Math::Matrix matRotate = Math::Matrix::RotationAxis(mRight, degree * Math::kDegToRad);
	const Math::Vector3 newLook = Math::TransformNormal(mLook, matRotate);
	const f32 dot = Math::Dot(newLook, Math::Vector3::YAxis());

	// Avoid looking straight up or down
	if (Math::Abs(dot) < 0.995f)
	{
		mLook = newLook;
		Renormalize();
	}
}

//----------------------------------------------------------------------------------------------------

void Camera::SetFOV(f32 fov)
{
	const f32 kMinFOV = 10.0f * Math::kDegToRad;
	const f32 kMaxFOV = 150.0f * Math::kDegToRad;
	mFOV = Math::Clamp(fov, kMinFOV, kMaxFOV);
}

//----------------------------------------------------------------------------------------------------

Math::Matrix Camera::GetViewMatrix() const
{
	const Math::Vector3 l = mLook;
	const Math::Vector3 r = mRight;
	const Math::Vector3 u = Math::Normalize(Math::Cross(mLook, mRight));
	const f32 dx = -Math::Dot(r, mPosition);
	const f32 dy = -Math::Dot(u, mPosition);
	const f32 dz = -Math::Dot(l, mPosition);

	return Math::Matrix
	(
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		dx,  dy,  dz,  1.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Matrix Camera::GetProjectionMatrix() const
{
	const f32 h = 1 / tan(mFOV * 0.5f);
	const f32 w = h / mAspectRatio;
	const f32 f = mFarPlane;
	const f32 n = mNearPlane;
	const f32 d = f / (f - n);

	return Math::Matrix
	(
		w,    0.0f, 0.0f,   0.0f,
		0.0f, h,    0.0f,   0.0f,
		0.0f, 0.0f, d,      1.0f,
		0.0f, 0.0f, -n * d, 0.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Ray Camera::ScreenPointToRay(s32 screenX, s32 screenY, u32 screenWidth, u32 screenHeight) const
{	
	f32 halfWidth = screenWidth * 0.5f;
	f32 halfHeight = screenHeight * 0.5f;
	f32 dx = tanf(mFOV * 0.5f) * ((f32)screenX / halfWidth - 1.0f) * mAspectRatio;
	f32 dy = tanf(mFOV * 0.5f) * (1.0f - (f32)screenY / halfHeight);

	Math::Ray ray;
	ray.org = Math::Vector3::Zero();
	ray.dir = Math::Normalize(Math::Vector3(dx, dy, 1.0f));

	Math::Matrix invMatView = Math::Inverse(GetViewMatrix());
	ray.org = Math::TransformCoord(ray.org, invMatView);
	ray.dir = Math::TransformNormal(ray.dir, invMatView);
	return ray;
}

//----------------------------------------------------------------------------------------------------

void Camera::Renormalize()
{
	mLook = Math::Normalize(mLook);
	mRight = Math::Normalize(Math::Cross(Math::Vector3::YAxis(), mLook));
}