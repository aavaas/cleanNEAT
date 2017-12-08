//
// Created by avs on 12/8/17.
//

#ifndef CLEANNEAT_AVSEXPERIMENT_H_H
#define CLEANNEAT_AVSEXPERIMENT_H_H

#include "population.h"
#include "organism.h"
#include "network.h"
#include <utility>

NEAT::Population *fe_test(int gens);
bool fe_evaluate(NEAT::Organism *org);
int fe_epoch(NEAT::Population *pop,int generation,char *filename, int &winnernum, int &winnergenes,int &winnernodes);
std::pair<bool,int> simulategame(NEAT::Network*);

#endif //CLEANNEAT_AVSEXPERIMENT_H_H
