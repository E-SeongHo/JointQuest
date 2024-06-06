// JNU-JointQuest

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CaptureComponent.generated.h"


class UJointQuestGameInstance;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class JOINTQUEST_API UCaptureComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCaptureComponent();
	void BeginCapture();
	void EndCapture();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

private:
	bool SaveTextureToBinary(UTexture2D* Texture, const FString& FilePath);
	bool SaveTextureToPNG(UTexture2D* Texture, const FString& FilePath);
	
private:
	bool bIsCapturing = false;
	int32 CapturingIndex;
	int32 FrameIndex;
	float TargetFrameInterval = 1.0f / 30.0f; // 30fps
	float TimeElapsedFromLastCapture;
	FString CaptureDirectory;
	
	UPROPERTY()
	UJointQuestGameInstance* GameInstance;
};
