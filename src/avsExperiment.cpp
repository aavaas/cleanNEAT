//
// Created by avs on 12/8/17.
//

#include <avsExperiment.h>

#include <iostream>
#include <sstream>
#include <neatIO.h>

//#define NO_SCREEN_OUT

using namespace std;

using namespace NEAT;

Population *fe_test(int gens) {
    Population *pop = 0;
    Genome *start_genome;
    char curword[20];
    int id;

    ostringstream *fnamebuf;
    int gen;

    int evals[NEAT::num_runs];  //Hold records for each run
    int genes[NEAT::num_runs];
    int nodes[NEAT::num_runs];
    int winnernum;
    int winnergenes;
    int winnernodes;
    //For averaging
    int totalevals = 0;
    int totalgenes = 0;
    int totalnodes = 0;
    int expcount;
    int samples;  //For averaging

    memset(evals, 0, NEAT::num_runs * sizeof(int));
    memset(genes, 0, NEAT::num_runs * sizeof(int));
    memset(nodes, 0, NEAT::num_runs * sizeof(int));

    ifstream iFile("avsneatstartgenes", ios::in);

    cout << "START" << endl;

    cout << "Reading in the start genome" << endl;
    //Read in the start Genome
    iFile >> curword;
    iFile >> id;
    cout << "Reading in Genome id " << id << endl;
//    start_genome=new Genome(id,iFile);
    start_genome = new Genome(16, 2, 1, 0);
    iFile.close();

    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        //Spawn the Population
        cout << "Spawning Population off Genome2" << endl;

        pop = new Population(start_genome, NEAT::pop_size);

        cout << "Verifying Spawned Pop" << endl;
        pop->verify();

        for (gen = 1; gen <= gens; gen++) {
            cout << "Epoch " << gen << endl;

            //This is how to make a custom filename
            fnamebuf = new ostringstream();
            (*fnamebuf) << "gen_" << gen << ends;  //needs end marker

#ifndef NO_SCREEN_OUT
            cout << "name of fname: " << fnamebuf->str() << endl;
#endif

            char temp[50];
            sprintf(temp, "gen_%d", gen);

            //Check for success
            if (fe_epoch(pop, gen, temp, winnernum, winnergenes, winnernodes)) {
                //Collect Stats on end of experiment
                evals[expcount] = NEAT::pop_size * (gen - 1) + winnernum;
                genes[expcount] = winnergenes;
                nodes[expcount] = winnernodes;
                gen = gens;

            }

            //Clear output filename
            fnamebuf->clear();
            delete fnamebuf;

        }

        if (expcount < NEAT::num_runs - 1) delete pop;

    }

    //Average and print stats
    cout << "Nodes: " << endl;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << nodes[expcount] << endl;
        totalnodes += nodes[expcount];
    }

    cout << "Genes: " << endl;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << genes[expcount] << endl;
        totalgenes += genes[expcount];
    }

    cout << "Evals " << endl;
    samples = 0;
    for (expcount = 0; expcount < NEAT::num_runs; expcount++) {
        cout << evals[expcount] << endl;
        if (evals[expcount] > 0) {
            totalevals += evals[expcount];
            samples++;
        }
    }

    cout << "Failures: " << (NEAT::num_runs - samples) << " out of " << NEAT::num_runs << " runs" << endl;
    cout << "Average Nodes: " << (samples > 0 ? (double) totalnodes / samples : 0) << endl;
    cout << "Average Genes: " << (samples > 0 ? (double) totalgenes / samples : 0) << endl;
    cout << "Average Evals: " << (samples > 0 ? (double) totalevals / samples : 0) << endl;

    return pop;

}




int fe_epoch(Population *pop, int generation, char *filename, int &winnernum, int &winnergenes, int &winnernodes) {
    vector<Organism *>::iterator curorg;
    vector<Species *>::iterator curspecies;

    bool win = false;


    //Evaluate each organism on a test
    for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
        if (fe_evaluate(*curorg)) {
            win = true;
            winnernum = (*curorg)->gnome->genome_id;
            winnergenes = (*curorg)->gnome->extrons();
            winnernodes = ((*curorg)->gnome->nodes).size();
            if (winnernodes == 5) {
                //You could dump out optimal genomes here if desired
                //(*curorg)->gnome->print_to_filename("xor_optimal");
                //cout<<"DUMPED OPTIMAL"<<endl;
            }
        }
    }

    //Average and max their fitnesses for dumping to file and snapshot
    for (curspecies = (pop->species).begin(); curspecies != (pop->species).end(); ++curspecies) {

        //This experiment control routine issues commands to collect ave
        //and max fitness, as opposed to having the snapshot do it,
        //because this allows flexibility in terms of what time
        //to observe fitnesses at

        (*curspecies)->compute_average_fitness();
        (*curspecies)->compute_max_fitness();
    }

    //Take a snapshot of the population, so that it can be
    //visualized later on
    //if ((generation%1)==0)
    //  pop->snapshot();

    //Only print to file every print_every generations
    if (win ||
        ((generation % (NEAT::print_every)) == 0))
        pop->print_to_file_by_species(filename);


    if (win) {
        for (curorg = (pop->organisms).begin(); curorg != (pop->organisms).end(); ++curorg) {
            if ((*curorg)->winner) {
                cout << "WINNER IS #" << ((*curorg)->gnome)->genome_id << endl;
                //Prints the winner to file
                //IMPORTANT: This causes generational file output!
                print_Genome_tofile((*curorg)->gnome, "FE_winner");
            }
        }

    }

    pop->epoch(generation);

    if (win) return 1;
    else return 0;

}

bool fe_evaluate(Organism *org) {

    bool success;  //Check for successful run
    int maxfitness = 3020 * 4;

    std::pair<bool, int> out = simulategame(org->net);

    if (out.first) { //OK
        org->fitness = out.second;
        org->error = maxfitness - out.second;
    } else {
        //somethings wrong (shouldnt happen)
        org->fitness = 0.001;
        org->error = maxfitness;
    }
    cout << "Org " << (org->gnome)->genome_id << "Total fitness: " << org->fitness << endl;

    if (out.second >= maxfitness) {
        org->winner = true;
        return true;
    } else {
        org->winner = false;
        return false;
    }

}



void exportGameParamsToFile(const std::string filename, int pos) {
    std::ofstream outFile(filename);
    outFile << "3 2 2000 0 0 "<< pos << std::endl;
    outFile.close();
}

std::pair<bool, int> simulategame(Network *net) {
    string path = "/home/daniel/CS776/FEGA/FEGA/";
    exportNetworkToFile(path + "networkconf", net);

    bool success = true;
    int totalfitness = 0;

    for (int i = 0; i<8;i=i+2){
        //positions 0,2,4 and 6
        exportGameParamsToFile(path + "gameparams", i);
        system((path + "sim.sh").c_str());
        ifstream infile(path + "outfitness", ios::in);
        std::string line;
        int a;
        if (!std::getline(infile, line)) {
            success = false;
            std::cout << "problem while reading fitness" << std::endl;
        }
        std::istringstream iss(line);
        iss >> a;
        std::cout << "infile fitness: " << a << std::endl;
        totalfitness += a;
        infile.close();
    }


    return {success, totalfitness};
}
