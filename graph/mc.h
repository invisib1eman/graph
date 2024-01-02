#ifndef _MC_H
#define _MC_H
#include "lattice.h"
#include "graph.h"
class MC
{
    public:
        vector<Graph> new_G;
        vector<Graph> old_G;
        int Nv;
        int Nsweep;
        double free_energy;
        double bond_energy;
        double density;
        double rotational_entropy;
        double dihedral_entropy;
        int N_neighbor=6;
        MC(){}
        void Initialize();
        void WriteTemplate();
        void LogProfile(int ,double );
        void Sweep();
        void Write_Graph();

};
#endif
