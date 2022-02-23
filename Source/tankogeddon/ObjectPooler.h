#pragma once
#include "CoreMinimal.h"

class TANKOGEDDON_API FObjectPooler //: public FGCObject // for added reflection
{
private:
	int32 PoolSize{ 0 };
	TSubclassOf<class AActor*> ObjectType;	///potentially replace aactor with apoolableactor

	TArray<class AActor*> ObjectStorage;
	TMap<UClass*, TArray<class AActor*>> InactiveObjectStorage;
	
public:
	FObjectPooler();
	~FObjectPooler();	// allegedly, clean up arrays and objects, because this is not included in reflection

	void Init(int32 NewPoolSize, TSubclassOf<AActor*> NewObjType);
	AActor* GetNewObject(UClass* Class, const FTransform& Transform);
	void DeactivateObject(AActor* Object);

	/*void AddReferencedObjects(FReferenceCollector& Collector) override
	{
		Collector.AddReferencedObject(SpecificSafeObject);
		//also in declaration:
		//UObject* SpecificSafeObject
		//FNormalClass(UObject* Object) : SpecificSafeObject(Object)
	}*/
		
};
