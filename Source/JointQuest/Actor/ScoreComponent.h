// JNU-JointQuest

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ScoreComponent.generated.h"

USTRUCT(BlueprintType)
struct FExerciseRecord
{
	GENERATED_BODY()
	FExerciseRecord() {}
	FExerciseRecord(bool bHasSucceeded, float PeakAngle)
			:bHasSucceeded(bHasSucceeded), PeakAngle(PeakAngle) {}
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Exercise")
	bool bHasSucceeded;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Exercise")
	float PeakAngle;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOINTQUEST_API UScoreComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UScoreComponent();

	void RecordCurrentRep(bool bHasSucceeded);
	void AwardPoints(int32 Points);
	float GetCurrentScore() const;
	TArray<FExerciseRecord> GetAllRecords() const;
	
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 GoalReps = 20;
	
	UPROPERTY()
	TArray<FExerciseRecord> ExerciseRecords;
	
	float CurrentScore = 0.0f;
};

