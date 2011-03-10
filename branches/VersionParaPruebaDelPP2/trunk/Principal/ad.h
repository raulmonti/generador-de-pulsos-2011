#ifndef AD_H
#define AD_H

#include "stdbool.h"


#define AD_MODO_MODULADO 0
#define AD_MODO_CONTINUO 1


bool ad_config (unsigned int mps, unsigned int kpc, unsigned int adqm);

unsigned int ad_adquirir(unsigned int kpc, unsigned int adqm);

#endif

