#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGameplayInterface.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	float TargetPitch;

	void Interact_Implementation(APawn* InstigatorPawn);

protected:

	UPROPERTY(ReplicatedUsing ="OnRep_LidOpened", Replicated)
	bool bLidOpened;

	UFUNCTION()
	void OnRep_LidOpened();

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UStaticMeshComponent* LidMesh;

public:	
	ASItemChest();

};
