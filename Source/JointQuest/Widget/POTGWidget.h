// JNU-JointQuest

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "POTGWidget.generated.h"

class UImage;

UCLASS()
class JOINTQUEST_API UPOTGWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void UpdateImage();

private:
	UPROPERTY(meta = (BindWidget))
	UImage* DisplayImage;
	
	FTimerHandle ImageUpdateTimer;
	float StreamFrameRate = 1.0f / 30.0f;
};
