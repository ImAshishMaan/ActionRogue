#include "AI/SBTTaskNode_SelfHeal.h"

#include "AIController.h"
#include "SAttributeComponent.h"

EBTNodeResult::Type USBTTaskNode_SelfHeal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {
	Super::ExecuteTask(OwnerComp, NodeMemory);

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ensure(AIPawn)) {
		if(USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn)) {
			AttributeComp->ApplyHealthChange(AIPawn, AttributeComp->GetHealthMax());
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
