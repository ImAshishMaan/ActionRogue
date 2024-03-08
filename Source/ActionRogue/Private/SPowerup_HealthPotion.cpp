#include "SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"


ASPowerup_HealthPotion::ASPowerup_HealthPotion() {

	CreditCost = 50;
}


void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn) {
	if(!ensure(InstigatorPawn)) {
		return;
	}

	USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	// Check if not already at max health
	if(ensure(AttributeComp) && !AttributeComp->IsFullHealth()) {
		if(ASPlayerState* PS = InstigatorPawn->GetPlayerState<ASPlayerState>()) {
			// Only activate if healed successfully
			if(PS->RemoveCredits(CreditCost) && AttributeComp->ApplyHealthChange(this, AttributeComp->GetHealthMax())) {
				HideAndCooldownPowerup();
			}
		}
	}
}
