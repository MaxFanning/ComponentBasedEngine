#ifndef INCLUDED_ENGINE_MODEL_H
#define INCLUDED_ENGINE_MODEL_H

//====================================================================================================
// Filename:	Model.h
// Created by:	Peter Chan
// Description:	Class for a 3D model.
//====================================================================================================

//====================================================================================================
// Forward Declarations
//====================================================================================================

namespace Math
{
	struct Matrix;
};
struct Bone;
class AnimationClip;
class Mesh;
class MeshBuffer;
class Texture;

//====================================================================================================
// Class Declarations
//====================================================================================================

class Model
{
public:
	Model();
	~Model();

	bool Load(const char* pFilename);
	void Unload();

	void Render(const Math::Matrix& transform) const;

	NONCOPYABLE(Model);

	std::vector<Mesh*> mMeshes;

	Bone* mRoot;
	std::vector<Bone*> mBones;
	std::map<std::string, u32> mBoneIndexMap;
	std::vector<AnimationClip*> mAnimations;

	std::vector<MeshBuffer*> mMeshBuffers;
	std::vector<Texture*> mTextures;
};

#endif // #ifndef INCLUDED_ENGINE_MODEL_H