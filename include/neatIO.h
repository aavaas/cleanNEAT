//
// Created by avs on 12/8/17.
//

#ifndef CLEANNEAT_NEATIO_H_H
#define CLEANNEAT_NEATIO_H_H

#include <network.h>
#include <string>

void exportNetworkToFile(std::string filename, NEAT::Network *net);

void saveNewGenome();

#endif //CLEANNEAT_NEATIO_H_H
