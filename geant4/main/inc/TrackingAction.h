#ifndef __TRACKINGACTION__
#define __TRACKINGACTION__

#ifdef __TRACKINGACTION_HEADER__
#include "G4ios.hh"
#include "AnalysisManager.h"
#endif

#include "G4UserTrackingAction.hh"

class TrackingAction : public G4UserTrackingAction {
public:
	TrackingAction();
	~TrackingAction();

	virtual void PreUserTrackingAction(const G4Track*);
	virtual void PostUserTrackingAction(const G4Track*);
};

#endif