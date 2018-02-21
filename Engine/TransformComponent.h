#ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H
#define INCLUDED_ENGINE_TRANSFORMCOMPONENT_H

#include "Component.h"
#include "EngineMath.h"

class TransformComponent : public Component
{
public:
	REGISTER_TYPE(TRFM);

	static Component* CreateFunc(const TiXmlNode* node);

	TransformComponent();
	~TransformComponent() override;

	void Update(f32 deltaTime) override;

	Math::Matrix GetLocalToWorld() const;

	void SetPosition(const Math::Vector3& pos) { mPosition = pos; }
	void SetForward(const Math::Vector3& forward) { mForward = forward; }
	void SetSnapToTerrain(bool snapToTerrain) { mSnapToTerrain = snapToTerrain; }

	const Math::Vector3& GetPosition() const { return mPosition; }
	const Math::Vector3& GetForward() const { return mForward; }

private:
	Math::Vector3 mPosition;
	Math::Vector3 mForward;
	bool mSnapToTerrain;
};

#endif // #ifndef INCLUDED_ENGINE_TRANSFORMCOMPONENT_H