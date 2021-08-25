//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file moonDetectorConstruction.cc
/// \brief Implementation of the moonDetectorConstruction class

#include "moonDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

moonDetectorConstruction::moonDetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0),
  logicWorld(nullptr)
{
  fMessenger = moonMessenger::Instance();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

moonDetectorConstruction::~moonDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* moonDetectorConstruction::Construct()
{
  //
  //note to self: G4Box is defined by half length
  //
  G4bool checkOverlaps = true; //set overlaps
  // Get nist material manager

  //G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");
  G4double atomicNumber = 1;
  G4double massOfMole = 1.008*g/mole;
  G4double density = 1.e-25*g/cm3;
  G4double temperature = 2.73*kelvin;
  G4double pressure = 3.e-18*pascal;
  G4Material* env_mat =
    new G4Material("vac",atomicNumber,massOfMole,density,kStateGas,temperature,
                  pressure);

  G4double world_dim = 8*cm; //cube
  //world
  G4Box* solidWorld =
    new G4Box("world",world_dim,world_dim,world_dim);

  logicWorld =
    new G4LogicalVolume(solidWorld, env_mat,"world");

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(nullptr,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "world",               //its name
                      nullptr,                     //its mother  volume (null)
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  G4double hx = .5*3*mm;
  G4double hy = .5*7*mm;
  G4double det_hz = .5*250*micrometer;
  G4double shield_hz = .5*.01*2.54*cm; // .01 inches
  G4double plate_thickness = fMessenger->GetThick();
  //Detector
  moonDetectorConstruction::detectorConstruction(hx,hy,det_hz);
  //shield
  moonDetectorConstruction::shieldConstruction(hx,hy,shield_hz);

  //plate to cover shield+detector
  moonDetectorConstruction::plateConstruction(hx,hy,plate_thickness);
  //
  //always return the physical World
  //
  return physWorld;
}

void moonDetectorConstruction::plateConstruction(const G4double plate_hx,
                                                  const G4double plate_hy,
                                                  const G4double plate_hz)
{
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* plate_mat = nist->FindOrBuildMaterial("G4_Al");
  G4double shield_hz = .5*.01*2.54*cm; // .01 inches
  G4double det_hz = .5*250*micrometer;
  G4ThreeVector plate_pos = G4ThreeVector(0,0,1*cm - (shield_hz+det_hz+plate_hz));

  G4Box* solidPlate =
      new G4Box("plate",plate_hx,plate_hy,plate_hz);
  G4LogicalVolume* logicPlate =
    new G4LogicalVolume(solidPlate,
                        plate_mat,
                        "plate");
  new G4PVPlacement(0,
                    plate_pos,
                    logicPlate,
                    "plate",
                    logicWorld,
                    false,
                    0,
                    true);
}

void moonDetectorConstruction::detectorConstruction(const G4double det_hx,
                                                    const G4double det_hy,
                                                    const G4double det_hz)
{
  G4NistManager* nist = G4NistManager::Instance();
  //Detector
  G4Material* det_mat = nist-> FindOrBuildMaterial("G4_Si");
  G4VisAttributes * detColor = new G4VisAttributes(G4Colour(0.,1.,1.));

  G4ThreeVector det_pos = G4ThreeVector(0,0,1*cm);

  G4Box* solidDetecor =
    new G4Box("detector",det_hx,det_hy,det_hz);
  G4LogicalVolume* logicDetector =
    new G4LogicalVolume(solidDetecor,
                        det_mat,
                        "detector");
  logicDetector-> SetVisAttributes(detColor);
    new G4PVPlacement(0,                     //no rotation
                      det_pos,
                      logicDetector,            //its logical volume
                      "detector",               //its name
                      logicWorld,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      true);        //overlaps checking

  // Set detector as scoring volume
  //
  fScoringVolume = logicDetector;

}

void moonDetectorConstruction::shieldConstruction(const G4double shield_hx,
                                                  const G4double shield_hy,
                                                  const G4double shield_hz)
{

  G4NistManager* nist = G4NistManager::Instance();
  G4Material* shield_mat = nist->FindOrBuildMaterial("G4_Al");
  G4double det_hz = .5*250*micrometer;
  G4ThreeVector shield_pos = G4ThreeVector(0,0,1*cm - (shield_hz+det_hz));

  G4Box* solidShield =
      new G4Box("shield",shield_hx,shield_hy,shield_hz);
  G4LogicalVolume* logicShield =
    new G4LogicalVolume(solidShield,
                        shield_mat,
                        "shield");
  new G4PVPlacement(0,
                    shield_pos,
                    logicShield,
                    "shield",
                    logicWorld,
                    false,
                    0,
                    true);

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
