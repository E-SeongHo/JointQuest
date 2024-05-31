// JNU-JointQuest


#include "WarningWidget.h"

void UWarningWidget::SetVisibility(ESlateVisibility InVisibility)
{
	Super::SetVisibility(InVisibility);

	if(InVisibility == ESlateVisibility::SelfHitTestInvisible)
	{
		OnWarningSignVisible();
	}
}
