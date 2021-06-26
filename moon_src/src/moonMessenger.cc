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
  thickCmd->SetDefaultValue(10*mm);

  // initital vals

  thick = 10 * mm;

}

moonMessenger::~moonMessenger()
{
  delete thickCmd;
}
