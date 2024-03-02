#ifndef __TRACKINGACTION__
#define __TRACKINGACTION__

#include "G4UserTrackingAction.hh"
#include "AnalysisManager.h"

class TrackingAction : public G4UserTrackingAction {
public:
    TrackingAction();
    ~TrackingAction();

    virtual void PreUserTrackingAction(const G4Track*);
    virtual void PostUserTrackingAction(const G4Track*);
};

#endif