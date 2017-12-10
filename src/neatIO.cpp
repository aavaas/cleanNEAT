//
// Created by avs on 12/8/17.
//

#include "neatIO.h"
#include "genome.h"

using namespace NEAT;


struct Conn {
    int from;
    int to;
    double weight;
};

bool hasLink(Link *link, const std::vector<Conn> &vec) {
    for (auto con: vec) {
        if (link->in_node->node_id == con.from && link->out_node->node_id == con.to) {
            return true;
        }
    }
    return false;
}


void exportNetworkToFile(std::string filename, Network *net) {
    std::ofstream outFile(filename);

    std::vector<NNode *>::iterator curnode;

    std::vector<Conn> links;

    outFile << net->all_nodes.size() << std::endl;

    for (auto nodes: net->all_nodes) {
        outFile << nodes->node_id << " " << (nodes->gen_node_label == 3 ? 1 : nodes->gen_node_label) << std::endl;

        for (auto inLinks : nodes->incoming) {
            if (!hasLink(inLinks, links)) {
                links.push_back(Conn{inLinks->in_node->node_id, inLinks->out_node->node_id, inLinks->weight});
            }
        }
        for (auto outLinks : nodes->incoming) {
            if (!hasLink(outLinks, links)) {
                links.push_back(Conn{outLinks->in_node->node_id, outLinks->out_node->node_id, outLinks->weight});
            }
        }
    }

    outFile << links.size() << std::endl;
    for (auto lnk: links) {
        outFile << lnk.from << " " << lnk.to << " " << lnk.weight << std::endl;
    }
    outFile.close();
}

void saveNewGenome() {
    Genome* start_genome = new Genome(16, 2, 1, 0);
    start_genome->print_to_filename("testgenome");
}