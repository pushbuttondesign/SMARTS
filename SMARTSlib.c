//
//  SMARTSlib.c
//  SMARTSlib
//
//  Libary contains functions for driving the SMARTS program installed in ./data/SMARTS_295_OS folder
//

//include & define
#include "SMARTSlib.h"

//
//  struct ALLsensors* readSMARTS(int len, int cmin, int cmax, int cstep);
//
//  Opens and reads ./smarts295.out.txt, putting data into an ALLsensor structure
//
//  EXTERNAL REQUIREMENTS: must free(): ALLsensor plus wavelength, spec_irradiance, spec_irrad_normalised, irradiance, colour temprature and arr_len vectors for each sensor type: direct_normal, diffuse_horizontal, global_horizontal
//  INPUTS: - length of wavelength data (number of data lines), int
//  OUTPUT: - pointer to ALLsensor structure with relevent data vectors malloc()'ed and filled with data from smarts295.out.txt, ALLsensor*
//  NOTE: the number of lines to skip varies depending on the variables output by SMARTS as specified in smarts295.inp.txt file, this is currently not accounted for in code
//

struct ALLsensors* readSMARTS(int len, int cmin, int cmax, int cstep) {
    //check arguments
    if (len < 1) {
        fprintf(stderr, "ERROR: length of data vector must be a positive interger to read smarts295.out.txt\n");
        return NULL;
    }
    
    struct ALLsensors *senarr;
    
    //allocate memory
    senarr = (struct ALLsensors*)calloc(1, sizeof(struct ALLsensors));
    senarr->diffuse_horizontal = (struct SPD_data*)calloc(1, sizeof(struct SPD_data));
    senarr->direct_horizontal = (struct SPD_data*)calloc(1, sizeof(struct SPD_data));
    senarr->direct_normal = (struct SPD_data*)calloc(1, sizeof(struct SPD_data));
    senarr->global_horizontal = (struct SPD_data*)calloc(1, sizeof(struct SPD_data));
    
    senarr->direct_normal->wavelength = (long double*)malloc(sizeof(long double) * len);
    senarr->direct_normal->spec_irradiance = (long double*)malloc(sizeof(long double) * len);
    senarr->direct_normal->spec_radiance = NULL;
    senarr->direct_normal->spec_rad_normalised = NULL;
    senarr->direct_normal->spec_irrad_normalised = (long double*)malloc(sizeof(long double) * len);
    
    senarr->diffuse_horizontal->wavelength = (long double*)malloc(sizeof(long double) * len);
    senarr->diffuse_horizontal->spec_irradiance = (long double*)malloc(sizeof(long double) * len);
    senarr->diffuse_horizontal->spec_radiance = NULL;
    senarr->diffuse_horizontal->spec_rad_normalised = NULL;
    senarr->diffuse_horizontal->spec_irrad_normalised = (long double*)malloc(sizeof(long double) * len);
    
    senarr->direct_horizontal->wavelength = (long double*)malloc(sizeof(long double) * len);
    senarr->direct_horizontal->spec_irradiance = (long double*)malloc(sizeof(long double) * len);
    senarr->direct_horizontal->spec_radiance = NULL;
    senarr->direct_horizontal->spec_rad_normalised = NULL;
    senarr->direct_horizontal->spec_irrad_normalised = (long double*)malloc(sizeof(long double) * len);
    
    senarr->global_horizontal->wavelength = (long double*)malloc(sizeof(long double) * len);
    senarr->global_horizontal->spec_irradiance = (long double*)malloc(sizeof(long double) * len);
    senarr->global_horizontal->spec_radiance = NULL;
    senarr->global_horizontal->spec_rad_normalised = NULL;
    senarr->global_horizontal->spec_irrad_normalised = (long double*)malloc(sizeof(long double) * len);
    
    senarr->direct_normal->arr_len = len;
    senarr->diffuse_horizontal->arr_len = len;
    senarr->direct_horizontal->arr_len = len;
    senarr->global_horizontal->arr_len = len;
    
    //initalize irradiance
    senarr->direct_normal->irradiance = 0;
    senarr->direct_horizontal->irradiance = 0;
    senarr->diffuse_horizontal->irradiance = 0;
    senarr->global_horizontal->irradiance = 0;
    
    FILE *fp = fopen("./data/SMARTS_295_Mac/smarts295.out.txt", "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: could not open ./data/SMARTS_295_Mac/smarts295.out.txt\n");
        exit(EXIT_FAILURE);
    }
    
    //search for data
    int count = 0;
    char datastr[] = "WVLGTH", *teststr;
    teststr = (char*)calloc(500, sizeof(char));
    do {
        fscanf(fp, "%s", teststr);
        count++;
    } while (strncmp(teststr, datastr, 6) != 0 && count != 1000);
    free(teststr);
    if (count >= 1000){
        free(senarr->direct_normal->wavelength);
        free(senarr->direct_normal->spec_irradiance);
        
        free(senarr->diffuse_horizontal->wavelength);
        free(senarr->diffuse_horizontal->spec_irradiance);
        
        free(senarr->direct_horizontal->wavelength);
        free(senarr->direct_horizontal->spec_irradiance);
        
        free(senarr->global_horizontal->wavelength);
        free(senarr->global_horizontal->spec_irradiance);
        
        free(senarr->diffuse_horizontal);
        free(senarr->direct_horizontal);
        free(senarr->direct_normal);
        free(senarr->global_horizontal);
        free(senarr);
        
        return NULL;
    }
    
    //count down 3 lines
    //NOTE: the number of lines to skip varies depending on the variables output by SMARTS as specified in smarts295.inp.txt file
    char c = '0';
    int lines = 0;
    while (lines < 3) {
        while (c != '\n') {
            c = fgetc(fp);
        }
        c = '0';    //reset char
        lines++;    //count a line
    }
    
    for (int i = 0; i < len; i++) {
        //read data
        fscanf(fp, "%Lf %*f %LE %LE %LE %LE %*E %*E", &senarr->diffuse_horizontal->wavelength[i], &senarr->direct_normal->spec_irradiance[i], &senarr->direct_horizontal->spec_irradiance[i], &senarr->diffuse_horizontal->spec_irradiance[i], &senarr->global_horizontal->spec_irradiance[i]);
        
        //copy wavelength data across structues
        senarr->direct_normal->wavelength[i] = senarr->diffuse_horizontal->wavelength[i];
        senarr->direct_horizontal->wavelength[i] = senarr->diffuse_horizontal->wavelength[i];
        senarr->global_horizontal->wavelength[i] = senarr->diffuse_horizontal->wavelength[i];
        
        //caculate irradiance
        senarr->direct_normal->irradiance += senarr->direct_normal->spec_irradiance[i];
        senarr->direct_horizontal->irradiance += senarr->direct_horizontal->spec_irradiance[i];
        senarr->diffuse_horizontal->irradiance += senarr->diffuse_horizontal->spec_irradiance[i];
        senarr->global_horizontal->irradiance += senarr->global_horizontal->spec_irradiance[i];
    }
    
    //normalise SPD
    normaliseSPDtoONE(senarr->direct_normal->spec_irradiance, senarr->direct_normal->spec_irrad_normalised, len);
    normaliseSPDtoONE(senarr->direct_horizontal->spec_irradiance, senarr->direct_horizontal->spec_irrad_normalised, len);
    normaliseSPDtoONE(senarr->diffuse_horizontal->spec_irradiance, senarr->diffuse_horizontal->spec_irrad_normalised, len);
    normaliseSPDtoONE(senarr->global_horizontal->spec_irradiance, senarr->global_horizontal->spec_irrad_normalised, len);
    
    //caculate colour temprature
    senarr->direct_normal->wav_interval = (senarr->direct_normal->wavelength[1] - senarr->direct_normal->wavelength[0]);
    senarr->direct_horizontal->wav_interval = (senarr->direct_normal->wavelength[1] - senarr->direct_normal->wavelength[0]);
    senarr->diffuse_horizontal->wav_interval = (senarr->direct_normal->wavelength[1] - senarr->direct_normal->wavelength[0]);
    senarr->global_horizontal->wav_interval = (senarr->direct_normal->wavelength[1] - senarr->direct_normal->wavelength[0]);
    senarr->direct_normal->colour_temp = colourtemp(senarr->direct_normal->wavelength[0], senarr->direct_normal->wavelength[len - 1], senarr->direct_normal->wav_interval, cmin, cmax, cstep, senarr->direct_normal, 1);
    senarr->direct_horizontal->colour_temp = colourtemp(senarr->direct_horizontal->wavelength[0], senarr->direct_horizontal->wavelength[len - 1], senarr->direct_horizontal->wav_interval, cmin, cmax, cstep, senarr->direct_horizontal, 1);
    senarr->diffuse_horizontal->colour_temp = colourtemp(senarr->diffuse_horizontal->wavelength[0], senarr->diffuse_horizontal->wavelength[len - 1], senarr->diffuse_horizontal->wav_interval, cmin, cmax, cstep, senarr->diffuse_horizontal, 1);
    senarr->global_horizontal->colour_temp = colourtemp(senarr->global_horizontal->wavelength[0], senarr->global_horizontal->wavelength[len - 1], senarr->global_horizontal->wav_interval, cmin, cmax, cstep, senarr->global_horizontal, 1);
    
    fclose(fp);
    return senarr;
}

//
//  int SMARTSsettup(struct time_data time, struct location_data location);
//
//  Generates the ./smarts295.inp.txt required by SMARTS to run
//
//  EXTERNAL REQUIREMENTS: none
//  INPUTS: - time_data and location_data structures with all values filled, see SMARTS documentation for list of valid values
//  OUTPUT: - returns 0 on scucess
//

int SMARTSsettup(struct time_data *time, struct location_data *location){
    FILE *fpheader = fopen("./data/SMARTS_295_Mac/smarts295.inp.txt", "w");
    if(fpheader == NULL){
        fprintf(stderr, "ERROR: cannot open ./data/SMARTS_295_Mac/smarts295.inp.txt\n");
        exit(EXIT_FAILURE);
    }
    
    //print location information to header file
    fprintf(fpheader, "GH_%s\n", location->name);                            //card 1
    fprintf(fpheader, "2\n");                                               //card 2
    fprintf(fpheader, "%.4f %.3f 0\n", location->latit, location->altit);     //card 2a
    fprintf(fpheader, "1\n");                                               //card 3
    fprintf(fpheader, "%s\n", location->atmos);                              //card 3a
    fprintf(fpheader, "1\n1\n1\n");                                         //card 4; card 5; card 6
    fprintf(fpheader, "%d\n", location->C02);                                //card 7
    fprintf(fpheader, "0\n");                                               //card 7a
    fprintf(fpheader, "%s\n", location->aerosol);                            //card 8
    fprintf(fpheader, "0\n");                                               //card 9
    fprintf(fpheader, "%f\n", location->aerodepth);                          //card 9a
    fprintf(fpheader, "%d\n", location->aldebo);                             //card 10
    fprintf(fpheader, "0\n360 830 1.0 1366.1\n3\n360 830 1\n1\n2\n0\n0\n-2\n0\n3\n"); //card 10b; card 11; card 12; card 12a; card 12b; card 12c; card 13; card 14; card 15; card 16; card 17
    fprintf(fpheader, "%04d %02d %02d %.2f %.4f %.4f %d", time->year, time->month, time->day, time->local_standard_hour24, location->latit, location->longit, location->timezone);

    fclose(fpheader);
    return 0;
}

//
//  int runSMARTS(int OS);
//
//  Runs the SMARTS program
//
//  EXTERNAL REQUIREMENTS: requies that the file ./data/yreturn.txt exist and the SMARTS_295_Mac program is installed to the same folder
//  INPUTS: - int representing system architecture. '0' is mac, '1' is linux
//  OUTPUT: - returns 0 on scucess
//  NOTE: Ensure that SMARTS is properly compiled for selected system
//

int runSMARTS(int OS){

    if (OS == 0){
        chdir("./data/SMARTS_295_Mac");             //change working directory in order to run SMARTS
    } else if (OS == 1){
        chdir("./data/SMARTS_295_Linux");
    } else {
        printf("system type %d not recognised", OS);
        return 1;
    }
    char *smarts1 = "./smarts295.ext.txt";
    char *smarts2 = "./smarts295.out.txt";
    //char *smarts3 = "./smarts295.inp.txt";
    remove(smarts1);                            //delete smarts295.ext.txt to allow SMARTS to run
    remove(smarts2);                            //delete smarts295.out.txt to allow SMARTS to run
    system("./smarts295 < ../yreturn.txt");     //run SMARTS program
    //remove(smarts3);                            //delete smarts295.inp.txt for clarity
    chdir("../..");                             //return to original working directory
    
    return 0;
}
