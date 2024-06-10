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

	void GameEnd();
	
	UFUNCTION(BlueprintCallable, Category = "Records")
	UTexture2D* StreamPOTG();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
protected:
	virtual void BeginPlay() override;

private:
	// OPTIMIZE: If PNG format is not required, save as binary to reduce overhead.
	static bool SaveTextureToBinary(UTexture2D* Texture, const FString& FilePath);
	
	static bool SaveTextureToPNG(UTexture2D* Texture, const FString& FilePath);
	static UTexture2D* LoadPNGToTexture(const FString& FilePath);
	
private:
	bool bIsCapturing = false;
	int32 CapturingIndex;
	int32 FrameIndex;
	float TargetFrameInterval = 1.0f / 30.0f; // 30fps
	float TimeElapsedFromLastCapture;
	FString CaptureDirectory;

	// End Game
	UPROPERTY()
	TArray<UTexture2D*> POTGData;  
	
	UPROPERTY()
	UJointQuestGameInstance* GameInstance;
};
