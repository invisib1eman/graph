//NANOROD: mc.h MC Class (Revision Date: Oct 27, 2023)
#ifndef _GRAPH_H
#define _GRAPH_H
#include "lattice.h"
class Neighbor{
    public:
    Latticeindex node;
    vector<int> neighbors;
    Neighbor(Latticeindex _node,vector<int> _neighbors){node=_node;neighbors=_neighbors;}
};
class Graph
{
    private:
    
    void DFS(int node, std::vector<bool>& visited, vector<vector<int> >& component) {
        visited[node] = true;
        component.push_back(adjList[node]); 
        for (int neighbour : adjList[node]) {
            if (!visited[neighbour]) {
                DFS(neighbour, visited, component);
            }
        }
    }
    public:
    vector<Latticeindex> Neighbora;
    vector<Latticeindex> Neighborb;
    int Nv;//number of vertices
    int Ne;//number of edges
    vector<Latticeindex> V;//vertices
    vector<int> keys;//int index of vertices
    vector<vector<int> > adjList;//edges
    Graph(int Nv): Nv(Nv), adjList(Nv)
    {
        Neighbora.push_back(Latticeindex(0,0,0,1));
        Neighbora.push_back(Latticeindex(0,0,-1,1));
        Neighbora.push_back(Latticeindex(-1,0,0,1));
        Neighbora.push_back(Latticeindex(-1,0,-1,1));
        Neighbora.push_back(Latticeindex(0,-1,0,1));
        Neighbora.push_back(Latticeindex(0,-1,-1,1));
        for(auto it=Neighbora.begin();it!=Neighbora.end();it++)
        {
            Latticeindex j=*it;
            Neighborb.push_back(Latticeindex(0,0,0,0)-j);
        }   
    }
    void addVertex()
    {
        adjList.push_back(vector<int>());
        Nv++;
    }
    void Init()
    {
        for(auto it=V.begin();it!=V.end();it++)
        {
            Latticeindex node=*it;
            keys.push_back(node.index);
        }
    }
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u); // Remove this line for a directed graph
        Ne+=1;
    }

    void removeEdge(int u, int v) {
        adjList[u].erase(remove(adjList[u].begin(), adjList[u].end(), v), adjList[u].end());
        adjList[v].erase(remove(adjList[v].begin(), adjList[v].end(), u), adjList[v].end());
        Ne-=1;
    }
   
        
    
    vector<Neighbor> find_neighbor()
    {
        vector<Neighbor> neighbor;
        vector<int> neighbornodes;
        for(int i=0;i<Nv;i++)
        {
            Latticeindex node=V[i];
            
            if(node.n4==0)
            {
                for(int j=0;j<6;j++)
                {
                    Latticeindex n=node+Neighbora[j];
                    if(find(keys.begin(),keys.end(),node.index)==keys.end())
                    {
                        auto it=find(neighbornodes.begin(),neighbornodes.end(),n.index);
                        if(it==neighbornodes.end())
                        {
                            neighbornodes.push_back(n.index);
                            vector<int> new_neighbor;
                            new_neighbor.push_back(i);
                            neighbor.push_back(Neighbor(n,new_neighbor));
                        }
                        else
                        {
                            int index=distance(neighbornodes.begin(),it);
                            neighbor[index].neighbors.push_back(i);
                        }
                        
                    }
                }
            }
            else
            {
                for(int j=0;j<6;j++)
                {
                    Latticeindex n=node+Neighborb[j];
                    if(find(keys.begin(),keys.end(),node.index)==keys.end())
                    {
                        auto it=find(neighbornodes.begin(),neighbornodes.end(),n.index);
                        if(it==neighbornodes.end())
                        {
                            neighbornodes.push_back(n.index);
                            vector<int> new_neighbor;
                            new_neighbor.push_back(i);
                            neighbor.push_back(Neighbor(n,new_neighbor));
                        }
                        else
                        {
                            int index=distance(neighbornodes.begin(),it);
                            neighbor[index].neighbors.push_back(i);
                        }
                        
                    }
                }
            }               
        }
        return neighbor;
    }
    vector<vector<vector<int> > > getConnectedComponents() {
        vector<bool> visited(Nv, false);
        vector<vector<vector<int> > > connectedComponents;

        for (int i = 0; i < Nv; ++i) {
            if (!visited[i]) {
                vector<vector<int> > component;
                DFS(i, visited, component);
                connectedComponents.push_back(component);
            }
        }

        return connectedComponents;
    }
    

};
#endif

