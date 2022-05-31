//
//  Part of the BolognaLaboratoryUtility package
//
//  Global Variables
//
//  Author              Nicola Rubini, Sofia Strazzi
//  Created             22/11/2021
//  Last modified       22/11/2021
#ifndef BOLOGNALABORATORYUTILITY_GLOBALUTILVARIABLES_H
#define BOLOGNALABORATORYUTILITY_GLOBALUTILVARIABLES_H
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY VARIABLES
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
TRandom*        uRandomGen  = new TRandom();
TBenchmark*     uBenchmark  = new TBenchmark();
TLatex*         uLatex      = new TLatex();
TString         uName       = "Name";
TString         uTitle      = "Title";
//
//  --- --- --- --- --- --- TYPDE DEFINITION
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
template< Int_t TDimension, typename TContent_Type >    struct TNVector                         { typedef std::vector< typename TNVector< TDimension - 1, TContent_Type >::type > type; };
template< typename TContent_Type >                      struct TNVector < 0, TContent_Type >    { typedef TContent_Type type; };
//
#endif
