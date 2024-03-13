// Fill out your copyright notice in the Description page of Project Settings.


#include "SItemChest.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest() {
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetPitch = 110;

	SetReplicates(true);
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn) {
	//LidMesh->SetRelativeRotation(FRotator(TargetPitch, 0, 0));
	bLidOpened = !bLidOpened;
	OnRep_LidOpened(); // for server
}

void ASItemChest::OnRep_LidOpened() { // for client (it will be called automatically for client)
	float CurrPitch = bLidOpened ? TargetPitch : 0.f;
	LidMesh->SetRelativeRotation(FRotator(CurrPitch, 0, 0));
}

void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bLidOpened);
}
