// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include <GameFramework/ProjectileMovementComponent.h>
#include <GameFramework/DamageType.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystemComponent.h>

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile"));
	RootComponent = ProjectileMesh;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	TrailParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Trail Particles"));
	TrailParticles->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

	if (LaunchSound)
		UGameplayStatics::PlaySoundAtLocation(this, LaunchSound, GetActorLocation());
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnHit(UPrimitiveComponent* hitComp, AActor* otherActor, UPrimitiveComponent* otherHitComp, FVector normalImpulse, const FHitResult& hit)
{
	auto owner = GetOwner();
	if (!owner)
	{
		Destroy();
		return;
	}

	if (otherActor && otherActor != this && otherActor != owner)
	{
		auto instigator = owner->GetInstigatorController();
		auto damageTypeClass = UDamageType::StaticClass();

		UGameplayStatics::ApplyDamage(otherActor, Damage, instigator, this, damageTypeClass);

		if (HitParticles)
			UGameplayStatics::SpawnEmitterAtLocation(this, HitParticles, GetActorLocation(), GetActorRotation());
		if (LaunchSound)
			UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
		if (HitCamerShakeClass)
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(HitCamerShakeClass);
	}

	Destroy();
}