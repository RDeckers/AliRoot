/////////////////////////////////////////////////////////
//  Manager and hits classes for set:PHOS version 1    //
/////////////////////////////////////////////////////////
 
// --- ROOT system ---
#include "TH1.h"
#include "TRandom.h"
#include "TFile.h"
#include "TTree.h"
#include "TBRIK.h"
#include "TNode.h"
#include "TMath.h"

// --- galice header files ---
#include "AliPHOSv1.h"
#include "AliRun.h"
#include "TGeant3.h" 

ClassImp(AliPHOSv1)

//______________________________________________________________________________


AliPHOSv1::AliPHOSv1()
{
}
 
//______________________________________________________________________________

AliPHOSv1::AliPHOSv1(const char *name, const char *title)
          : AliPHOS(name, title)
{
}
 
//___________________________________________
void AliPHOSv1::CreateGeometry()
{

  AliPHOS *PHOS_tmp = (AliPHOS*)gAlice->GetModule("PHOS");
  if( NULL==PHOS_tmp )
  {
    printf("There isn't PHOS detector!\n");
    return;
  }
//  AliPHOS &PHOS = *PHOS_tmp;

  //////////////////////////////////////////////////////////////////////////////

  Int_t                 rotation_matrix_number=0;
  Float_t               par[11],
                        x,y,z;

  const float           cell_length             = GetCrystalLength()+GetAirThickness()+GetWrapThickness()+GetPIN_Length(),
                        cell_side_size          = GetCrystalSideSize()+2*GetAirThickness()+2*GetWrapThickness(),
//                        cell_angle              = 180/kPI * 2 * atan(cell_side_size/2 / GetRadius()),        // radians
                        cradle_thikness         = cell_length + GetCPV_Thickness() + GetCPV_PHOS_Distance(),
                        distance_to_CPV         = GetRadius() - GetCPV_Thickness() - GetCPV_PHOS_Distance();

  //////////////////////////////////////////////////////////////////////////////
  // CELL volume and subvolumes creation
  //////////////////////////////////////////////////////////////////////////////

  par[0] = GetCrystalSideSize()/2 + GetWrapThickness();
  par[1] = GetCrystalSideSize()/2 + GetWrapThickness();
  par[2] = GetCrystalLength()  /2 + GetWrapThickness()/2;
  gMC->Gsvolu("WRAP","BOX ",GetPHOS_IDTMED_Tyvek(),par,3);

  par[0] = GetCrystalSideSize()/2;
  par[1] = GetCrystalSideSize()/2;
  par[2] = GetCrystalLength()/2;
  gMC->Gsvolu("CRST","BOX ",GetPHOS_IDTMED_PbWO4(),par,3);

  // PIN
  par[0] = GetPIN_SideSize()/2;
  par[1] = GetPIN_SideSize()/2;
  par[2] = GetPIN_Length()/2;
  gMC->Gsvolu("PIN ","BOX ",GetPHOS_IDTMED_PIN(),par,3);

  //////////////////////////////////////////////////////////////////////////////
  // CRADLE,CPV creation.
  //////////////////////////////////////////////////////////////////////////////

  par[0] = cell_side_size/2 * GetNz();
  par[1] = cell_side_size/2 * GetNphi();
  par[2] = cradle_thikness/2;
  gMC->Gsvolu("PHOS","BOX ",GetPHOS_IDTMED_AIR(),par,3);

//par[0] : the same as above
//par[1] : the same as above
  par[2] = GetCPV_Thickness()/2;
  gMC->Gsvolu("CPV ","BOX ",GetPHOS_IDTMED_CPV(),par,3);

  x = 0;
  y = 0;
  z = (cell_length+GetCPV_PHOS_Distance())/2;
  gMC->Gspos("CPV ",1,"PHOS",x,y,z,0,"ONLY");

  par[0] = cell_side_size/2 * GetNz();
  par[1] = cell_side_size/2 * GetNphi();
  par[2] = cell_length/2;
  gMC->Gsvolu("CRS0","BOX ",GetPHOS_IDTMED_AIR(),par,3);

  x = 0;
  y = 0;
  z = -(cradle_thikness-cell_length)/2;
  gMC->Gspos("CRS0",1,"PHOS",x,y,z,0,"ONLY");

  gMC->Gsdvn("CRS1","CRS0",GetNphi(),2);
  gMC->Gsdvn("CELL","CRS1",GetNz()  ,1);

  //////////////////////////////////////////////////////////////////////////////
  // CELL creation
  //////////////////////////////////////////////////////////////////////////////

  x = 0;
  y = 0;
  z = -GetWrapThickness()/2;
  gMC->Gspos("CRST",1,"WRAP",x,y,z,0,"ONLY");

  x = 0;
  y = 0;
  z = GetPIN_Length()/2;
  gMC->Gspos("WRAP",1,"CELL",x,y,z,0,"ONLY");

  x = 0;
  y = 0;
  z = -GetCrystalLength()/2-GetWrapThickness()/2;
  gMC->Gspos("PIN ",1,"CELL",x,y,z,0,"ONLY");

  //////////////////////////////////////////////////////////////////////////////
  // CELL has been created.
  //////////////////////////////////////////////////////////////////////////////

//   int n=0;
//   z = -(GetCPV_Thickness()+GetCPV_PHOS_Distance())/2;
//
//   for( int iy=0; iy<GetNphi(); iy++ )
//   {
//     y = (iy-(GetNphi()-1)/2.)*cell_side_size;
//     for( int ix=0; ix<GetNz(); ix++ )
//     {
//       x = (ix-(GetNz()-1)/2.)*cell_side_size;
//       gMC->Gspos("CELL",++n,"PHOS",x,y,z,0,"ONLY");
//     }
//   }

  //////////////////////////////////////////////////////////////////////////////
  // End of CRADLE creation.
  //////////////////////////////////////////////////////////////////////////////


  //////////////////////////////////////////////////////////////////////////////
  // PHOS creation
  //////////////////////////////////////////////////////////////////////////////

  for( int i=0; i<GetCradlesAmount(); i++ )
  {
    float c                = distance_to_CPV,           // Distance to CPV
          l                = cell_side_size*GetNphi()/2,      // Cradle half size around beam (for rect. geom.)
          cradle_angle     = 360/kPI*atan(l/c),
          cradle_angle_pos = -90+(i-(GetCradlesAmount()-1)/2.) * (cradle_angle+GetAngleBetweenCradles());
    // Cradles are numerated in clock reversed order. (general way of angle increment)

    float   r       = GetRadius() + cradle_thikness/2;
    x = r*cos(cradle_angle_pos*kPI/180);
    y = r*sin(cradle_angle_pos*kPI/180);
    z = 0;
    AliMatrix(rotation_matrix_number, 0,0 , 90,90+cradle_angle_pos , 90,180+cradle_angle_pos);
    gMC->Gspos("PHOS",i+1,"ALIC",x,y,z,rotation_matrix_number,"ONLY");

    GetCradleAngle(i) = cradle_angle_pos;
//
//    int n = PHOS.fCradles->GetEntries();
//    PHOS.fCradles->Add(new AliPHOSCradle( 1,            // geometry.
//                                          GetCrystalSideSize    (),
//                                          GetCrystalLength      (),
//                                          GetWrapThickness      (),
//                                          GetAirThickness       (),
//                                          GetPIN_SideSize       (),
//                                          GetPIN_Length         (),
//                                          GetRadius             (),
//                                          GetCPV_Thickness      (),
//                                          GetCPV_PHOS_Distance  (),
//                                          GetNz                 (),
//                                          GetNphi               (),
//                                          cradle_angle_pos      ));
//
//    if( n+1 != PHOS.fCradles->GetEntries() ||
//        NULL == PHOS.fCradles->At(n) )
//    {
//      cout << "  Can not create or add AliPHOSCradle.\n";
//      exit(1);
//    }
  }
  AddPHOSCradles();

  //////////////////////////////////////////////////////////////////////////////
  // All is done.
  // Print some information.
  //////////////////////////////////////////////////////////////////////////////
}

void AliPHOSv1::StepManager()
{
  static Bool_t inwold=0;   // Status of previous ctrak->inwvol
  Int_t copy;

  int cradle_number, cell_Z, cell_Phi;  // Variables that describe cell position.

  if( gMC->GetMedium() == GetPHOS_IDTMED_PIN() && (gMC->TrackInside() || gMC->TrackExiting()==2) && inwold && gMC->TrackCharge()!=0 )
  {
    // GEANT particle just have entered into PIN diode.

    AliPHOS &PHOS = *(AliPHOS*)gAlice->GetModule("PHOS");

    gMC->CurrentVolOff(4,0,copy);
    cradle_number  = copy-1;
    gMC->CurrentVolOff(1,0,copy);
    cell_Z         = copy-1;
    gMC->CurrentVolOff(2,0,copy);
    cell_Phi       = copy-1;
/*
        cradle_number  = cvolu->number[cvolu->nlevel-5]-1;
        cell_Z         = cvolu->number[cvolu->nlevel-2]-1;
        cell_Phi       = cvolu->number[cvolu->nlevel-3]-1;
*/

    TH2S &h = PHOS.GetCradle(cradle_number).fChargedTracksInPIN;
    h.AddBinContent(h.GetBin(cell_Z,cell_Phi));
  }

  //////////////////////////////////////////////////////////////////////////////

  if( gMC->GetMedium() == GetPHOS_IDTMED_PbWO4() )
  {
    // GEANT particle into crystal.

    AliPHOS &PHOS = *(AliPHOS*)gAlice->GetModule("PHOS");

    gMC->CurrentVolOff(5,0,copy);
    cradle_number  = copy-1;
    gMC->CurrentVolOff(2,0,copy);
    cell_Z         = copy-1;
    gMC->CurrentVolOff(3,0,copy);
    cell_Phi       = copy-1;
/*
        cradle_number  = cvolu->number[cvolu->nlevel-6]-1;
        cell_Z         = cvolu->number[cvolu->nlevel-3]-1;
        cell_Phi       = cvolu->number[cvolu->nlevel-4]-1;
*/
    TH2F &h = PHOS.GetCradle(cradle_number).fCellEnergy;
    h.AddBinContent(h.GetBin(cell_Z,cell_Phi),gMC->Edep());
  }

  //////////////////////////////////////////////////////////////////////////////

  if( gMC->GetMedium()==GetPHOS_IDTMED_CPV() && (gMC->TrackInside() || gMC->TrackExiting()) && inwold )
  {
    // GEANT particle just have entered into CPV detector.

    AliPHOS &PHOS = *(AliPHOS*)gAlice->GetModule("PHOS");

    gMC->CurrentVolOff(1,0,cradle_number);
    cradle_number--;
//        cradle_number  = cvolu->number[cvolu->nlevel-2]-1;

    // Save CPV x,y hits position of charged particles.

    AliPHOSCradle  &cradle = PHOS.GetCradle(cradle_number);

    Float_t   xyz[3];
    gMC->TrackPosition(xyz);
    TVector3          p(xyz[0],xyz[1],xyz[2]),v;

    float x,y,l;
    float R = cradle.GetRadius() - cradle.GetCPV_PHOS_Distance() - cradle.GetCPV_Thikness();
    cradle.GetXY(p,v,R,x,y,l);
    if( PHOS.fDebugLevel>0 )
      if( l<0 )
        printf("PHOS_STEP:  warning: negative distance to CPV!! %f\n", l);

    // Store current particle in the list of Cradle particles.
    Float_t  pmom[4];
    gMC->TrackMomentum(pmom);
    float     Px      =       pmom[0] * pmom[3],
              Py      =       pmom[1] * pmom[3],
              Pz      =       pmom[2] * pmom[3];
    Int_t     Ipart   =       gMC->TrackPid();

//     TClonesArray &P=cradle.GetParticles();
//     new( P[P.GetEntries()] ) AliPHOSgamma(x,0,y,0,ctrak->getot,0,Px,Py,Pz);
    cradle.GetParticles().Add(new AliPHOSgamma(x,y,gMC->Etot(),Px,Py,Pz,Ipart));

    if( gMC->TrackCharge()!=0 )
      cradle.AddCPVHit(x,y);
  }

  inwold=gMC->TrackEntering();         // Save current status of GEANT variable.
}

