/*
Jordan Francia
Assignment 5: Dijkstra

Graph data structure to hold vertexes (represented at integers), connected by weighted edges (represented as floats)

CONSTRUCTION: a filename of a file containing graph data, formatted as specified by the Assignment 5 sheet, and a starting vertex (int). 

******************PUBLIC OPERATIONS*********************
void setSource(int id)                              --> sets source vertex and runs Dijkstra
int getSource()					   --> returns source vertex
bool addVertex(int id)                              --> adds a new vertex. Returns false if already exists
bool addAdjacent(int id, int adj, float weight)     --> adds vertex adj to adjacency list of vertex id with weight. Returns false if adj already exists
bool contains(int id)                               --> returns true if graph contains vertex with given id
int getSize()                                       --> returns size of graph
void displayShortestPath(int finish)                --> displays shortest path from finish to source, along with path length
void displayGraph()                                 --> displays all vertices and adjacency lists
void clearGraph()                                   --> empties graph
*/

#ifndef GRAPH_H
#define GRAPH_H
#include "BinaryHeap.h"
#include <stdlib.h>
#include <vector> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

const float NON_EDGE = -999;
const float INF = 9999;

class Graph{
    
    public: 
        
        Graph(string filename, int source);
        void setSource(int id);
	 int getSource();
        bool addVertex(int id);
        bool addAdjacent(int id, int adj, float weight);
        bool contains(int id);
        int getSize();
        void displayShortestPath(int finish);
        void displayGraph();
        void clearGraph();

    private:
        struct Vertex{
                int id;
                vector<float> adjacent;
                float dist;
                bool known;
                Vertex * path; 

                Vertex(int size, int i) : id(i), known(false), dist(INF), path(0){
                    for(int n = 0; n < size; n++)
                        adjacent.push_back(NON_EDGE);   
                }
        };
        int size,
            dSource;
        vector<Vertex> vertices;
        HashTable<int> table;
        
        void setVertex (Vertex & v, int size, int i);
        float printPath(Vertex v, float length);
        Vertex * getVertex(int id);
        float dijkstra(int source);

};

int Graph::getSize(){
    return size;    
}

void Graph::setSource(int id){
    dSource = id;
    dijkstra(dSource);
}

int Graph::getSource(){
    return dSource;
}

void Graph::displayShortestPath(int finish){
    float totalPath = 0;
    if(finish == dSource)
       cout << "Path: " << finish << "\n";     
    else{
        cout << "Path: ";
        totalPath = printPath(vertices[finish], 0);
        if(!totalPath)
            cout << " has no path from " << dSource << "\n";
    }
    cout <<"\nTotal Length: " << totalPath << "\n";
}

float Graph::printPath(Graph::Vertex v, float length){
    if(v.path){
        length = printPath(*(v.path), length + v.path->adjacent[v.id]);
        cout << " --> ";
    }    
    cout << v.id;
    return length;
}

float Graph::dijkstra(int source){
        Vertex * thisVert, 
                * thisAdj;
        BinaryHeap<int> heap;
        float distance;
        
        for(int n = 0; n < size; n++){
            vertices[n].dist = INF;
            vertices[n].known = false;
            vertices[n].path = 0;    
        }
        
        vertices[source].dist = 0;               
        heap.insert(source, vertices[source].dist);
       
        
        while(!heap.isEmpty()){
            thisVert = &vertices[heap.deleteMin()];
             
            thisVert->known = true; 
            for(int k = 0; k < size; k++){
                if(thisVert->adjacent[k] != NON_EDGE){
                    thisAdj = &vertices[k];
                    distance = thisVert->adjacent[k] + thisVert->dist;
                    if(distance < thisAdj->dist){
                        thisAdj->dist = distance;
                        thisAdj->path = thisVert;
                        if(!thisAdj->known)
                            heap.insert(k,thisAdj->dist);
                    }
                }    
            }   
        }
        return distance;
}

Graph::Graph(string fileName, int source){
    dSource = source;
    std::ifstream istream(fileName.c_str());    
    if(!istream.is_open()){
        cout << "\nERROR: file \"" << fileName << "\" not found. Cannot load graph.\n";  
        dSource = -1;   
        return;
    }
    string thisLine, temp;
    Vertex * v;
    stringstream * ss;
    vector<string> inputs;
    int id, adj;
    float weight;
    
    getline(istream,thisLine); 
    size = atoi(thisLine.c_str());      //get size
    for(int n = 0; !istream.eof() && n < size; n++){
        getline(istream,thisLine);
        ss = new stringstream(thisLine);
        while(ss->good()){
            *ss >> temp;
            inputs.push_back(temp);    
        }
        id = atoi(inputs[0].c_str());
        v = new Vertex(size,id); // Vertex ID
        vertices.push_back(*v);
        for(int k = 1; k < inputs.size(); k++){
            if(k%2)
                adj = atoi(inputs[k].c_str());
            else{
                weight = atof(inputs[k].c_str());
                addAdjacent(id, adj, weight);    
            }
        }
        inputs.clear();
        delete v;
        delete ss;
    }
    istream.close();
    setSource(source);
}

bool Graph::addVertex(int id){
    if(contains(id))
        return false;
    Vertex v(size, id);
    vertices.push_back(v);
    return true;
}

bool Graph::addAdjacent(int id, int adj, float weight){
    if(!contains(id) || adj < 0 || adj > size)
        return false;
    Vertex * v = getVertex(id);
    v->adjacent[adj] = weight;
    return true;
}

Graph::Vertex * Graph::getVertex(int id){
    Vertex * result;
    for(int n = 0; n < vertices.size(); n++){
        if(vertices[n].id == id){
            result = &vertices[n]; 
            return result;  
        }  
    }    
    cout << "\nERROR: getVertex called on non-existant vertex with id " << id << "\n";
}

bool Graph::contains(int id){
    for(int n = 0; n < vertices.size(); n++){
        if(vertices[n].id == id)
            return true;    
    }
    return false;
}

void Graph::displayGraph(){
    float weight;
    for(int n = 0; n < size; n++){
        for(int k = 0; k < size; k++){
            weight = vertices[n].adjacent[k];
            if(weight != NON_EDGE)
                cout << n << " --(" << weight << ")--> " << k << "\n";
        }        
        cout << "\n";
    }
}

void Graph::clearGraph(){
    size = 0;
    vertices.clear();    
}

#endif
