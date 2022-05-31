//
//  Part of the Bologna Laboratory Utility package
//
//  Utilities for Histogram handling
//
//  Author              Nicola Rubini
//  Created             22/11/2021
//  Last modified       23/03/2022
//
#ifndef BOLOGNALABORATORYUTILITY_HISTOGRAMS_H
#define BOLOGNALABORATORYUTILITY_HISTOGRAMS_H
//
//  TODO: Implement histogram check for all functions
//  TODO: Implement general function w/ warning and error messages for histogram check and bool to enable work on histo
//
//  Global File w/ Constants and other functions
#include "../BolognaLaboratoryUtility.h"
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY VARIABLES
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
Int_t       iBuilderTH1_TypeCounter =   0;
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
template <  typename THXTarget_Type >
Int_t
uGetTHDimension
 ( THXTarget_Type*   fTarget ) {
    TObject* kTObj1DTestTarget  =   dynamic_cast< TObject* >( fTarget );
    TH1* kHist1DTestTarget  =   dynamic_cast< TH1* >( fTarget );
    if ( !kHist1DTestTarget || !kTObj1DTestTarget || !fTarget )   {
        if ( !fTarget ) cout << "[ERROR] Target is a nullptr" << endl;
        if ( !kHist1DTestTarget &&  kTObj1DTestTarget ) cout << "[ERROR] Target " << fTarget->GetName() << " is not a histogram!" << endl;
        if ( !kHist1DTestTarget && !kTObj1DTestTarget ) cout << "[ERROR] Target is not a TObject!" << endl;
        return -1;
    }
    TH2* kHist2DTestTarget  =   dynamic_cast< TH2* >( fTarget );
    TH3* kHist3DTestTarget  =   dynamic_cast< TH3* >( fTarget );
    if ( !kHist2DTestTarget && !kHist3DTestTarget ) return 1;
    if (  kHist2DTestTarget && !kHist3DTestTarget ) return 2;
    if ( !kHist2DTestTarget &&  kHist3DTestTarget ) return 3;
    return -1;
}
//
template <  typename THXTarget_Type,
            typename THXSource_Type >
Int_t
uGetTHPairDimension
( THXTarget_Type*   fTarget_1,    THXSource_Type*   fTarget_2 )  {
    auto kTarget_1_Dim  =   uGetTHDimension(fTarget_1);
    auto kTarget_2_Dim  =   uGetTHDimension(fTarget_2);
    if ( kTarget_1_Dim == kTarget_2_Dim )   return  kTarget_1_Dim;
    cout << "[ERROR] Dimension of histograms are not coherent!" << endl;
    cout << "[INFO] Dimension of Target 1 is " << kTarget_1_Dim << endl;
    cout << "[INFO] Dimension of Target 2 is " << kTarget_2_Dim << endl;
    return -1;
}
//
template <  typename THXTarget_Type,
            typename THXSource_Type >
Bool_t
uIsTHPairConsistent
( THXTarget_Type*   fTarget_1,    THXSource_Type*   fTarget_2 )  {
    auto    nDimension  = uGetTHPairDimension( fTarget_1, fTarget_2 );
    if ( nDimension < 0 ) return false;
    if ( fTarget_1->GetNbinsX() != fTarget_2->GetNbinsX() ) return false;
    if ( fTarget_1->GetNbinsY() != fTarget_2->GetNbinsY() ) return false;
    if ( fTarget_1->GetNbinsZ() != fTarget_2->GetNbinsZ() ) return false;
    for ( Int_t iBin = 1; iBin <= fTarget_1->GetNbinsX(); iBin++ ) {
        for ( Int_t jBin = 1; jBin <= fTarget_1->GetNbinsY(); jBin++ ) {
            for ( Int_t kBin = 1; kBin <= fTarget_1->GetNbinsZ(); kBin++ ) {
                if ( fTarget_1->GetXaxis()->GetBinLowEdge( iBin ) != fTarget_2->GetXaxis()->GetBinLowEdge( iBin ) ) return false;
                if ( fTarget_1->GetYaxis()->GetBinLowEdge( jBin ) != fTarget_2->GetYaxis()->GetBinLowEdge( jBin ) ) return false;
                if ( fTarget_1->GetZaxis()->GetBinLowEdge( kBin ) != fTarget_2->GetZaxis()->GetBinLowEdge( kBin ) ) return false;
            }
        }
    }
    return true;
}
//
template <  Bool_t   TCheckBoundaries = true,
            typename THXTarget_Type,
            typename THXSource_Type >
Bool_t
uIsTHPairRebinnable
( THXTarget_Type*   fTarget_1,    THXSource_Type*   fTarget_2 )  {
    auto    nDimension  = uGetTHPairDimension( fTarget_1, fTarget_2 );
    if ( nDimension < 0 ) return false;
    if ( TCheckBoundaries ) {
        if ( fTarget_1->GetXaxis()->GetXmax()() != fTarget_2->GetXaxis()->GetXmax()() ) return false;
        if ( fTarget_1->GetYaxis()->GetXmax()() != fTarget_2->GetYaxis()->GetXmax()() ) return false;
        if ( fTarget_1->GetZaxis()->GetXmax()() != fTarget_2->GetZaxis()->GetXmax()() ) return false;
        if ( fTarget_1->GetXaxis()->GetXmin()() != fTarget_2->GetXaxis()->GetXmin()() ) return false;
        if ( fTarget_1->GetYaxis()->GetXmin()() != fTarget_2->GetYaxis()->GetXmin()() ) return false;
        if ( fTarget_1->GetZaxis()->GetXmin()() != fTarget_2->GetZaxis()->GetXmin()() ) return false;
    }
    for ( Int_t iBin = 1; iBin <= fTarget_1->GetNbinsX(); iBin++ ) {
        auto    kLowBinEdge_1   =   fTarget_1->GetXaxis()->GetBinLowEdge(iBin);
        auto    kLowBinEdge_2   =   fTarget_2->GetXaxis()->GetBinLowEdge(fTarget_2->GetXaxis()->FindBin(kLowBinEdge_1));
        auto    kLowBinEdge_3   =   fTarget_1->GetXaxis()->GetBinLowEdge(fTarget_1->GetXaxis()->FindBin(kLowBinEdge_2));
        if ( kLowBinEdge_3 != kLowBinEdge_2 ) return false;
    }
    for ( Int_t jBin = 1; jBin <= fTarget_1->GetNbinsY(); jBin++ ) {
        auto    kLowBinEdge_1   =   fTarget_1->GetYaxis()->GetBinLowEdge(jBin);
        auto    kLowBinEdge_2   =   fTarget_2->GetYaxis()->GetBinLowEdge(fTarget_2->GetYaxis()->FindBin(kLowBinEdge_1));
        auto    kLowBinEdge_3   =   fTarget_1->GetYaxis()->GetBinLowEdge(fTarget_1->GetYaxis()->FindBin(kLowBinEdge_2));
        if ( kLowBinEdge_3 != kLowBinEdge_2 ) return false;
    }
    for ( Int_t kBin = 1; kBin <= fTarget_1->GetNbinsZ(); kBin++ ) {
        auto    kLowBinEdge_1   =   fTarget_1->GetZaxis()->GetBinLowEdge(kBin);
        auto    kLowBinEdge_2   =   fTarget_2->GetZaxis()->GetBinLowEdge(fTarget_2->GetZaxis()->FindBin(kLowBinEdge_1));
        auto    kLowBinEdge_3   =   fTarget_1->GetZaxis()->GetBinLowEdge(fTarget_1->GetZaxis()->FindBin(kLowBinEdge_2));
        if ( kLowBinEdge_3 != kLowBinEdge_2 ) return false;
    }
    return true;
}
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! HISTOGRAM BUILDING FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
//  --  --  --  TODO: Make Build TH2 and TH3 and THn
template<   typename TH1_Type = TH1F,
            typename stdVec_Type,
            typename = typename std::enable_if<std::is_arithmetic<stdVec_Type>::value, stdVec_Type>::type >
TH1_Type*
uBuildTH1
( std::vector<stdVec_Type> fInputData, Int_t fNofBins = -1, Float_t fOffset = 0., Float_t fLowBound = 0, Float_t fHigBound = 0 )    {
    if ( fInputData.size() == 0 ) return new TH1_Type(Form("TH1_Type_from_vector_%i",iBuilderTH1_TypeCounter),Form("TH1_Type_from_vector_%i",iBuilderTH1_TypeCounter),1,0,1);
    auto    fMaxValue   =   *std::max_element(std::begin(fInputData),std::end(fInputData));
    auto    fMinValue   =   *std::min_element(std::begin(fInputData),std::end(fInputData));
    auto    fSizeOfAr   =   fInputData.size();
    if ( fLowBound == fHigBound )   {
        fLowBound   =   fMinValue - 0.2*(fMaxValue-fMinValue) + fOffset;
        fHigBound   =   fMaxValue + 0.2*(fMaxValue-fMinValue) + fOffset;
    }
    if ( fNofBins <= 0 )    {
        fNofBins = 12;
        if      ( fSizeOfAr >= 1.e2 )   fNofBins = (int)(fSizeOfAr/3.) + 2;
        if      ( fSizeOfAr >= 1.e3 )   fNofBins = (int)(fSizeOfAr/5.) + 2;
        if      ( fSizeOfAr >= 1.e3 )   fNofBins = 216;
    }
    TH1_Type   *fBuiltTH1_Type  =   new TH1_Type(Form("TH1_Type_from_vector_%i",iBuilderTH1_TypeCounter),Form("TH1_Type_from_vector_%i",iBuilderTH1_TypeCounter),fNofBins,fLowBound,fHigBound);
    for     ( auto iValue : fInputData )    fBuiltTH1_Type->Fill( iValue + fOffset );
    iBuilderTH1_TypeCounter++;
    return  fBuiltTH1_Type;
}
//
//  --  --  --  TODO: Implement
template<   typename TH1_Type = TH2F,
            typename stdVec_Type,
            typename = typename std::enable_if<std::is_arithmetic<stdVec_Type>::value, stdVec_Type>::type >
TH1_Type*
uBuildTH2
 ( std::vector<stdVec_Type> fInputData, Int_t fNofBins = -1, Float_t fOffset = 0., Float_t fLowBound = 0, Float_t fHigBound = 0 )    {
    return  new TH1_Type;
}
//
//  --  --  --  TODO: Implement
template<   typename TH1_Type = TH3F,
            typename stdVec_Type,
            typename = typename std::enable_if<std::is_arithmetic<stdVec_Type>::value, stdVec_Type>::type >
TH1_Type*
uBuildTH3
 ( std::vector<stdVec_Type> fInputData, Int_t fNofBins = -1, Float_t fOffset = 0., Float_t fLowBound = 0, Float_t fHigBound = 0 )    {
    return  new TH1_Type;
}
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! HISTOGRAM MANIPULATION FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
//
//  --- --- --- --- --- --- BINNING FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
//  --  --  --  TODO: Generalise
template <  typename TH1Target_Type,
            typename TH1Source_Type >
void
uRebin1D
( TH1Target_Type*   fTarget,    TH1Source_Type*   fSource )  {
    auto    kPairDimension  =   uGetPairDimension( fTarget, fSource );
    if      ( kPairDimension != 1 ) {
        cout << "[ERROR] Histograms are inconsistent, skipping rebinning" << endl;
        return;
    }
    auto fCurrentBin        =   1;
    auto fCurrentMergeN     =   0;
    auto fCurrentContent    =   0.;
    auto fCurrentError      =   0.;
    for ( Int_t iBin = 0; iBin < fSource->GetNbinsX(); iBin++ ) {
        auto    fYvalue     =   fSource ->  GetBinContent   ( iBin );
        auto    fYerror     =   fSource ->  GetBinError     ( iBin );
        auto    fXvalue     =   fSource ->  GetBinCenter    ( iBin );
        auto    fLowEdge    =   fTarget ->  GetBinLowEdge   ( fCurrentBin + 1 );
        auto    fMinEdge    =   fTarget ->  GetBinLowEdge   ( fCurrentBin );
        if ( fXvalue < fMinEdge )   continue;
        if ( fXvalue > fLowEdge )   {
            fCurrentBin++;
            iBin--;
            fCurrentMergeN  =   0;
            fCurrentContent =   0;
            fCurrentError   =   0;
            continue;
        }
        fCurrentMergeN++;
        fCurrentContent +=  fYvalue;
        fCurrentError   +=  fYerror*fYerror;
        fTarget ->  SetBinContent   ( fCurrentBin, fCurrentContent / ( fCurrentMergeN*1. ) );
        fTarget ->  SetBinError     ( fCurrentBin, sqrt(fCurrentError) / ( fCurrentMergeN*1. ) );
    }
}
//
//  --  --  --  TODO: Implement
template <  typename TH2Taregt_Type,
            typename TH2Source_Type >
void
uRebin2D
( TH2Taregt_Type*   fTarget,    TH2Source_Type*   fSource )  {
    return;
}
//
//  --  --  --  TODO: Implement
template <  typename TH3Target_Type,
            typename TH3Source_Type >
void
uRebin3D
( TH3Target_Type*   fTarget,    TH3Source_Type*   fSource )  {
    return;
}
//
//  --  --  --  TODO: Avoid in generalisation
template <  typename THXTarget_Type,
            typename THXSource_Type >
void
uRebin
( THXTarget_Type*   fTarget,    THXSource_Type*   fSource )  {
    auto    kPairDimension  =   uGetPairDimension( fTarget, fSource );
    if      ( kPairDimension == 1 ) uRebin1D(fTarget,fSource);
    else if ( kPairDimension == 2 ) uRebin2D(fTarget,fSource);
    else if ( kPairDimension == 3 ) uRebin3D(fTarget,fSource);
    else    {
        cout << "[ERROR] Histograms are inconsistent, skipping rebinning" << endl;
    }
}
//
template<   typename stdArr_Type,
            typename = typename std::enable_if<std::is_arithmetic<stdArr_Type>::value, stdArr_Type>::type >
Float_t*
uGetUniformBinningArray
( stdArr_Type fMinBin, stdArr_Type fMaxBin, Int_t fNBins ) {
    stdArr_Type*    fResult =   new stdArr_Type[fNBins+1];
    for ( Int_t iBin = 0; iBin < fNBins+1; iBin++ )    fResult[iBin]    =   fMinBin + ( iBin )*( fMaxBin - fMinBin ) / ( static_cast<Float_t>( fNBins ) );
    return  fResult;
}
//
template<   typename TInput1,
            typename TInput2,
            typename TInput3,
            typename = typename std::enable_if<std::is_arithmetic<TInput1>::value, TInput1>::type,
            typename = typename std::enable_if<std::is_arithmetic<TInput2>::value, TInput2>::type,
            typename = typename std::enable_if<std::is_arithmetic<TInput3>::value, TInput3>::type >
std::pair<Int_t,Float_t*>
uUniformBinning
( TInput1 fBinWidth, TInput2 fLowEdge, TInput3 fHighEdge )  {
    Int_t       nBinNumber  =   ( fHighEdge - fLowEdge ) / fBinWidth;
    if ( ( ( ( fHighEdge - fLowEdge ) / fBinWidth ) - nBinNumber ) >= fBinWidth ) nBinNumber++;
    Float_t*    fResult     =   new Float_t [ nBinNumber+1 ];
    for ( Int_t iBin = 0; iBin <= nBinNumber; iBin++ )  fResult[iBin]   =   fLowEdge + iBin * fBinWidth;
    if ( fResult[ nBinNumber ] != fHighEdge )  cout << "[WARNING] [uUniformBinning] Changed High Edge from " << fHighEdge << " to " << fResult[ nBinNumber ] << endl;
    return  std::pair<Int_t,Float_t*> ( nBinNumber, fResult );
}
//
//  --- --- --- --- --- --- DATA HANDLING FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
template<   typename TH1_Type >
void
uOffset
( TH1_Type* hTarget, Double_t kOffset, Bool_t kAbsolute = false ){
    for ( Int_t iBin = 1; iBin <= hTarget->GetNbinsX(); iBin++ ) {
        auto fYValue = hTarget->GetBinContent(iBin);
        if ( kAbsolute )    hTarget->SetBinContent(iBin, fabs( fYValue+kOffset ) );
        else                hTarget->SetBinContent(iBin, fYValue+kOffset );
    }
}
//
template<   typename TH1_Type >
void
uAbsolute
( TH1_Type* hTarget ){
    uOffset( hTarget, 0, kTRUE );
}
//
template<   typename THXTarget_Type,
            typename TInput1 = Double_t,
            typename TInput2 = Double_t,
            typename = typename std::enable_if<std::is_arithmetic<TInput1>::value, TInput1>::type,
            typename = typename std::enable_if<std::is_arithmetic<TInput2>::value, TInput2>::type >
THXTarget_Type*
uScale
 ( THXTarget_Type* hTarget, TInput1 fScaleFactor, Double_t fScaleError = -1. )  {
    auto    nDimension  =   uGetTHDimension( hTarget );
    auto    fResult     =   (THXTarget_Type*)(hTarget->Clone());
    if ( nDimension < 0 ) return fResult;
    for ( Int_t iBin = 1; iBin <= fResult->GetNbinsX(); iBin++ ) {
        for ( Int_t jBin = 1; jBin <= fResult->GetNbinsY(); jBin++ ) {
            for ( Int_t kBin = 1; kBin <= fResult->GetNbinsZ(); kBin++ ) {
                auto    kGlobalBin  =   fResult->GetBin( iBin, jBin, kBin );
                if ( fScaleFactor == -1 )       fResult ->  SetBinContent   ( kGlobalBin,   1 );
                else                            fResult ->  SetBinContent   ( kGlobalBin,   fScaleFactor * hTarget   ->  GetBinContent   ( iBin ) );
                if ( fScaleError == -1 )        fResult ->  SetBinError     ( kGlobalBin,   fScaleFactor * hTarget   ->  GetBinError     ( iBin ) );
                else if ( fScaleError == -2 )   fResult ->  SetBinError     ( kGlobalBin,   0 );
                else                            fResult ->  SetBinError     ( kGlobalBin,   fScaleFactor * hTarget   ->  GetBinContent   ( iBin ) * SquareSum( { hTarget->GetBinError( iBin ) / hTarget->GetBinContent( iBin ), fScaleError / fScaleFactor } ) );
                if ( fScaleFactor == -1 )       fResult ->  SetBinError     ( kGlobalBin,   hTarget   ->  GetBinError   ( iBin ) / hTarget   ->  GetBinContent   ( iBin ) );
            }
        }
    }
    return fResult;
}
//
// TODO: Generalise w/ THXTarget_Type_3 as return
template <  Bool_t      TSquareSum          = kTRUE,
            typename    THXTarget_Type_1    = TH1F,
            typename    THXTarget_Type_2    = TH1F  >
THXTarget_Type_1*
uSumErrors
 ( THXTarget_Type_1* hTarget_1, THXTarget_Type_2* hTarget_2 )    {
    THXTarget_Type_1*   fResult =   (THXTarget_Type_1*)(hTarget_1->Clone());
    if ( !uIsTHPairConsistent( hTarget_1, hTarget_2 ) ) return fResult;
    for ( Int_t iBin = 1; iBin <= fResult->GetNbinsX(); iBin++ ) {
        for ( Int_t jBin = 1; jBin <= fResult->GetNbinsY(); jBin++ ) {
            for ( Int_t kBin = 1; kBin <= fResult->GetNbinsZ(); kBin++ ) {
                auto    kGlobalBin  =   fResult->GetBin( iBin, jBin, kBin );
                auto    kNewError   =   0.;
                if ( TSquareSum )   kNewError   =   SquareSum( { hTarget_1->GetBinError(iBin), hTarget_2->GetBinError(iBin) } );
                else                kNewError   =   hTarget_1->GetBinError(iBin) + hTarget_2->GetBinError(iBin);
                fResult ->  SetBinError( iBin, kNewError );
            }
        }
    }
    return  fResult;
}
//
template <  Bool_t      TSquareSum      = kTRUE,
            typename    THXTarget_Type  = TH1F >
THXTarget_Type*
uSumErrors
 ( std::vector<THXTarget_Type*> hTarget )    {
    THXTarget_Type*   fResult =   (THXTarget_Type*)(hTarget.at(0)->Clone());
    Bool_t  bSkipFirst = true;
    for ( auto kCurrentHisto : hTarget )    {
        if ( bSkipFirst ) {
            bSkipFirst = false;
            continue;
        }
        fResult =   uSumErrors<TSquareSum,THXTarget_Type>( fResult, kCurrentHisto );
    }
    return  fResult;
}
//
template<   Bool_t TSquareSum = true,
            typename THXTarget_Type_1,
            typename THXTarget_Type_2 = THXTarget_Type_1 >
THXTarget_Type_2*
uRandomisePoints
( THXTarget_Type_1* hTarget ) {
    auto    nDimension  =   uGetTHDimension( hTarget );
    THXTarget_Type_2*  fResult =   (THXTarget_Type_2*)(hTarget->Clone());
    if ( nDimension < 0 )   return  fResult;
    for ( Int_t iBin = 1; iBin <= fResult->GetNbinsX(); iBin++ ) {
        for ( Int_t jBin = 1; jBin <= fResult->GetNbinsY(); jBin++ ) {
            for ( Int_t kBin = 1; kBin <= fResult->GetNbinsZ(); kBin++ ) {
                auto    kGlobalBin  =   fResult->GetBin         ( iBin, jBin, kBin );
                auto    kBinContent =   fResult->GetBinContent  ( kGlobalBin );
                auto    kBinError   =   fResult->GetBinError    ( kGlobalBin );
                auto    kNewBinCont =   uRandomGen -> Gaus( kBinContent, kBinError );
                fResult ->  SetBinContent( kGlobalBin, kNewBinCont );
            }
        }
    }
    return fResult;
}
//
template<   Bool_t TSquareSum = true,
            typename THXTarget_Type_1,
            typename THXTarget_Type_2,
            typename THXTarget_Type_3 = THXTarget_Type_1 >
THXTarget_Type_3*
uRandomisePoints
( THXTarget_Type_1* hTarget_1, THXTarget_Type_2* hTarget_2  ) {
    auto    nDimension  =   uGetTHDimension( hTarget_1 );
    THXTarget_Type_3*  fResult =   (THXTarget_Type_3*)(hTarget_1->Clone());
    if ( nDimension < 0 )   return  fResult;
    fResult =   uRandomisePoints( hTarget_1 );
    fResult =   uSumErrors      ( hTarget_1, hTarget_2 );
    return fResult;
}
//
//  --- --- --- --- --- --- WRITE/READ FILE FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
// TODO: Generalise for N cycles, for labels, for any TH1* if input std::vector& you can desume the TH1 type
template<   Int_t TDimension = 0,
            typename THXTarget_Type = TH1F,
            Bool_t   TResverse  =   false,
            typename TReturn_Type = typename TNVector< TDimension, THXTarget_Type* >::type >
TReturn_Type
uLoadHistograms
( TFile* kDataFile, TString kHistogramName, TString kNewName = ""  ) {
    TReturn_Type fResult;
    uInternalLoadHisto( fResult, kDataFile, kHistogramName, kNewName );
    return fResult;
}
template<   typename THXTarget_Type = TH1F >
void
uInternalLoadHisto
( THXTarget_Type*& fResult, TFile* kDataFile, TString kHistogramName, TString kNewName = "" ) {
    if (  kDataFile->Get(kHistogramName) ) fResult = ( THXTarget_Type* )( ( kDataFile->Get( kHistogramName ) )->Clone() );
    if ( !kNewName.IsNull() )   fResult->SetName((kNewName));
    if ( !fResult ) cout << "[ERROR] No Histogram match found for " << kHistogramName.Data() << endl;
    return fResult;
}
template<   typename THXTarget_Type = TH1F >
void
uInternalLoadHisto
( std::vector<THXTarget_Type*>& fResult, TFile* kDataFile, TString kHistogramName, TString kNewName = "" ) {
    auto iTer = 0;
    while ( true ) {
        if ( !kDataFile->Get(Form(kHistogramName,iTer)) ) break;
        fResult.push_back( (THXTarget_Type*)( ( kDataFile->Get( Form( kHistogramName, iTer ) ) )->Clone() ) );
        if ( !kNewName.IsNull() )    fResult.at(iTer)->SetName(Form(kNewName,iTer));
        iTer++;
    }
    if ( fResult.size() == 0 ) cout << "[ERROR] No Histogram match found for " << kHistogramName.Data() << endl;
}
template<   typename THXTarget_Type = TH1F >
void
uInternalLoadHisto
( std::vector<std::vector<THXTarget_Type*>>& fResult, TFile* kDataFile, TString kHistogramName, TString kNewName = "" ) {
    auto iTer = 0;
    while ( true ) {
        std::vector<THXTarget_Type*>   kUtility;
        auto jTer = 0;
        while ( true ) {
            if ( !kDataFile->Get(Form(kHistogramName,iTer,jTer)) ) break;
            kUtility.push_back( (THXTarget_Type*)( ( kDataFile->Get( Form( kHistogramName, iTer, jTer ) ) )->Clone() ) );
            if ( !kNewName.IsNull() )    kUtility.at(jTer)->SetName(Form(kNewName,iTer,jTer));
            jTer++;
        }
        if ( jTer == 0 ) break;
        fResult.push_back( kUtility );
        iTer++;
    }
    if ( fResult.size() == 0 ) cout << "[ERROR] No Histogram match found for " << kHistogramName.Data() << endl;
}
template<   typename THXTarget_Type = TH1F >
void
uInternalLoadHisto
( std::vector<std::vector<std::vector<THXTarget_Type*>>>& fResult, TFile* kDataFile, TString kHistogramName, TString kNewName = "" ) {
    auto iTer = 0;
    while ( true ) {
        std::vector<std::vector<THXTarget_Type*>>  kUtilit2;
        auto jTer = 0;
        while ( true ) {
            std::vector<THXTarget_Type*>   kUtility;
            auto kTer = 0;
            while ( true ) {
                if ( !kDataFile->Get(Form(kHistogramName,iTer,jTer,kTer)) ) break;
                kUtility.push_back( (THXTarget_Type*)( ( kDataFile->Get( Form( kHistogramName, iTer, jTer, kTer ) ) )->Clone() ) );
                if ( !kNewName.IsNull() )    kUtility.at(kTer)->SetName(Form(kNewName,iTer,jTer,kTer));
                kTer++;
            }
            if ( kTer == 0 ) break;
            kUtilit2.push_back( kUtility );
            jTer++;
        }
        if ( jTer == 0 ) break;
        fResult.push_back( kUtilit2 );
        iTer++;
    }
    if ( fResult.size() == 0 ) cout << "[ERROR] No Histogram match found for " << kHistogramName.Data() << endl;
}
//
template<   typename THXTarget_Type,
            typename TInput1 = Float_t,
            typename = typename std::enable_if<std::is_arithmetic<TInput1>::value > >
void
uAddSumHistogram
 ( std::vector<THXTarget_Type*> &hTarget, TString kNewName = "", std::vector<TInput1> kWeights = {} ) {
    // TODO: null vec, warning weitghs less than histos
    //if ( size == 0 ) warnign erorr
    auto    iTer = 0;
    auto    hResult =   (THXTarget_Type*)(hTarget.at(0)->Clone());
    hResult ->  Reset();
    for ( auto kHisto : hTarget )    {
        if ( kWeights.size() < iTer+1 ) hResult -> Add( kHisto );
        else                            hResult -> Add( kHisto, kWeights.at(iTer) );
        iTer++;
    }
    if ( kNewName.IsNull() )    hResult ->  SetName( "SumHisto_from_uAddSumHistogram" );
    else                        hResult ->  SetName( kNewName );
    hTarget                     .insert( hTarget.begin(), hResult );
}
//
template<   typename THXTarget_Type,
            typename TInput1,
            typename = typename std::enable_if<std::is_arithmetic<TInput1>::value > >
void
uAddSumHistogram
 ( std::vector<std::vector<THXTarget_Type*>> &hTarget, TString kNewName = "", std::vector<TInput1> kWeights = {} ) {
    std::vector<THXTarget_Type*>  fResult;
    for ( Int_t jTer = 0; jTer < hTarget.at(0).size(); jTer++ )   {
        std::vector<THXTarget_Type*> kUtility;
        for ( Int_t iTer = 0; iTer < hTarget.size(); iTer++ )   {
            kUtility.push_back( hTarget.at(iTer).at(jTer) );
        }
        uAddSumHistogram( kUtility, Form( kNewName, jTer), kWeights );
        fResult.push_back( kUtility.at(0) );
    }
    hTarget.push_back( fResult );
}
//
//TODO: Implement
template<   typename THXTarget_Type = TH1F >
std::vector<std::vector<THXTarget_Type*>>
uReverseStructure
( std::vector<std::vector<THXTarget_Type*>> hTarget ) {
    std::vector<std::vector<THXTarget_Type*>> fResult;
    for ( Int_t iTer = 0; iTer < hTarget.at(0).size(); iTer++ )  {
        std::vector<THXTarget_Type*> kUtility;
        for ( Int_t jTer = 0; jTer < hTarget.size(); jTer++ ) {
            kUtility.push_back( (THXTarget_Type*)hTarget[jTer][iTer]->Clone() );
        }
        fResult.push_back(  *(new std::vector<THXTarget_Type*> (kUtility)) );
    }
    return fResult;
}
//
template<   typename THXTarget_Type = TH1F >
std::vector<std::vector<std::vector<THXTarget_Type*>>>
uReverseStructure
( std::vector<std::vector<std::vector<THXTarget_Type*>>> hTarget ) {
    std::vector<std::vector<std::vector<THXTarget_Type*>>> fResult;
    for ( Int_t iTer = 0; iTer < hTarget.at(0).size(); iTer++ )  {
        std::vector<std::vector<THXTarget_Type*>> kUtilit2;
        for ( Int_t jTer = 0; jTer < hTarget.size(); jTer++ ) {
            std::vector<THXTarget_Type*> kUtility;
            for ( Int_t kTer = 0; kTer < hTarget.size(); kTer++ ) {
                kUtility.push_back( (THXTarget_Type*)hTarget[jTer][kTer][iTer]->Clone() );
            }
            kUtilit2.push_back(  *(new std::vector<THXTarget_Type*> (kUtility)) );
        }
        fResult.push_back(  *(new std::vector<std::vector<THXTarget_Type*>> (kUtilit2)) );
    }
    return fResult;
}
//
template<   Int_t TDimension = 2,
            typename THXTarget_Type = TH1F,
            typename TTarget_Type = typename TNVector< TDimension, THXTarget_Type* >::type >
TTarget_Type
uReverseStructure
( TTarget_Type hVecTarget ) {
    return uReverseStructure(hVecTarget);
}
//
#endif
