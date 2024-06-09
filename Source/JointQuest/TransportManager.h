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
	FString BytestreamToString(TArray<uint8> bytes);


	UFUNCTION(BlueprintCallable)
	TArray<uint8> StringToBytestream(FString text);

	UFUNCTION(BlueprintCallable)
	UTexture2D* DecodeImage(FString text);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UTexture2D* CreateTextureFromBits(TArray<uint8> data);
	float JointAngle;
	float SubAngle1;
	float SubAngle2;
	static FString BodyData;

	// capture
	UPROPERTY()
	UTexture2D* CurrentWebcamDisplay;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UTexture2D* GetCurrentWebcamDisplay();
	
	UFUNCTION(BlueprintCallable)
	float GetJointAngle();

	UFUNCTION(BlueprintCallable)
	void SetJointAngle(float value);

	UFUNCTION(BlueprintCallable)
	float GetSubAngle1();

	UFUNCTION(BlueprintCallable)
	void SetSubAngle1(float value);

	UFUNCTION(BlueprintCallable)
	float GetSubAngle2();

	UFUNCTION(BlueprintCallable)
	void SetSubAngle2(float value);

	UFUNCTION(BlueprintCallable)
	bool SaveImage(UTexture2D* texture, const FString& path);


	UFUNCTION(BlueprintCallable)
	static void SetBodyData(FString data);

	UFUNCTION(BlueprintCallable)
	static FString GetBodyData();
};
