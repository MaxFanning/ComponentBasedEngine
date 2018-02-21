#ifndef SERVICE_FACTORY_HEADER_H
#define SERVICE_FACTORY_HEADER_H

#include "Service.h"

class ServiceFactory
{
public:
	typedef std::function<Service*(const TiXmlNode*)> CreateFunc;

	ServiceFactory(ServiceAllocator& gAllocator);

	void Register(const char* name, CreateFunc service);

	Service* Create(const char* templateFilename);
	void Destroy(Service* newObject);

private:
	typedef std::unordered_map<std::string, CreateFunc> CreateFuncMap;

	ServiceAllocator& mServiceAllocator;
	CreateFuncMap mCreateFuncMap;
};

#endif
