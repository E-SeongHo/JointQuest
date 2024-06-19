// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/WidgetComponent.h"
#include "Engine/TextureRenderTarget2D.h"

void UGameplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UCanvasPanel* CanvasPanel = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvasPanel"));
	WidgetTree->RootWidget = CanvasPanel;

	UE_LOG(LogTemp, Warning, TEXT("CanvasPanel created"));

	
	RenderTargetImage = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("RenderTargetImage"));
	UCanvasPanelSlot* ImageSlot = CanvasPanel->AddChildToCanvas(RenderTargetImage);
	if (ImageSlot)
	{
		ImageSlot->SetPosition(FVector2D(0, 0));
		ImageSlot->SetSize(FVector2D(800, 600)); 
	}

	CameraRenderTarget = NewObject<UTextureRenderTarget2D>(this);
	CameraRenderTarget->InitAutoFormat(800, 600);

	if (RenderTargetImage && CameraRenderTarget)
	{
		FSlateBrush Brush;
		Brush.SetResourceObject(CameraRenderTarget);
		RenderTargetImage->SetBrush(Brush);
	}
}
