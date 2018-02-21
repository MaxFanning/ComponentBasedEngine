#include "Precompiled.h"
#include "ServiceFactory.h"

#include <Engine.h>
#include <Utils.h>
#include "XMLReader.h"
#include "Service.h"

ServiceFactory::ServiceFactory(ServiceAllocator& gAllocator)
	: mServiceAllocator(gAllocator)
{
}

void ServiceFactory::Register(const char* name, CreateFunc service)
{
	mCreateFuncMap.insert({ name, service });
}

Service* ServiceFactory::Create(const char* templateFilename)
{
		TiXmlDocument document(templateFilename);

	if (!document.LoadFile())
	{
		LOG("[GameObjectFactory] Failed to open template file '%s'.", templateFilename);
		return nullptr;
	}
	Service* service = mServiceAllocator.New();
	
	const TiXmlNode* serviceNode = document.FirstChild();
	if (Utils::AreEqual(serviceNode->Value(), "Service")) //if the node is a gameobject 
	{
		serviceNode = serviceNode->FirstChild();

		const char* serviceName = serviceNode->Value();
		auto function = mCreateFuncMap[serviceName];

		Service* newService = function(serviceNode);
		service = newService;
	}

	return service;
}

void ServiceFactory::Destroy(Service* newObject)
{
	mServiceAllocator.Delete(newObject);
}