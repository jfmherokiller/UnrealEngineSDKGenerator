#include <Windows.h>

#include "PatternFinder.hpp"
#include "ObjectsStore.hpp"

#include "EngineClasses.hpp"

class FUObjectItem
{
public:
	UObject* Object;
	int Flags;
	int ClusterRootIndex;
	int SerialNumber;
};

class TUObjectArray
{
public:
	FUObjectItem* Objects;
	int32_t MaxElements;
	int32_t NumElements;
};

class FUObjectArray
{
public:
	__int32 ObjFirstGCIndex; //0x0000
	__int32 ObjLastNonGCIndex; //0x0004
	__int32 MaxObjectsNotConsideredByGC; //0x0008
	__int32 OpenForDisregardForGC; //0x000C

	TUObjectArray ObjObjects; //0x0010
};

FUObjectArray* GlobalObjects = nullptr;

bool ObjectsStore::Initialize()
{
	auto address = FindPattern(GetModuleHandleW(nullptr), reinterpret_cast<const unsigned char*>("\x48\x8D\x0D\x00\x00\x00\x00\xC6\x05\x00\x00\x00\x00\x01\xE8\x00\x00\x00\x00\xC6\x05"), "xxx????xx????xx????xx");
	if (address == -1)
	{
		return false;
	}
	auto offset = *reinterpret_cast<uint32_t*>(address + 3);
	GlobalObjects = reinterpret_cast<decltype(GlobalObjects)>(address + 7 + offset);

	return true;
}

void* ObjectsStore::GetAddress()
{
	return GlobalObjects;
}

size_t ObjectsStore::GetObjectsNum() const
{
	return GlobalObjects->ObjObjects.NumElements;
}

UEObject ObjectsStore::GetById(size_t id) const
{
	return GlobalObjects->ObjObjects.Objects[id].Object;
}
