#ifndef AD_H
#define AD_H

#include <stdbool.h>


bool ad_adquirir(short **canala, short **canalb, unsigned int modo,
                 unsigned int kpc, unsigned int mpc);

#endif
