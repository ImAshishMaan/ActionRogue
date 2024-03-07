#include "AI/SBTService_CheckHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) {
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ensure(AIPawn)) {
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);
		if(ensure(AttributeComp)) {
			UBlackboardComponent* BlackBordComp = OwnerComp.GetBlackboardComponent();
			BlackBordComp->SetValueAsBool(LowHealthKey.SelectedKeyName, AttributeComp->IsLowHealth());
		}
	}
}
