#ifndef ALITPCCALIBCOSMIC_H
#define ALITPCCALIBCOSMIC_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

#include "AliTPCcalibBase.h"
#include "AliTPCCalPad.h"
#include "TH2F.h"
class TH1F;
class TList;
class AliESDEvent;
class AliESDtrack;

#include "TTreeStream.h"


class AliTPCcalibCosmic:public AliTPCcalibBase {
public:
  AliTPCcalibCosmic(); 
  AliTPCcalibCosmic(const Text_t *name, const Text_t *title);
  virtual ~AliTPCcalibCosmic();
  
  virtual void      Process(AliESDEvent *event);
  virtual Long64_t  Merge(TCollection *li);
  virtual void      Analyze();
  //
  void              FindPairs(AliESDEvent *event);
  Bool_t            IsPair(AliExternalTrackParam *tr0, AliExternalTrackParam *tr1);
  void              SetGainMap(AliTPCCalPad *GainMap){fGainMap = GainMap;};
  static void       CalculateBetheParams(TH2F *hist, Double_t * initialParam);
  static Double_t   CalculateMIPvalue(TH1F * hist);
  AliExternalTrackParam *Invert(AliExternalTrackParam *input);
  //
  TH1F   *          GetHistNTracks(){return fHistNTracks;};
  TH1F   *          GetHistClusters(){return fClusters;};
  TH2F   *          GetHistAcorde(){return fModules;};
  TH1F   *          GetHistPt(){return fHistPt;};
  TH2F   *          GetHistDeDx(){return fDeDx;};
  TH1F   *          GetHistMIP(){return fDeDxMIP;};
  //
  Double_t          GetMIPvalue(){return fMIPvalue;};
  //
  static void       BinLogX(TH1 * h);   // method for correct histogram binning


private:

  void              FillAcordeHist(AliESDtrack *upperTrack);

  AliTPCCalPad *fGainMap;         //  gain map from Krypton calibration
  TH1F  *fHistNTracks;            //  histogram showing number of ESD tracks per event
  TH1F  *fClusters;               //  histogram showing the number of clusters per track
  TH2F  *fModules;                //  2d histogram of tracks which are propagated to the ACORDE scintillator array
  TH1F  *fHistPt;                 //  Pt histogram of reconstructed tracks
  TH2F  *fDeDx;                   //  dEdx spectrum showing the different particle types
  TH1F  *fDeDxMIP;                //  TPC signal close to the MIP region of muons 0.4 < p < 0.45 GeV

  Double_t fMIPvalue;             //  MIP value calculated via a fit to fDeDxMIP
  //
  // cuts
  //
  Float_t fCutMaxD;     // maximal distance in rfi ditection
  Float_t fCutTheta;    // maximal distance in theta ditection
  Float_t fCutMinDir;   // direction vector products

  AliTPCcalibCosmic(const AliTPCcalibCosmic&); 
  AliTPCcalibCosmic& operator=(const AliTPCcalibCosmic&); 

  ClassDef(AliTPCcalibCosmic, 1); 
};

#endif

