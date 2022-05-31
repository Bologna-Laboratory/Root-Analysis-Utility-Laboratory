//
//  Part of the BolognaLaboratoryUtility package
//
//  General File to be included in the individual analyses, macros, etc.
//
//  Authors             Nicola Rubini, Sofia Strazzi
//  Created             28/03/2022
//  Last modified       28/03/2022
#ifndef BOLOGNALABORATORYUTILITY_H
#define BOLOGNALABORATORYUTILITY_H
//
//  --- Include All Library File
//  --- --- General Library
#include "./library/BLU_GlobalUtilVariables.h"
#include "./library/BLU_GlobalUtilFunctions.h"
#include "./library/BLU_Histograms.h"
#include "./library/BLU_Graphs.h"
//  --- --- SiPM Library
#include "./library/SiPM/BLU_BreakdownVoltage.h"
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY VARIABLES
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
//  --- --- --- --- --- --- GENERAL UTILITY DATA STRUCTURES
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
typedef struct {
    Int_t       nEvents,    nLines,             nChannel;
    Float_t     fVoltage,   fAmplitude[50000],  fTime[50000],   fDerivative[50000];
} SDataStructure;
//
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
void
uLoadDataFromFile
( TString fOutputFile, std::vector<TString> fInputFile, Int_t nChannel, Float_t fTension, Int_t iNaverage, TString fOption = "", TString fCustomName = "LGAD_Measurements" ) {
    //
    //    ---    Output Tree Variables
    SDataStructure    kOutputFileData;
    //
    //    ---    Build output Tree
    TTree*    tOutputTree    = new TTree( fCustomName, fCustomName );
    tOutputTree    -> Branch( "nEvents",        &kOutputFileData.nEvents,       "nEvents/I"                 );
    tOutputTree    -> Branch( "nLines",         &kOutputFileData.nLines,        "nLines/I"                  );
    tOutputTree    -> Branch( "nChannel",       &kOutputFileData.nChannel,      "nChannel/I"                );
    tOutputTree    -> Branch( "fVoltage",       &kOutputFileData.fVoltage,      "fVoltage/F"                );
    tOutputTree    -> Branch( "fAmplitude",     &kOutputFileData.fAmplitude,    "fAmplitude[nLines]/F"      );
    tOutputTree    -> Branch( "fTime",          &kOutputFileData.fTime,         "fTime[nLines]/F"           );
    tOutputTree    -> Branch( "fDerivative",    &kOutputFileData.fDerivative,   "fDerivative[nLines]/F"     );
    //
    //    ---    Read File
    Int_t iEvent = 0;
    for ( auto kCurrentFile : fInputFile ) {
        //
        //    Load Graph from file
        TGraph* gUtility_from_file = new TGraph(kCurrentFile,"%lg %lg"," \\t,;");
        //
        //    ---    Check the file is correctly read/ not empty
        if ( gUtility_from_file->GetN() == 0 ) {
            cout << Form("[WARNING][uLoadDataFromFile] File %s empty",kCurrentFile.Data()) << endl;
            cout << "[WARNING][uLoadDataFromFile] Skipping this file" << endl;
            continue;
        }
        //
        auto gUtility = uMovingAverage(gUtility_from_file,iNaverage);
        auto gUtilit2 = uMovingAverage(gUtility_from_file,iNaverage);
        if ( fOption.Contains("-X") )   gUtility = uInvertX     (gUtility);
        if ( fOption.Contains("-Y") )   gUtility = uInvertY     (gUtility);
        if ( fOption.Contains("-XY") )  gUtility = uInvertXY    (gUtility);
        if ( fOption.Contains("DRV") )  gUtilit2 = uDerivative  (uMakeMeTGraphErrors(gUtility));
        //
        //    ---    Write To Tree
        kOutputFileData.nEvents     = iEvent;
        kOutputFileData.nLines      = gUtility->GetN();
        kOutputFileData.nChannel    = nChannel;
        kOutputFileData.fVoltage    = fTension;
        for ( Int_t iPnt = 0; iPnt < gUtility->GetN(); iPnt++ )    {
            kOutputFileData.fTime       [iPnt]    =    gUtility->GetPointX(iPnt);
            kOutputFileData.fAmplitude  [iPnt]    =    gUtility->GetPointY(iPnt);
            kOutputFileData.fDerivative [iPnt]    =    gUtilit2->GetPointY(iPnt);
        }
        tOutputTree    ->Fill();
        iEvent++;
    }
    //
    //    ---    Save To File
    TFile*    kOutputFile = new TFile( fOutputFile, "RECREATE" );
    tOutputTree->Write();
    kOutputFile->Close();
}
//
void
uLoadDataFromFile
( TString fOutputFile, TString fInputFile, Int_t nChannel, Float_t fTension, TString fOption = "", TString fCustomName = "LGAD_Measurements" ) {
    uLoadDataFromFile(fOutputFile,{fInputFile},nChannel,fTension,fOption,fCustomName);
}
//
#endif
