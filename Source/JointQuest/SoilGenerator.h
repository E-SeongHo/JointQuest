
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SoilGenerator.generated.h"

class AMovableActor;

UCLASS()
class JOINTQUEST_API USoilGenerator : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	//virtual void Shutdown() override;

	void GenerateSoils();
	
private:
	TSubclassOf<AMovableActor>* LeftBlockTemplate;
	TSubclassOf<AMovableActor>* RightBlockTemplate;
	TSubclassOf<AMovableActor>* BackBlockTemplate;
	
	TQueue<AMovableActor*> LeftSoilsQueue;
	TQueue<AMovableActor*> RightSoilsQueue;
	TQueue<AMovableActor*> BackSoilsQueue;

	uint32 GeneratedBlocks = 0;
	
	const uint32 GenAmount = 10;
	const uint32 GenThreshold = 5; 
	
	float BlockHeight = 100.0f; // hack : for test
	FVector2D LeftGenPos;
	FVector2D RightGenPos;
	FVector2D BackGenPos;

};
