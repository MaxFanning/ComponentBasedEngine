#ifndef MODEL_COMPONENT_HEADER_H
#define MODEL_COMPONENT_HEADER_H

#include "Component.h"
#include "Model.h"
#include "EngineMath.h"

class TransformComponent;

class ModelComponent : public Component
{
public:
	REGISTER_TYPE(MODL);

	static Component* CreateFunc(const TiXmlNode* node);

	ModelComponent();
	~ModelComponent() override;

	void Initialize() override;
	void Terminate() override;
	void Render() override;

	void SetModelDirectory(char* direc);
	void SetScale(const f32& scale);
	void SetOffset(const Math::Vector3& offset);
	void SetRotY(const float& rotY);

private:
	char* mModelDirectory;
	Model* mModel;
	TransformComponent* mTransform;

	float mRotationY;
	f32 mScale;
	Math::Vector3 mOffset;

};

#endif //!MODEL_COMPONENT_HEADER_H
