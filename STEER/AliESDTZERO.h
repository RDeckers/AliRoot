// -*- mode: C++ -*- 
#ifndef ALIESDTZERO_H
#define ALIESDTZERO_H

/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */


//-------------------------------------------------------------------------
//                          Class AliESDTZERO
//   This is a class that summarizes the TZERO data for the ESD   
//   Origin: Christian Klein-Boesing, CERN, Christian.Klein-Boesing@cern.ch 
//-------------------------------------------------------------------------



#include <TObject.h>

class AliESDTZERO: public TObject {
public:
  AliESDTZERO();
  AliESDTZERO(const AliESDTZERO& tzero);
  AliESDTZERO& operator=(const AliESDTZERO& tzero);
  virtual void Copy(TObject &obj) const;

  Double32_t GetT0zVertex() const {return fT0zVertex;}
  void SetT0zVertex(Double32_t z) {fT0zVertex=z;}
  Double32_t GetT0() const {return fT0timeStart;}
  void SetT0(Double_t timeStart) {fT0timeStart = timeStart;}
 Float_t GetT0clock() const {return fT0clock;}
  void SetT0clock(Float_t timeStart) {fT0clock = timeStart;}
  Double32_t GetT0TOF(Int_t i) const {return fT0TOF[i];}
  const Double32_t * GetT0TOF() const {return fT0TOF;}
  void SetT0TOF(Int_t icase, Float_t time) { fT0TOF[icase] = time;}
  Int_t GetT0Trig() const {return fT0trig;}
  void SetT0Trig(Int_t tvdc) {fT0trig = tvdc;}
  const Double32_t * GetT0time() const {return fT0time;}
  void SetT0time(Double32_t time[24]) {
    for (Int_t i=0; i<24; i++) fT0time[i] = time[i];
  }
  const Double32_t * GetT0amplitude() const {return fT0amplitude;}
  void SetT0amplitude(Double32_t amp[24]) {
    for (Int_t i=0; i<24; i++) fT0amplitude[i] = amp[i];
  }

  void    Reset();
  void    Print(const Option_t *opt=0) const;

private:

  Float_t      fT0clock;     // backward compatibility
  Double32_t   fT0TOF[3];     // interaction time in ns ( A&C, A, C)
  Double32_t      fT0zVertex;       // vertex z position estimated by the T0
  Double32_t      fT0timeStart;     // interaction time estimated by the T0
  Int_t        fT0trig;            // T0 trigger signals
  Double32_t      fT0time[24];      // best TOF on each T0 PMT
  Double32_t      fT0amplitude[24]; // number of particles(MIPs) on each T0 PMT

  ClassDef(AliESDTZERO,4)
};


#endif
