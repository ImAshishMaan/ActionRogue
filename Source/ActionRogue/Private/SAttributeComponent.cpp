#include "SAttributeComponent.h"
#include "SGameModeBase.h"

USAttributeComponent::USAttributeComponent() {
	HealthMax = 100;
	Health = HealthMax;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor) {
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

bool USAttributeComponent::IsAlive() const {
	return Health > 0.0f;
}

bool USAttributeComponent::IsFullHealth() const {
	return Health == HealthMax;
}

bool USAttributeComponent::IsLowHealth() {
	return Health < 40;
}


float USAttributeComponent::GetHealthMax() const {
	return HealthMax;
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta) {
	if(!GetOwner()->CanBeDamaged()) {
		return false;
	}
	
	float OldHealth = Health;

	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = Health - OldHealth;
	OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);

	if(ActualDelta < 0.0f && Health == 0.0f) {
		ASGameModeBase* GM = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if(GM) {
			GM->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}
	
	return ActualDelta != 0;
}

USAttributeComponent* USAttributeComponent::GetAttributes(AActor* FromActor) {
	if(FromActor) {
		return FromActor->GetComponentByClass<USAttributeComponent>();
		//return Cast<USAttributeComponent>(FromActor->GetComponentByClass(USAttributeComponent::StaticClass()));
	}

	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor) {
	if(USAttributeComponent* AttributeComp = GetAttributes(Actor)) {
		return AttributeComp->IsAlive();
	}
	return false;
}
