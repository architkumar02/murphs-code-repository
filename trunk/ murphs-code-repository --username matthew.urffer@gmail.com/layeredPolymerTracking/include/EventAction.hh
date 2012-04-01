#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "EventActionMessenger.hh"

#include "G4THitsMap.hh"
#include "globals.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles in Absober and Gap layers 
/// stored in the hits collections.
///
/// The data member fPrintModulo defines the frequency of printing.
/// Its value can be changed via a command defined in EventActionMassenger 
/// class. 

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
                     
  // set methods
  void SetPrintModulo(G4int value);
    
private:
  // methods
  G4THitsMap<G4double>* GetHitsCollection(const G4String& hcName,
                                          const G4Event* event) const;
  G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
  void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,
                            G4double gapEdep, G4double gapTrackLength) const;
  
  // data members                   
  EventActionMessenger  fMessenger;
  G4int  fPrintModulo;
};

// inline functions

inline void EventAction::SetPrintModulo(G4int value) {
  fPrintModulo = value;
}
#endif
