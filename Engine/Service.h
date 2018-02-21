#ifndef SERVICE_HEADER_H
#define SERVICE_HEADER_H

#include "RTTI.h"
#include "World.h"

class Service
{
public:
	REGISTER_TYPE(SRVC)

	Service();
	virtual ~Service();

	virtual void Initialize() {}
	virtual void Terminate() {}

	virtual void Update(f32 deltaTime) {}
	virtual void Render() {}
	virtual void Render2D() {}

	World& GetWorld() { return *mWorld; }

private:
	friend class World; // Set mWorld through World

	World* mWorld;
	std::string mName;

};

#endif
