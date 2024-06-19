
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
class UImage;
class UTextureRenderTarget2D;

UCLASS()
class JOINTQUEST_API UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(EditAnywhere, Category = "RenderTarget")
	UTextureRenderTarget2D* CameraRenderTarget;

	UPROPERTY(EditAnywhere)
	UImage* RenderTargetImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UImage* WebcamResource;
};
