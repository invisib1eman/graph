#include "mc.h"

void MC::Initialize()
{
   Nv=1;
   Graph g(Nv);
   g.Nv=Nv;
   g.V.push_back(Latticeindex(0,0,0,0));
   g.Ne=0;
   new_G.push_back(g);
   cout<<Nv<<endl;
   Write_Graph();
   cout<<Nv<<endl;
}
void MC::Sweep()
{
    cout<<Nv<<endl;
    for(int n=0;n<Nsweep;n++)
    {
        Nv=Nv+1;
        cout<<Nv<<endl;
        old_G=new_G;
        new_G.clear();
        for(auto it=old_G.begin();it!=old_G.end();it++)
        {
            
            Graph g=*it;
            vector<Neighbor> neighbor;
            neighbor=g.find_neighbor();
            for(auto iit=neighbor.begin();iit!=neighbor.end();iit++)
            {
                Neighbor n=*iit;
                Graph newgraph=g;
                newgraph.addVertex();
                newgraph.V.push_back(n.node);
                for(auto iiit=n.neighbors.begin();iiit!=n.neighbors.end();iiit++)
                {
                    int vertice=*iiit;
                    cout<<Nv-1<<"\t"<<vertice<<endl;
                    newgraph.addEdge(Nv-1,vertice);
                }
                new_G.push_back(newgraph);
            }
        
        }
        Write_Graph();
    }

    
    
}
void MC::Write_Graph()
{
    ofstream out;
    out.open("Graphs.txt",ios::app);
    cout<<Nv<<endl;
    out<<"Nv="<<setw(4)<<Nv<<"\n";
    cout<<Nv<<endl;
    for(auto it=new_G.begin();it!=new_G.end();it++)
    {
        Graph g=*it;
        out<<"Ne="<<setw(4)<<g.Ne<<"\n"; 
        out<<"Vertices"<<"\n";
        for(int i=0;i<g.Nv;i++)
        {
            out<<setw(4)<<g.V[i].n1<<setw(4)<<g.V[i].n2<<setw(4)<<g.V[i].n3<<setw(4)<<g.V[i].n4<<"\n";
        }
    }   
}

