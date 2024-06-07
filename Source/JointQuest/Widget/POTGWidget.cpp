// JNU-JointQuest


#include "POTGWidget.h"

#include "Components/Image.h"
#include "JointQuest/Actor/CaptureComponent.h"
#include "JointQuest/PlayerController/MinerPlayerController.h"
#include "Kismet/GameplayStatics.h"

void UPOTGWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GetWorld()->GetTimerManager().SetTimer(ImageUpdateTimer, this, &UPOTGWidget::UpdateImage, StreamFrameRate, true);
}

void UPOTGWidget::NativeDestruct()
{
	Super::NativeDestruct();

	GetWorld()->GetTimerManager().ClearTimer(ImageUpdateTimer);
}

void UPOTGWidget::UpdateImage()
{
	AMinerPlayerController* PC = Cast<AMinerPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	check(PC);

	FSlateBrush Brush;
	UTexture2D* TextureToStream = PC->GetCaptureComponent()->StreamPOTG();
	if(TextureToStream)
	{
		Brush.SetResourceObject(TextureToStream);
		DisplayImage->SetBrush(Brush);
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("NULL PTR"));
	}
}




