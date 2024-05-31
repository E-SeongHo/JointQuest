// JNU-JointQuest

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarningWidget.generated.h"

/**
 * 
 */
UCLASS()
class JOINTQUEST_API UWarningWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnWarningSignVisible();
};
