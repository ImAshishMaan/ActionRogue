// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();


	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	int32 NrOfAliveBots = 0;
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive()) {
			NrOfAliveBots;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("At maximum bot capacity. Skipping bot spawn."));

	float MaxBotcount = 10.0f;

	if (DifficultyCurve) {
		MaxBotcount = DifficultyCurve->GetFloatValue(GetWorld()->TimeSeconds);
	}
	if (NrOfAliveBots >= MaxBotcount) {
		UE_LOG(LogTemp, Warning, TEXT("At maximum bot capacity. Skipping bot spawn"));
		return;
	}

	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	if (ensure(QueryInstance)) {
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	}


}

void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) {
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	FVector error = FVector(0, 0, 100); // because my Landscape is at 100z
	if (Locations.Num() > 0) {
		GetWorld()->SpawnActor<AActor>(MinionClass, Locations[0] + error, FRotator::ZeroRotator);

		DrawDebugSphere(GetWorld(), Locations[0] + error, 50.0f, 20, FColor::Blue, false, 60.0f);
	}

}

