// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TOONTANKS_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	float Health = 0.f;

	UPROPERTY(EditAnywhere)
		float MaxHealth = 100.f;

	class AToonTanksGameMode* ToonTanksGameMode;

public:
	// Sets default values for this component's properties
	UHealthComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
		void DamageTaken(AActor* damagedActor, float damage, const UDamageType* type, class AController* instigator, AActor* damageCauser);
};
