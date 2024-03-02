// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackHoleProjectile.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/StaticMeshActor.h"

ASBlackHoleProjectile::ASBlackHoleProjectile()
{
	SphereComp->SetSphereRadius(60.0f);
    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackHoleProjectile::OnActorOverlap);

	ForceComp->ForceStrength = -20000000;
	ForceComp->Radius = 400;

	MoveComp->InitialSpeed = 4000;
	
	DamageAmount = -50;
}

void ASBlackHoleProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    {
        // Check if the owner of the overlapped component is a StaticMeshActor
        AStaticMeshActor* StaticMeshActor = Cast<AStaticMeshActor>(OtherComp->GetOwner());
        if (StaticMeshActor)
        {
            // Check if the actor is still valid before destroying it
            if (StaticMeshActor && StaticMeshActor->IsValidLowLevelFast())
            {
                StaticMeshActor->Destroy();
            }
        }
    }
}
