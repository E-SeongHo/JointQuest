// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PythonLauncher.generated.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogCustom, Log, All);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPyEventSignature);

UCLASS(ClassGroup = "Transport", meta = (BlueprintSpawnableComponent))
class JOINTQUEST_API UPythonLauncher : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintAssignable, Category = "PyLauncher Events")
	FPyEventSignature OnScriptBegin;

	UPROPERTY(BlueprintAssignable, Category = "PyLauncher Events")
	FPyEventSignature OnScriptEnd;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Python Launcher")
	FString Path;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Python Launcher")
	bool LaunchImmediately;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Python Launcher")
	bool SearchFromSource;

	// Sets default values for this actor's properties
	UPythonLauncher(const FObjectInitializer& init);

	void Launch(FString LaunchPath);

	UFUNCTION(BlueprintCallable, Category = "PyLauncher Functions")
	void Launch();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	FString GetRootPath();
};
