#include "SPlayerState.h"

void ASPlayerState::AddCredits(int32 Delta) {
	if(!ensure(Delta > 0.f)) {
		return;
	}
	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta) {
	if(!ensure(Delta > 0.f)) {
		return false;
	}
	if(Credits < Delta) {
		return false;
	}
	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	return true;
}

int32 ASPlayerState::GetCurrentCredits() const {
	return Credits;
}