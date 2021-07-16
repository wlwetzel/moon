#ifndef moonMessenger_h
#define moonMessenger_h 1

#include "G4UImessenger.hh"
#include "globals.hh"

class moonDetectorConstruction;
class G4UIcommand;
class G4UIdirectory;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithAnInteger;

class moonMessenger: public G4UImessenger
{
  public:
    static moonMessenger * Instance();
    void SetNewValue(G4UIcommand * command,G4String newValues);
    G4String GetCurrentValue(G4UIcommand * command);

    G4double GetThick() const;
    G4String GetParticle() const;

  private:
  static moonMessenger * instance;

  moonMessenger();
  ~moonMessenger();

  G4UIdirectory * geoDirectory;
  G4UIcmdWithADoubleAndUnit * thickCmd;
  G4UIcmdWithAString* particleCmd;

  G4double thick;
  G4String particle;

};

inline G4double moonMessenger::GetThick() const
{
  return thick;
}

inline G4String moonMessenger::GetParticle() const
{
  return particle;
}

#endif
