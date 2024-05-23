// JNU-JointQuest

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Actor/ScoreComponent.h"

#include "GraphWidget.generated.h"
/**
 * 
 */

UCLASS()
class JOINTQUEST_API UGraphWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TArray<FExerciseRecord> Records;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FVector2D ViewPortSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float PlayerLimitAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float Stride;
};
