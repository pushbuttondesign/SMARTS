#include <stdio.h>
#include "SMARTSlib.h"
#include "SPDlib.h"

int main() {
    
    //SMARTSsettup example
    struct time_data *time;
    struct location_data *location;
    time = (struct time_data*)calloc(1, sizeof(struct time_data));
    location = (struct location_data*)calloc(1, sizeof(struct location_data));
    location->name = (char*)calloc(50, sizeof(char));
    location->atmos = (char*)calloc(10, sizeof(char));
    location->aerosol = (char*)calloc(15, sizeof(char));
    
    //fill variables with example values
    strncpy(location->name, "Vancouver", 30);
    location->latit = 49.2827;
    location->longit = 123.1207;
    location->altit = 0.09;
    strncpy(location->atmos, "\'STS\'", 10);             //CARD3a
    location->C02 = 370;                                 //CARD7 constant
    strncpy(location->aerosol, "\'S&F_URBAN\'", 15);     //CARD8
    location->aerodepth = 0.083;                         //CARD9
    location->aldebo = 42;                               //CARD10
    location->timezone = 8;                              //CARD17a
    time->local_standard_hour24 = 13.00;
    time->day = 12;
    time->month = 09;
    
    //setup time and location
    SMARTSsettup(time, location);
    
    free(location->name);
    free(location->atmos);
    free(location->aerosol);
    free(location);
    free(time);
    
    //runSMARTS example
    runSMARTS(0);
    
    //readSMARTS example
    struct ALLsensors *TESTas;
    TESTas = readSMARTS((830 - 360), 1000, 10000, 10);
    
    if (TESTas != NULL) {
        printf("\ntotal gh irradiance value of data in ./data/SMARTS_295_Mac/smarts.inp.txt is: %Lf\n", TESTas->global_horizontal->irradiance);
        
        //print results
        printf("wavelength (nm) | GH spectral irradiance | DRN spectral irradiance\n");
        for (int i = 0; i < TESTas->direct_normal->arr_len; i++) {
            printf("%.0Lf %Le %Le\n", TESTas->global_horizontal->wavelength[i], TESTas->global_horizontal->spec_irradiance[i], TESTas->direct_normal->spec_irradiance[i]);
        }
        
        
        free(TESTas->direct_normal->wavelength);
        free(TESTas->direct_normal->spec_irradiance);
        free(TESTas->direct_normal->spec_irrad_normalised);
        
        free(TESTas->diffuse_horizontal->wavelength);
        free(TESTas->diffuse_horizontal->spec_irradiance);
        free(TESTas->diffuse_horizontal->spec_irrad_normalised);
        
        free(TESTas->direct_horizontal->wavelength);
        free(TESTas->direct_horizontal->spec_irradiance);
        free(TESTas->direct_horizontal->spec_irrad_normalised);
        
        free(TESTas->global_horizontal->wavelength);
        free(TESTas->global_horizontal->spec_irradiance);
        free(TESTas->global_horizontal->spec_irrad_normalised);
        
        free(TESTas->diffuse_horizontal);
        free(TESTas->direct_horizontal);
        free(TESTas->direct_normal);
        free(TESTas->global_horizontal);
        free(TESTas);
    }
    
    return 0;
}