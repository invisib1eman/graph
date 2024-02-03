import numpy as np
import random
import networkx as nx
import matplotlib.pyplot as plt
import os
from mpl_toolkits.mplot3d import Axes3D  # Import for 3D plotting
from collections import Counter
from itertools import combinations
import cProfile
def is_edge_cut(graph, edges_to_remove):
    # Create a copy of the graph
    temp_graph = graph.copy()
    
    # Remove the specified edges
    temp_graph.remove_edges_from(edges_to_remove)
    
    # Check if the graph is disconnected
    return not nx.is_connected(temp_graph)
def find_index(A):
    
    rate_list=[i["rate"] for i in A]
    total_rate=sum(rate_list)
    r=random.random()*total_rate
    prefix_sum=[0]*(len(A)+1)
    #Calculate the prefix sum array
    for i in range(len(rate_list)):
        prefix_sum[i+1]=prefix_sum[i]+rate_list[i]
    total_rate=prefix_sum[len(A)]
    left,right=1,len(prefix_sum)-1
    while left<=right:
        mid=left+(right-left)//2
        if prefix_sum[mid]<r:
            left=mid+1
        else:
            right=mid-1
    return left-1,total_rate

def lattice4id2id(lattice4id):
    return 2*lattice4id[0]+50*2*lattice4id[1]+50*50*2*lattice4id[2]+lattice4id[3]
def latticeid24id(latticeid):
    return([((latticeid//2)%50),(latticeid//100)%50,(latticeid//5000)%50,latticeid%2])
class growth_subgraph:
    def __init__(self,seed):
        self.g0=-15
        self.g1=25
        G=nx.Graph()
        for i in range(0,50):
            for j in range(0,50):
                for k in range(0,50):
                    for l in range(0,2):
                        G.add_node(lattice4id2id([i,j,k,l]),lattice4id=(i,j,k,l))

        edges=[]
        neighborlist=[[0,0,0,1],[0,0,-1,1],[-1,0,0,1],[-1,0,-1,1],[-1,1,0,1],[-1,1,-1,1]]
        for i in range(0,50):
            for j in range(0,50):
                for k in range(0,50):
                    for l in range(0,6):
                        i_n=i+neighborlist[l][0]
                        j_n=j+neighborlist[l][1]
                        k_n=k+neighborlist[l][2]
                        if (i_n>=0 and i_n<50 and j_n>=0 and j_n<50 and k_n>=0 and k_n<50):
                            

                            particle1_index=lattice4id2id([i,j,k,0])
                            particle2_index=lattice4id2id([i_n,j_n,k_n,1])
                            edges.append((particle1_index,particle2_index))
        G.add_edges_from(edges)
        self.G=G
        subgraph_nodes=seed
        subgraph=G.subgraph(subgraph_nodes)
        subgraph=nx.Graph(subgraph)
        self.subgraph=subgraph
        self.vertice=self.subgraph.number_of_nodes()
        self.edge=self.subgraph.number_of_edges()
        self.step=0
        self.time=0
        neighbors_list=[]
        neighbors_count={}
        for node in subgraph.nodes:
            neighbors=list(G[node])
            
            neighbors_list=neighbors_list+neighbors
        neighbors_list=[i for i in neighbors_list if i not in list(subgraph.nodes)]
       
        neighbors_count=Counter(neighbors_list)
        
         
        self.neighbors_count=neighbors_count
        
        self.nneighbors=len(self.neighbors_count)
        self.event_list=[]
        self.write_template()
        self.write_subgraph()
        
    def write_template(self):
        with open("sample_graph2.txt","w") as f:
            f.write(f"step time particle_number bond_number graph\n")
    def write_subgraph(self):
        with open("sample_graph2.txt","a") as f:
           
            f.write(f"{self.step} {self.time} {self.vertice} {self.edge} [")
            lattice4id_dic=nx.get_node_attributes(self.subgraph,"lattice4id")
            for node,l in lattice4id_dic.items():
                f.write(f"[{l[0]},{l[1]},{l[2]},{l[3]}],")
            f.write(f"]\n")
    def neighbor(self,edge):
        m1=edge[0]
        m2=edge[1]
        id1=self.G.nodes[m1].get("lattice4id")
        id2=self.G.nodes[m2].get("lattice4id")
        
        if id1[3]==1:
            exchange=lambda id1,id2: (id2,id1)
            id1,id2=exchange(id1,id2)
        
        neighbor14id=[id2[0],id2[1],-id2[2]+2*id1[2]-1,1]
        neighbor1id=lattice4id2id(neighbor14id)
        neighbor24id=[id1[0],id1[1],-id1[2]+2*id2[2]+1,0]
        neighbor2id=lattice4id2id(neighbor24id)
        neighbor1_edge=(lattice4id2id(id1),neighbor1id)
        neighbor2_edge=(neighbor2id,lattice4id2id(id2))
        return neighbor1_edge,neighbor2_edge
    def is_neighbor(self,edge1,edge2):
        neighbor1,neighbor2=self.neighbor(edge1)
        if set(neighbor1)==set(edge2) or set(neighbor2)==set(edge2):
            return True
        else:
            return False
    def add_node(self,i):
        origin_nodes=set(self.subgraph.nodes)
        origin_nodes.add(i)
        subgraph=self.G.subgraph(origin_nodes)
        subgraph=nx.Graph(subgraph)
        self.subgraph=subgraph
        self.vertice=self.subgraph.number_of_nodes()
        self.edge=self.subgraph.number_of_edges()
        neighbors_list=[]
        for node in subgraph.nodes:
            neighbors=list(self.G[node])
            
            neighbors_list=neighbors_list+neighbors
        neighbors_list=[i for i in neighbors_list if i not in list(self.subgraph.nodes)]
        neighbors_count=Counter(neighbors_list)
        self.neighbors_count=neighbors_count
        self.nneighbors=len(self.neighbors_count)
    def remove_nodes(self,cut):
        temp_graph=self.subgraph.copy()
        temp_graph.remove_edges_from(cut)
        components=list(nx.connected_components(temp_graph))
        largest_component=max(components,key=lambda d:len(d))
        subgraph=self.G.subgraph(largest_component)
        self.subgraph=nx.Graph(subgraph)
        self.vertice=self.subgraph.number_of_nodes()
        self.edge=self.subgraph.number_of_edges()
        neighbors_list=[]
        for node in subgraph.nodes:
            neighbors=list(self.G[node])
            
            neighbors_list=neighbors_list+neighbors
        neighbors_list=[i for i in neighbors_list if i not in list(self.subgraph.nodes)]
        neighbors_count=Counter(neighbors_list)
        self.neighbors_count=neighbors_count
        self.nneighbors=len(self.neighbors_count)
    def create_add_event(self):
        for keys,value in self.neighbors_count.items():
            if value>1:
                self.event_list.append({"type":"add","rate":0.25**value,"parameters":keys})
    def create_remove_event(self):
        gdict=dict(self.subgraph.degree())
        nodes_list=[k for k,v in gdict.items() if v==2]
        for node in nodes_list:
            edges_connected_to_node=self.subgraph.edges(node)
            if self.is_neighbor:
                free_bonds=6
            else:
                free_bonds=4
            self.event_list.append({"type":"remove","rate":0.25**2*np.exp(self.g0*2+self.g1+free_bonds),"parameters":edges_connected_to_node})

    def execute_event(self,event):
        if event["type"]=='add':
            self.add_node(event["parameters"])
        if event["type"]=='remove':
            self.remove_nodes(event["parameters"])
    def sample(self):
        self.event_list=[]
        self.create_add_event()
        self.create_remove_event()
        
        event_chosen,total_rate=find_index(self.event_list)
        self.execute_event(self.event_list[event_chosen])
        self.write_subgraph()
        self.step+=1
        self.time+=1/total_rate
def main():
    directory_name="free_dif_8"
    if not os.path.exists(directory_name):
        os.mkdir(directory_name)
    os.chdir(directory_name)
    #construct seed particles
    layer_4id=[[25,25,25,0],[25,25,25,1],[26,25,25,0],[25,26,25,1],[25,26,25,0],[24,26,25,1]]
    seed=[]
    #6 layers
    for i in range(0,6):
        for atom in layer_4id:
            seed.append(lattice4id2id([atom[0],atom[1],atom[2]+i,atom[3]]))
    sample=growth_subgraph(seed)
    for i in range(10000):
        sample.sample()
if __name__=='__main__':
    cProfile.run('main()')

