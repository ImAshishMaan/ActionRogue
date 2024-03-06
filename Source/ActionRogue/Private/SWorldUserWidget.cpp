#include "SWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!IsValid(AttachedActor)) {
		RemoveFromParent();
		
		UE_LOG(LogTemp, Warning, TEXT("Attached Actor no longer valid, removing Health Widget."));
		return;
	}
	
	FVector2D ScreenPositon;
	if(UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + Worldoffset, ScreenPositon)) {
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);
		ScreenPositon /= Scale;

		if(ParentSizeBox) {
			ParentSizeBox->SetRenderTranslation(ScreenPositon);
		}
	}
}
