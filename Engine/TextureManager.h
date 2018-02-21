#ifndef INCLUDED_ENGINE_TEXTUREMANAGER_H
#define INCLUDED_ENGINE_TEXTUREMANAGER_H

typedef std::size_t TextureId;

class Texture;

class TextureManager
{
public:
	static void StaticInitialize(const char* root);
	static void StaticTerminate();
	static TextureManager* Get();

public:
	TextureManager();
	~TextureManager();

	void Initialize(const char* path);
	void Terminate();

	TextureId Load(const char* fileName);

	Texture* GetTexture(TextureId id);

private:
	TextureManager(const TextureManager&) = delete;
	TextureManager& operator=(const TextureManager&) = delete;

	std::string mRoot;
	std::unordered_map<std::size_t, Texture*> mInventory;
};

#endif // #ifndef INCLUDED_ENGINE_TEXTUREMANAGER_H