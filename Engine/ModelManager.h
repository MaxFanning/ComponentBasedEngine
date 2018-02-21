#ifndef INCLUDED_ENGINE_MODELMANAGER_H
#define INCLUDED_ENGINE_MODELMANAGER_H

typedef std::size_t ModelId;

class Model;

class ModelManager
{
public:
	static void StaticInitialize(const char* root);
	static void StaticTerminate();
	static ModelManager* Get();

public:
	ModelManager();
	~ModelManager();

	void Initialize(const char* path);
	void Terminate();

	ModelId Load(const char* fileName);

	Model* GetModel(ModelId id);

private:
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;

	std::string mRoot;
	std::unordered_map<std::size_t, Model*> mInventory;
};

#endif // #ifndef INCLUDED_ENGINE_MODELMANAGER_H