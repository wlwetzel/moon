#include "moonMessenger.hh"

#include "G4SystemOfUnits.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithAnInteger.hh"

moonMessenger * moonMessenger::instance(nullptr);

moonMessenger * moonMessenger::Instance()
{
    if (instance == nullptr)
        instance = new moonMessenger;

    return instance;
}

moonMessenger::moonMessenger(){
  geoDirectory = new G4UIdirectory("/config/");
  geoDirectory->SetGuidance("Configure Scene Geometries.");

  thickCmd = new G4UIcmdWithADoubleAndUnit("/config/Thick",this);
  thickCmd->SetGuidance("Shielding thickness.");
  thickCmd->SetGuidance("  (10mm is default)");
  thickCmd->SetParameterName("Thick",true);
  thickCmd->SetDefaultValue(.5*4*mm);


  particleCmd = new G4UIcmdWithAString("/config/particle",this);
  particleCmd->SetGuidance("Particle Type.");
  particleCmd->SetGuidance("  (Electron is default)");
  particleCmd->SetParameterName("Electron",true);
  particleCmd->SetDefaultValue("Electron");

  // initital vals

  thick = .5 * 4 * mm;
  particle = "Electron";
}

moonMessenger::~moonMessenger()
{
  delete thickCmd;
  delete particleCmd;
}

void moonMessenger::SetNewValue(G4UIcommand * command, G4String newValues)
{
    if (command == thickCmd)
        thick = G4UIcmdWithADoubleAndUnit::GetNewDoubleValue(newValues);
    if (command == particleCmd)
        particle = newValues;
}

G4String moonMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String cv;
    if( command==thickCmd )
        cv = thick;
    if (command==particleCmd)
        cv = particle;
    return cv;
}
