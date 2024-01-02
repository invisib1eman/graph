//Define lattice
#ifndef _LATTICE_H
#define _LATTICE_H
#include "header.h"
#include "xyz.h"
class Latticeindex
{

public:
    int n1,n2,n3,n4;
    int L=10;
    int index;
    void set(int _n1, int _n2, int _n3, int _n4){n1=_n1;        n2=_n2;   n3=_n3;n4=_n4;index=2*(L*n1+L*L*n2+L*L*L*n3+n4);}
    Latticeindex(int _n1, int _n2, int _n3, int _n4){n1=_n1;        n2=_n2;   n3=_n3;;n4=_n4;index=2*(L*n1+L*L*n2+L*L*L*n3+n4);}
    Latticeindex(){n1=0;      n2=0;  	n3=0;n4=0;index=2*(L*n1+L*L*n2+L*L*L*n3+n4);}
    Latticeindex operator + (Latticeindex& other){return Latticeindex(n1+other.n1,n2+other.n2,n3+other.n3,n4+other.n4);}
    Latticeindex operator - (Latticeindex& other){return Latticeindex(n1-other.n1,n2-other.n2,n3-other.n3,n4+other.n4);}
    
};
class Lattice
{
public:
    vector<Latticeindex> latticePoints;
    //vector<XYZ> BasisPoints;
    //vector<XYZ> LatticeVectors;
    int Nbasis;//number of basis points in one unit lattice
    int Ng;//number of lattices in one direction
    void Create_lattice()
    {
        for (int i=-Ng/2;i<Ng/2;i++)
        {
            for (int j=-Ng/2;j<Ng/2;j++)
            {
                for (int k=-Ng/2;k<Ng/2;k++)
                {
                    for (int l=0;l<Nbasis;l++)
                    {
                        latticePoints.push_back(Latticeindex(i,j,k,l));
                    }
                }
            }
        }
    }
    /*XYZ index_to_xyz(Latticeindex m)
    {
        return XYZ(LatticeVectors[0].x*m.n1+LatticeVectors[1].x*m.n2+LatticeVectors[2].x*m.n3+BasisPoints[m.n4].x,LatticeVectors[0].y*m.n1+LatticeVectors[1].y*m.n2+LatticeVectors[2].y*m.n3+BasisPoints[m.n4].y,LatticeVectors[0].z*m.n1+LatticeVectors[1].z*m.n2+LatticeVectors[2].z*m.n3+BasisPoints[m.n4].z);
    }*/
    
    
};
#endif
