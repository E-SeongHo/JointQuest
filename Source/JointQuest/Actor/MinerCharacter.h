
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MinerCharacter.generated.h"

class AMinerPlayerController;
class USpringArmComponent;
class UCameraComponent;
class USoundBase;
class UInputMappingContext;
class UInputAction;
class ADiggingSoilGameMode;
class UNiagaraSystem; 


UCLASS()
class JOINTQUEST_API AMinerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	AMinerCharacter();

protected:
	virtual auto BeginPlay() -> void override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void BeginCharging();

	UFUNCTION(BlueprintCallable)
	void EndCharging();

	UFUNCTION(BlueprintCallable)
	void DigGround();

	UFUNCTION(BlueprintCallable)
	float GetHoldingProgressPercent() const;

private:
	void Move(const struct FInputActionValue& Value);
	virtual void StopJumping() override;
	void TriggerDiggingNiagaraEffect(float Duration);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KneeUpAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* KneeDownAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	USoundBase* JumpingSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound, meta = (AllowPrivateAccess = "true"))
	USoundBase* DiggingSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraShake, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UCameraShakeBase> DiggingShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* DiggingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Effect, meta = (AllowPrivateAccess = "true"))
	FVector EffectOffset;
	
	TArray<AActor*> MovableActors;

	FTimerHandle ChargingTimerHandle;

	AMinerPlayerController* PlayerController;
	ADiggingSoilGameMode* GameMode;
	
	bool bIsCharging = false;
	
	float ChargedTime = 0.0f;
	float MinChargeDuration = 1.0f; 
};
