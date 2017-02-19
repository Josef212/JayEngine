#include "ComponentResource.h"
#include "Resource.h"
#include "FileParser.h"

ComponentResource::~ComponentResource()
{}

const Resource* ComponentResource::GetResource()const
{
	return resource;
}

UID ComponentResource::GetResourceUID()const
{
	return resource->GetUID();
}

void ComponentResource::OnSaveRes(FileParser& file)const
{
	file.AddInt("resource_id", resource->GetUID());
}

void ComponentResource::OnLoadRes(FileParser* file)
{
	SetResource(file->GetInt("resource_id", 0));
}
