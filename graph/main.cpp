#include "header.h"
#include "lattice.h"
#include "mc.h"

int main(int argc, char *argv[])
{
    
    
	time_t start, end;
	time(&start);
	Lattice lattice;
    lattice.Ng=10;
    lattice.Nbasis=2;
    lattice.BasisPoints.push_back(XYZ(0,0,0));
    lattice.BasisPoints.push_back(XYZ(2.26,0,1.06));
    lattice.LatticeVectors.push_back(XYZ(0,2.26*sqrt(3),0));
    lattice.LatticeVectors.push_back(XYZ(2.26*1.5,2.26*sqrt(3)/2,0));
    lattice.LatticeVectors.push_back(XYZ(0,0,2.12));
    lattice.Create_lattice();
    ofstream outFile("lattice_data.csv");
    for(auto it=lattice.latticePoints.begin();it!=lattice.latticePoints.end();it++)
    {
        Latticeindex j=*it;
        XYZ position=lattice.index_to_xyz(j);
        outFile<<position.x<<","<<position.y<<","<<position.z<<"\n";
    }
    outFile.close();
    MC mc;
    mc.Nsweep=5;
    mc.Initialize();
    mc.Sweep();
    return 0;
    
    /*
   XYZ a=quarterrotation(XYZ(1.0,0.0,-0.47),quarternion(0.899125,0.28284,0.290824,-0.164306));
   cout<<a.x<<endl;
   cout<<a.y<<endl;
   cout<<a.z<<endl;
   return 0;*/
}