// JNU-JointQuest

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"

class UJointQuestGameInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOINTQUEST_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScoreComponent();

	void RecordCurrentRep(bool bHasSucceeded);
	void AwardPoints(int32 Points);
	float GetCurrentScore() const;

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 GoalReps = 20;

	UPROPERTY()
	UJointQuestGameInstance* GameInstance;
	
	float CurrentScore = 0.0f;
	float MaxRepScore = 0.0f;
};

