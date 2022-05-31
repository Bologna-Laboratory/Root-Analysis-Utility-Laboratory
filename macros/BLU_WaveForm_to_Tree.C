//
//  Part of the BolognaLaboratoryUtility package
//
//  Global Functions
//
//  Author              Nicola Rubini, Sofia Strazzi
//  Created             22/11/2021
//  Last modified       22/11/2021
#ifndef BOLOGNALABORATORYUTILITY_GLOBALUTILFUNCTIONS_H
#define BOLOGNALABORATORYUTILITY_GLOBALUTILFUNCTIONS_H
//
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//  --- --- --- --- --- --- //! GENERAL UTILITY FUNCTIONS
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
template    <   typename TTarget  >
int
sgn( TTarget uTarget )    { return ( TTarget(0) < uTarget ) - ( uTarget < TTarget(0) ); }
//
Double_t
uSquareSum
 ( std::initializer_list<Double_t> list )  {
    Double_t    fResult =   0;
    for ( auto element : list ) fResult +=  element*element;
    return  TMath::Sqrt(fResult);
}
//
//  --- --- --- --- --- --- /
//  --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---
//
#endif
