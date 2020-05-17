# SMARTS295
C tool for running the SMARTS daylight simulation program
Libary contains C functions for driving SMARTS from your own C program

SMARTS is fully documented here: https://www.nrel.gov/grid/solar-resource/smarts.html
Register to download it. Note it is free for non-commercial use only.

"Developed by Dr. Christian Gueymard, SMARTS computes clear sky spectral irradiances (including direct beam, circumsolar, hemispherical diffuse, and total on a tilted or horizontal receiver plane) for specified atmospheric conditions. SMARTS users can specify conditions from any of 10 standard atmospheres or their own data. Users can also specify output for one or many points in time or solar geometries. SMARTS 2.9.2 is the basis for American Society of Testing and Materials (ASTM) reference spectra (ASTM G-173 and ASTM G-177) used for photovoltaic performance testing and materials degradation studies."

For more detailed simulation, see MODTRAN: http://modtran.spectral.com/

*********************************

##Usage

requies that the file ./data/yreturn.txt exist and the SMARTS_295_Mac program is installed to the same folder

*********************************

##Functions

- SMARTSsettup - Generates the ./smarts295.inp.txt required by SMARTS to run Runs the SMARTS program

- runSMARTS - Runs the SMARTS program

- readSMARTS - Opens and reads ./smarts295.out.txt, putting data into an ALLsensor structure
