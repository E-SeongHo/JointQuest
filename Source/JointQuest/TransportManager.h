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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	static float GetJointAngle();

	UFUNCTION(BlueprintCallable)
	static void SetJointAngle(float value);
};
