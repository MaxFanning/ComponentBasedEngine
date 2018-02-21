#include "Precompiled.h"

#include "CameraComponent.h"
#include "MeshRenderer.h"

CameraComponent* CameraComponent::main = nullptr;

Component* CameraComponent::CreateFunc(const TiXmlNode* node)
{
	CameraComponent* newComponent = new CameraComponent;

	return newComponent;
}

//----------------------------------------------------------------------------------------------------

CameraComponent::CameraComponent()
	: mPosition(0.0f, 0.0f, 0.0f)
	, mLook(0.0f, 0.0f, 1.0f)
	, mRight(1.0f, 0.0f, 0.0f)
	, mFOV(60.0f * Math::kDegToRad)
	, mAspectRatio(4.0f / 3.0f)
	, mNearPlane(0.01f)
	, mFarPlane(1000.0f)
	, mOffset(Math::Vector3::Zero())
{
	if (main == nullptr)
	{
		main = this;
	}
}

//----------------------------------------------------------------------------------------------------

CameraComponent::~CameraComponent()
{
	
}

//----------------------------------------------------------------------------------------------------

void CameraComponent::Setup(f32 fov, f32 aspectRatio, f32 nearPlane, f32 farPlane)
{
	mFOV = fov;
	mAspectRatio = aspectRatio;
	mNearPlane = nearPlane;
	mFarPlane = farPlane;
}

//----------------------------------------------------------------------------------------------------

void CameraComponent::SetFOV(f32 fov)
{
	const f32 kMinFOV = 10.0f * Math::kDegToRad;
	const f32 kMaxFOV = 150.0f * Math::kDegToRad;
	mFOV = Math::Clamp(fov, kMinFOV, kMaxFOV);
}

//----------------------------------------------------------------------------------------------------

Math::Matrix CameraComponent::GetViewMatrix() const
{
	const Math::Vector3 l = mLook;
	const Math::Vector3 r = mRight;
	const Math::Vector3 u = Math::Normalize(Math::Cross(mLook, mRight));
	const f32 dx = -Math::Dot(r, mPosition + mOffset);
	const f32 dy = -Math::Dot(u, mPosition + mOffset);
	const f32 dz = -Math::Dot(l, mPosition + mOffset);

	return Math::Matrix
	(
		r.x, u.x, l.x, 0.0f,
		r.y, u.y, l.y, 0.0f,
		r.z, u.z, l.z, 0.0f,
		dx, dy, dz, 1.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Matrix CameraComponent::GetProjectionMatrix() const
{
	const f32 h = 1 / tan(mFOV * 0.5f);
	const f32 w = h / mAspectRatio;
	const f32 f = mFarPlane;
	const f32 n = mNearPlane;
	const f32 d = f / (f - n);

	return Math::Matrix
	(
		w, 0.0f, 0.0f, 0.0f,
		0.0f, h, 0.0f, 0.0f,
		0.0f, 0.0f, d, 1.0f,
		0.0f, 0.0f, -n * d, 0.0f
	);
}

//----------------------------------------------------------------------------------------------------

Math::Ray CameraComponent::ScreenPointToRay(s32 screenX, s32 screenY, u32 screenWidth, u32 screenHeight) const
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

void CameraComponent::Renormalize()
{
	mLook = Math::Normalize(mLook);
	mRight = Math::Normalize(Math::Cross(Math::Vector3::YAxis(), mLook));
}

//----------------------------------------------------------------------------------------------------

void CameraComponent::SetOffset(const Math::Vector3& offset)
{
	mOffset = offset;
}