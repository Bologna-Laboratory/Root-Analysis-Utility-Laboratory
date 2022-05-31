//
//  Part of the AliAnalysisUtility package
//
//  Utilities for Histogram handling
//
//  Author              Nicola Rubini
//  Created             22/11/2021
//  Last modified       23/03/2022
//
#ifndef ALIANALYSISUTILITY_HISTOGRAMS_H
#define ALIANALYSISUTILITY_HISTOGRAMS_H
//
//  TODO: Implement histogram check for all functions
//  TODO: Implement general function w/ warning and error messages for histogram check and bool to enable work on histo
//
//  Global File w/ Constants and other functions
#include "../../BolognaLaboratoryUtility.h"
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY VARIABLES
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! BREAKDOWN VOLTAGE MEASUREMENT
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
// !TODO: Re-write and unpack sub-functions
/*
std::pair<Double_t,Double_t>
fMeasureVbd
 ( TGraphErrors *gIV, Double_t fGuessVbd = 25, Int_t fMode = 0, Double_t fMnusRng = 1, Double_t fPlusRng = 4, TString fName = "" )  {
    
    fMode = 0;
    
    TString fSensor = "UNK";
    if ( fName.Contains("FBK") ) fSensor = "FBK";
    TString fSerial = "_snX";
    for ( Int_t iSer = 0; iSer < 10; iSer++ )   {
        if ( fName.Contains(Form("_sn%i",iSer)) ) fSerial = Form("_sn%i",iSer);
    }
    TString fStep = "_0";
    for ( Int_t iStp = 0; iStp < 10; iStp++ )   {
        if ( fName.Contains(Form("_%i_",iStp)) ) fStep = Form("_%i",iStp);
    }
    
    TString fFolder = Form("./Data/plots/VBD/%s%s%s/",fSensor.Data(),fSerial.Data(),fStep.Data());
    gROOT->ProcessLine( Form(".! mkdir -p ./%s",fFolder.Data()) );
    
    
    TGraphErrors*   gILD;
    TCanvas*        c1;
    TF1*            fFitresult;
    auto fTolerance = 1.e-5;
    auto fIncr = -.0001;
    auto m = 0.;
    auto q = 0.;
    auto T0= 0.;
    m = 0.026;
    q = 26.9;
    T0= 298;
    if ( fName.Contains("BCOM") ) {
        m = 0.026;
        q = 26.9;
        T0= 298;
    }   if ( fName.Contains("FBK") ) {
        m = 0.035;
        q = 32;
        T0= 297;
    }
    auto temp = 0.;
    if ( fName.Contains("243K") ) temp = 243;
    if ( fName.Contains("263K") ) temp = 263;
    if ( fName.Contains("293K") ) temp = 293;
    auto TargetTemp = (temp-T0)*m+q;
    TLatex * ftext = new TLatex();
    TGraph * fTemperatureTarget = new TGraph();
    fTemperatureTarget->SetLineColor(kBlue);
    fTemperatureTarget->SetLineStyle(kDashed);
    fTemperatureTarget->SetLineWidth(3);
    for ( Int_t i = 0; i < 10; i++ )   {
        auto fYvalue = 0.1 + i*1.e-1;
        fTemperatureTarget->SetPoint(i,TargetTemp,fYvalue);
    }
    switch ( fMode )    {
            // Fit of IV with (x-x0)^n
        case 2: {
            gILD = (gIV);
            c1 = new TCanvas("","",800,800);
            c1->SetLogy();
            gILD->SetLineColor(kBlack);
            gILD->SetMarkerStyle(20);
            gILD->Fit("fBaseline","MRS","Q",0,fGuessVbd-fMnusRng);
            fMeasVb2->FixParameter(1,fGuessVbd);
            fMeasVb2->FixParameter(2,2);
            auto fpowerLaw =  gILD->Fit("fMeasVb2","MRS","Q",fGuessVbd+fMnusRng,fGuessVbd+fPlusRng);
            fMeasVb2->ReleaseParameter(1);
            fMeasVb2->ReleaseParameter(2);
            fMeasVb2->SetParLimits(2,0.,100.);
            auto fBaseLine = gILD->Fit("fMeasVb2","MRS","Q",fGuessVbd+fMnusRng,fGuessVbd+fPlusRng);
            auto _CurrentX = fGuessVbd + 3;
            for ( Int_t iTer = 0; iTer < 100; iTer++ )  {
                _CurrentX = fGuessVbd + 3;
                for ( Int_t iTer = 0; iTer < 150000; iTer++ )  {
                    auto x1 = fabs(fBaseline ->Eval(_CurrentX));
                    auto x2 = fabs(fMeasVb2  ->Eval(_CurrentX));
                    if ( x2 < x1 ) break;
                    _CurrentX += fIncr;
                }
                if ( fabs(fGuessVbd - _CurrentX ) < fTolerance ) break;
                fGuessVbd = _CurrentX;
                fpowerLaw = gILD->Fit("fMeasVb2",   "MRS",  "Q", fGuessVbd,fGuessVbd+fPlusRng);
                fBaseLine = gILD->Fit("fBaseline",  "MRS",  "Q", fGuessVbd-fPlusRng,fGuessVbd-fMnusRng);
            }
            // ERROR Evaluation
            
            Double_t fMeasuredVbdPnt[1]     =   {fGuessVbd};
            
            Double_t fEvaluatePowerLaw[1]   =   {fMeasVb2->Eval(fGuessVbd)};
            Double_t fErrorOnPowerLaw[1];
            fpowerLaw->GetConfidenceIntervals(1,1,1,fMeasuredVbdPnt,fErrorOnPowerLaw,0.683,false);
            
            Double_t fEvaluateBaseLine[1]   =   {fMeasVb2->Eval(fGuessVbd)};
            Double_t fErrorOnBaseLine[1];
            fBaseLine->GetConfidenceIntervals(1,1,1,fMeasuredVbdPnt,fErrorOnBaseLine,0.683,false);
            
            auto fPar0 = fBaseline->GetParameter(0);
            auto fPEr0 = fBaseline->GetParError(0);
            auto fPar1 = fBaseline->GetParameter(1);
            auto fPEr1 = fBaseline->GetParError(1);
            
            fBaseline->SetParameter(0,fPar0+fPEr0);
            fBaseline->SetParameter(1,fPar1-fPEr1);
            _CurrentX = fGuessVbd + 3;
            for ( Int_t iTer = 0; iTer < 150000; iTer++ )  {
                auto x1 = fabs(fBaseline ->Eval(_CurrentX));
                auto x2 = fabs(fMeasVb2  ->Eval(_CurrentX));
                if ( x2 < x1 ) break;
                _CurrentX += fIncr;
            }
            auto fHighError = _CurrentX;
            
            fBaseline->SetParameter(0,fPar0-fPEr0);
            fBaseline->SetParameter(1,fPar1+fPEr1);
            _CurrentX = fGuessVbd + 3;
            for ( Int_t iTer = 0; iTer < 150000; iTer++ )  {
                auto x1 = fabs(fBaseline ->Eval(_CurrentX));
                auto x2 = fabs(fMeasVb2  ->Eval(_CurrentX));
                if ( x2 < x1 ) break;
                _CurrentX += fIncr;
            }
            auto fLow_Error = _CurrentX;
            
            fBaseline->SetParameter(0,fPar0);
            fBaseline->SetParameter(1,fPar1);
            auto fError = 0.5*fabs(fHighError-fLow_Error);
            
            //
            gILD->GetXaxis()->SetTitle("V_{bias} (V)");
            gILD->GetYaxis()->SetTitle("I (A)");
            gILD->GetYaxis()->SetTitleOffset(1.);
            gILD->Draw("ape");
            fBaseline->SetRange(0,60);
            fBaseline->DrawCopy("SAME");
            fMeasVb2->SetRange(fMeasVb2->GetParameter(1),100);
            fMeasVb2->Draw("SAME");
            c1->SaveAs(Form("%s/CHFit_%s.pdf",fFolder.Data(),fName.Data()));
            // Close UP
            gILD->GetXaxis()->SetRangeUser(fGuessVbd-3,fGuessVbd+3);
            gILD->SetMaximum(fMeasVb2->Eval(fGuessVbd+3.5));
            ftext->DrawLatexNDC(0.2,0.8,fName.Data());
            ftext->DrawLatexNDC(0.2,0.7,Form("Vbd = %.3f +- %.3f",fGuessVbd,fError));
            ftext->DrawLatexNDC(0.2,0.6,Form("DTS = %.3f",TargetTemp));
            for ( Int_t i = 0; i < 1000000; i++ )   {
                auto fYvalue = i*1.e-12;
                if ( (fMeasVb2->Eval(fGuessVbd+1)) < fYvalue ) break;
                fTemperatureTarget->SetPoint(i,TargetTemp,fYvalue);
            }
            fTemperatureTarget->Draw("same L ");
            fBaseline->DrawCopy("SAME");
            fBaseline->SetParameter(0,fPar0+fPEr0);
            fBaseline->SetParameter(1,fPar1-fPEr1);
            fBaseline->DrawCopy("SAME");
            fBaseline->SetParameter(0,fPar0-fPEr0);
            fBaseline->SetParameter(1,fPar1+fPEr1);
            fBaseline->DrawCopy("SAME");
            

            c1->SaveAs(Form("%s/CHFit_%s_CloseUp.pdf",fFolder.Data(),fName.Data()));
            gILD->SetMaximum(fBaseline->Eval(fGuessVbd+3.5));
            c1->SetLogy(false);
            c1->SaveAs(Form("%s/CHFit_%s_CloseUpNoLog.pdf",fFolder.Data(),fName.Data()));
            delete c1;
            return std::pair<Double_t,Double_t> (fGuessVbd,fError);
        break;
        }
            // Linear fit of ILD(gIV)
        default: {
            gILD = ILD(gIV);
            c1 = new TCanvas("","",800,800);
            gILD->SetMinimum(0.);
            gILD->SetMaximum(5.);
            gILD->SetLineColor(kBlack);
            gILD->SetMarkerStyle(20);
            gILD->Fit("fMeasVbd","MRS","",fGuessVbd+fMnusRng,fGuessVbd+fPlusRng);
            fFitresult = gILD->GetFunction("fMeasVbd");
            for ( Int_t iTer = 0; iTer < 100; iTer++ )  {
                if ( fabs(fGuessVbd - fFitresult->GetParameter(1)) < fTolerance ) break;
                fGuessVbd = fFitresult->GetParameter(1);
                gILD->Fit("fMeasVbd","MRS","",fGuessVbd+fMnusRng,fGuessVbd+fPlusRng);
                fFitresult = gILD->GetFunction("fMeasVbd");
            }
            fFitresult->SetRange(25.,30.);
            fFitresult->Draw("same");
            gILD->GetXaxis()->SetTitle("V_{bias} (V)");
            gILD->GetYaxis()->SetTitle("Inverse Log Derivative");
            gILD->Draw("ape");
            ftext->DrawLatexNDC(0.2,0.8,fName.Data());
            ftext->DrawLatexNDC(0.2,0.7,Form("Vbd = %.3f",fGuessVbd));
            ftext->DrawLatexNDC(0.2,0.6,Form("DTS = %.3f",TargetTemp));
            fTemperatureTarget->Draw("same L ");
            c1->SaveAs(Form("./Data/plots/CHFit_1_%s.pdf",fName.Data()));
            //delete c1;
            return std::pair<Double_t,Double_t> (fMeasVbd->GetParameter(1),fMeasVbd->GetParError(1));
        break;
        }
    }
}
 */
//
#endif
