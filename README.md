# Bologna Laboratory Utility
This software library holds generic functions that might be useful for Histograms and Graphs handling, together with specific functions for each detector type. The structure is as follows:
- Library:
    - BolognaLaboratoryUtility.h is the main include file, which should be included for all analysese and macros to use all utility tools.
    - library: this directory holds all include files divided per topic.
        - BLU_Whatever.h is the standard naming convention, at the moment are implemented:
            - BLU_GlobalUtilVariables.h : Global Variables
            - BLU_GlobalUtilFunctions.h : Global Functions
            - BLU_Graphs.h : Variables and Functions that handle Graphs
            - BLU_Histograms.h : Variables and Functions that handle Histograms
            
            - SiPM: this directory holds all include files that are pertinent to SiPMs analyses
                - BLU_BreakdownVoltage.h : All methods measuring Vbd for SiPMs

Library should always be inlcuded in all macros, you only need to include BolognaLaboratoryUtility.h in order to access all functions.

- Macros:
    - [To be updated] A place to store all macros
    
An example of macro can be:

//--------
\#include "path/to/BolognaLaboratoryUtility.h"

void
MyMacro
( MyInputs ) {
    MyFunction
}

//--------
