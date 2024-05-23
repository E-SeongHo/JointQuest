// JNU-JointQuest


#include "GraphWidget.h"
#include "Rendering/DrawElements.h"

int32 UGraphWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	int32 NewLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle,
	                          bParentEnabled);

	const float height = ViewPortSize.Y - 300.0f;
	const float offset = 300.0f;
	
	for(int i = 0; i < Records.Num(); i++)
	{
		FVector2D PositionA = FVector2D(Stride * i + offset, height);
		FVector2D PositionB = FVector2D(Stride * i + offset, height + 100.0f * 10.0f);

		FLinearColor LineColor = Records[i].bHasSucceeded ? FLinearColor::Green : FLinearColor::Red;

		FSlateDrawElement::MakeDrawSpaceSpline(
			OutDrawElements,
			NewLayerId,
			PositionA,
			FVector2D::ZeroVector,
			PositionB,
			FVector2D::ZeroVector,
			2.0f,
			ESlateDrawEffect::None,
			LineColor
		);
	}
	
	return NewLayerId;
}
