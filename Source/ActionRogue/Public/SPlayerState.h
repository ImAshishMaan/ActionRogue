#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

UCLASS()
class ACTIONROGUE_API ASPlayerState : public APlayerState {
	GENERATED_BODY()

protected:
	
	int32 Credits;

public:

	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);
	
	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCurrentCredits() const;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditsChanged;
};
