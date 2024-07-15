#ifndef vtuneConfiguration_H
#define vtuneConfiguration_H

#define VTUNE_ANALYSIS

#ifdef VTUNE_ANALYSIS
  #include <ittnotify.h>
#endif

#define NO_IO

//Mapping ROI value to the region it profiles
//0->All
//1->just alignment 
//2->just extension
#ifdef VTUNE_ANALYSIS
  //#define ROI 0
  #define ROI 1
  //#define ROI 2
#endif

#endif
