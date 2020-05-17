//
//  SMARTSlib.h
//  SMARTSlib
//
//  Libary contains functions for driving the SMARTS program installed in ./data/SMARTS_295_OS folder
//

#ifndef SMARTSlib_h
#define SMARTSlib_h

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "SPDlib.h"

#define S_W_MIN 360    //360nm is bottom of visible range as used by SMARTS
#define S_W_MAX 830    //830nm is top of visible range as use by SMARTS

//global variables
struct ALLsensors{
    struct SPD_data *direct_normal;
    struct SPD_data *diffuse_horizontal;
    struct SPD_data *global_horizontal;
    struct SPD_data *direct_horizontal;
};

struct location_data{
    char *name;
    float latit;
    float longit;
    float altit;
    char *atmos;
    int C02;
    char *aerosol;
    float aerodepth;
    int aldebo;
    int timezone;
};

struct time_data{
    int year;
    int month;
    int day;
    float local_standard_hour24;
    float local_standard_minutes;
    float local_standard_seconds_from_midnight;
};

//function declarations
int SMARTSsettup(struct time_data *time, struct location_data *location);
int runSMARTS(int OS);
struct ALLsensors* readSMARTS(int len, int cmin, int cmax, int cstep);


#endif /* SMARTSlib_h */