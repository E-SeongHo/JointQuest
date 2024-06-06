// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ImageUtils.h"
#include "Json.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TransportManager.generated.h"



UCLASS()
class JOINTQUEST_API ATransportManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATransportManager();


	UFUNCTION(BlueprintCallable)
	static FString BytestreamToString(TArray<uint8> bytes);


	UFUNCTION(BlueprintCallable)
	static TArray<uint8> StringToBytestream(FString text);

	UFUNCTION(BlueprintCallable)
	static UTexture2D* DecodeImage(FString text);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	static UTexture2D* CreateTextureFromBits(TArray<uint8> data);
	static float JointAngle;
	static float SubAngle1;
	static float SubAngle2;

	// capture
	static UTexture2D* CurrentWebcamDisplay;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	static UTexture2D* GetCurrentWebcamDisplay();
	
	UFUNCTION(BlueprintCallable)
	static float GetJointAngle();

	UFUNCTION(BlueprintCallable)
	static void SetJointAngle(float value);

	UFUNCTION(BlueprintCallable)
	static float GetSubAngle1();

	UFUNCTION(BlueprintCallable)
	static void SetSubAngle1(float value);

	UFUNCTION(BlueprintCallable)
	static float GetSubAngle2();

	UFUNCTION(BlueprintCallable)
	static void SetSubAngle2(float value);

	UFUNCTION(BlueprintCallable)
	static bool SaveImage(UTexture2D* texture, const FString& path);
};
