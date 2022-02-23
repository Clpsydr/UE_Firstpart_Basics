#include "ObjectPooler.h"

FObjectPooler::FObjectPooler()
{
}

FObjectPooler::~FObjectPooler()
{
}

void FObjectPooler::Init(int32 NewPoolSize, TSubclassOf<AActor*> NewObjType)
{
	PoolSize = NewPoolSize;
	ObjectType = NewObjType;


	for (int32 i = 0; i < PoolSize; PoolSize++)
	{
		//instantiating new object
		//extending pool
		//ObjectStorage.Add();
	}
}

AActor* FObjectPooler::GetNewObject(UClass* Class, const FTransform& Transform)
{
	return nullptr;
}

void FObjectPooler::DeactivateObject(AActor* Object)
{
}
