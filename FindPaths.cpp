/*
Jordan Francia
Assignment 5: Dijkstra

Main program to test graph class. Takes graph input file and source vertex (int) as command-line inputs. 
Allows user to display graph, change source vertex, or display path from user-input vertex to source, along with total path length.
*/

#include <iostream> 
#include <string>
#include <stdlib.h>
#include "Graph.hpp"

int main(int argc, char *argv[]){
    bool testMode = true;
    string filename;
    int start;
    
    if(!testMode){
    	if(argc != 3){
	   	   cout << "\nTo use this program, type ./FindPaths <graph file> <starting vertex (int)>" << "\n";
		   return 1;
	   }
	   filename = argv[1];
	   start = atoi(argv[2]);
    } else {
        filename = "Graph2.txt";
        start = 0;    
    }
    
    Graph alpha(filename, start);
    if(alpha.getSource() == -1)
	return 1;
    
    cout << "\n\n";
    cout << "Assignment 5: Dijkstra\n";
    cout << "-------------------------\n";
    cout << "Coded by Jordan Francia\n\n";
    
    bool running = true;
    int input, subInput;
    while(running){				//main loop
        input = 0;
        subInput = -1;
        cout << "Graph loaded with source vertex : " << start << "\n\n";
    	cout << "Choose Function:\n";
        cout << "1: Display graph\n";
    	cout << "2: Change source vertex\n";
    	cout << "3: Get shortest path\n";
    	cout << "4: Quit\n";
    	while(!input){
    		cout << "\nEnter 1 - 4: ";
    		cin >> input;
    		if(input < 1 || input > 4){
    		    cout << "\n\n" << "Invalid selection. Please try again." << "\n\n";
    		    input = 0;
    		    cin.clear();
    		    cin.ignore(1000, '\n');
    		} else {	
    			cout << "\n\n";
    			switch(input){
    				case 1:
                        alpha.displayGraph();
    					break;
    				case 2: 
    				    while(subInput == -1){
                    	   cout << "Enter new source vertex: ";
                    	   cin >> subInput;
                    	   if(subInput < 0 || subInput > alpha.getSize()-1){
                                cout << "\nInvalid input. Please enter 0 through " << alpha.getSize()-1 << "\n";
                                subInput = -1;         
                           }
                        }
                        start = subInput;
                        alpha.setSource(start);
                        break;
    				case 3: 
                         while(subInput == -1){
                    	   cout << "Enter destination vertex: ";
                    	   cin >> subInput;
                    	   if(subInput < 0 || subInput > alpha.getSize()-1){
                                cout << "\nInvalid input. Please enter 0 through " << alpha.getSize()-1 << "\n";
                                subInput = -1;         
                           }
                        }
                        alpha.displayShortestPath(subInput);
                        break;
                    case 4: 
    					running = false;
    					break;
    			}
    		}
    	}
    	input = 0;
    	cin.clear();
    	cin.ignore(1000, '\n');
    	cout << "\n\n";
    }

    cout << "\nGoodbye!\n";
    
    return 0; 
}

