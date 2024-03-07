#include "SGameModeBase.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("su.SpawnBots"), true, TEXT("Enable spawning of bots via timer."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();


	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

void ASGameModeBase::KillAll() {
	for (TActorIterator<ASAICharacter> It(GetWorld()); It; ++It) {
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(Bot);
		if (ensure(AttributeComp) && AttributeComp->IsAlive()) {
			AttributeComp->Kill(this);
		}
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	if(!CVarSpawnBots.GetValueOnGameThread()){
		UE_LOG(LogTemp, Log, TEXT("Bot spawning disabled via cvar 'CVarSpawnBots'."));
		return;
	}
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

void ASGameModeBase::RespawnPlayerElapsed(AController* Controller) {
	if(ensure(Controller)) {
		Controller->UnPossess();
		
		RestartPlayer(Controller);
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* Killer) {
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if(ensure(Player)) {
		FTimerHandle TimerHandle_RespawnDelay;

		FTimerDelegate Delegate;
		Delegate.BindUFunction(this, "RespawnPlayerElapsed", Player->GetController());
		
		float RespawnDelay = 2.0f;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnDelay, Delegate, RespawnDelay, false);
	}
	UE_LOG(LogTemp, Log, TEXT("OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(Killer));
}

