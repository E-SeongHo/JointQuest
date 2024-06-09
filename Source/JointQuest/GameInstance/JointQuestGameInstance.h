// JNU-JointQuest

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "JointQuestGameInstance.generated.h"

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

UCLASS()
class JOINTQUEST_API UJointQuestGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	void RecordCurrentRep(FExerciseRecord NewRecord);	
	const TArray<FExerciseRecord>& GetRecords() const;
	
	// Graph drawing helper functions
	UFUNCTION(BlueprintCallable, Category = "Records")
	float GetRecordAngleRate(const int32 idx) const;
	UFUNCTION(BlueprintCallable, Category = "Records")
	bool HasRecordSucceeded(const int32 idx) const;
	UFUNCTION(BlueprintCallable, Category = "Records")
	float GetGameScore();
	UFUNCTION(BlueprintCallable, Category = "Records")
	float GetGameMaxAngle();

	UFUNCTION(BlueprintCallable, Category = "Reset")
	void CleanUp();
	
	int32 GetBestRepIndex();
	void SetCurrentRepAsBestRep();

	void GameEnd(const float GameScore);
	
private:
	float PlayerLimitAngle = 90.0f; // reads from file when game started
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Records", meta = (AllowPrivateAccess = "true"))
	TArray<FExerciseRecord> ExerciseRecords;
	int32 BestRepIndex = 0;

	// hack
	float Score;
	float MaxAngle;
};




