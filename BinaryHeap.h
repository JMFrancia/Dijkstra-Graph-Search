/*
Assignment 5: Dijkstra 
Jordan Francia

Binary Heap class from Data Structures and Analysis in C++ by Mark Allen Weiss,
modified to use a hash table in order to have the operations increasekey(comparable, int), 
decreasekey(comparable, int), and remove(x) in O(logn) time. 

 CONSTRUCTION: with an optional capacity (that defaults to 100)

 ******************PUBLIC OPERATIONS*********************
 bool insert( x, priority )       --> Insert x with priority. Return false if x already existed
 bool increaseKey(x, delta) --> Increases priority of x by delta. Returns false if x doesn't exist in heap
 bool decreaseKey(x, delta) --> Decreases priority of x by delta. Returns false if x doesn't exist in heap
 bool remove(x)          --> removes x from heap. Returns false if x doesn't exist in heap
 bool contains(x)        --> returns true if x exists in heap
 deleteMin( minItem )   --> Remove (and optionally return) smallest item
 Comparable findMin( )  --> Return smallest item
 bool isEmpty( )        --> Return true if empty; else false
 void makeEmpty( )      --> Remove all items
*/

#ifndef BINARY_HEAP_H
#define BINARY_HEAP_H

#include "QuadraticProbing.h"
#include "TableElement.h"
#include <vector>
#include <iostream>

using namespace std;


template <typename Comparable>
class BinaryHeap
{
  public:
    explicit BinaryHeap( int capacity = 100 ) : array( capacity + 1 ), currentSize( 0 )
    {}

    explicit BinaryHeap( const vector<Comparable> & items )
      : array( items.size( ) + 10 ), currentSize( items.size( ) )
    {
        for( int i = 0; i < items.size( ); i++ )
            array[ i + 1 ] = items[ i ];
        buildHeap( );
    }

    bool isEmpty( ) const
      { return currentSize == 0; }

    /**
     * Find the smallest item in the priority queue.
     * Return the smallest item, or throw Underflow if empty.
     */
    const Comparable & findMin( ) const
    {
        if( isEmpty( ) )
              cout << "\nERROR: findMin() called on empty heap\n";
        return array[ 1 ];
    }
    /**
     * Insert item x, allowing duplicates.
     */
    bool insert( const Comparable & x, float priority)
    {
        
        if(contains(x))                                 //Prevents duplicate elements
            return false;
     
        if( currentSize == array.size( ) - 1 )          //Resize array if necessary
            array.resize( array.size( ) * 2 );
     
        HeapElement newElement(x, priority);
               
        int hole = ++currentSize;                           //hole = last position
        array[hole] = newElement;
        percolateUp(hole);
        return true;
    }

    bool contains(const Comparable & x){
        return (table.contains(TableElement<Comparable>(x,-1)));    
    }
    
    bool increaseKey(Comparable & x, float delta){
         if(!contains(x))
            return false;
         int pos = (table.get(x)).index;
         array[pos].priority += delta;      
         percolateDown(pos);
         return true;
    }

   bool decreaseKey(Comparable & x, float delta){
        if(!contains(x))
            return false;
        int pos = (table.get(x)).index;
        tableRemove(x);
        array[pos].priority -= delta;
        percolateUp(pos);
        return true;
    }
    
    bool remove(Comparable & x){
        if(!contains(x))
            return false;
        decreaseKey(x, 1000000);
        deleteMin();    
        return true;
    }

    /**
     * Remove the minimum item.
     */
    Comparable deleteMin( )
    {
        Comparable result = array[1].element;
        if( isEmpty( ) )
            cout << "\nWARNING: DeleteMin() called on empty heap\n";
        tableRemove(array[1].element);                        //Remove min from table
        array[ 1 ] = array[ currentSize-- ];          //Move max element into root
        tableRemove(array[1].element);                        //Remove max so it can be updated while percolating down
        percolateDown( 1 );
        return result;
    }

    /**
     * Remove the minimum item and place it in minItem.
     * Throws Underflow if empty.
     */
    void deleteMin( Comparable & minItem )
    {
        if( isEmpty( ) )
            cout << "\nWARNING: DeleteMin() called on empty heap\n";
        minItem = array[1];
        tableRemove(minItem);
        array[ 1 ] = array[ currentSize-- ];
        tableRemove(array[1]);   
        percolateDown( 1 );
    }

    void makeEmpty( )
      { currentSize = 0; }

    void displayAll(){
        display(1,"",false);
    }
    
    void rawDisplay(){
        if(!isEmpty()){
            cout << "Index\tElement\tPriority\n";
            cout << "------------------------\n";
            for(int n = 1; n <= currentSize; n++)
                cout << n << "\t" << array[n].element << "\t" << array[n].priority << "\n";
        } else {
            cout << "HEAP IS EMPTY\n";    
        }
        cout << "\n";
    }

  private:
    
     struct HeapElement{
        Comparable element;
        float priority; 
        
        HeapElement(){}
        HeapElement(Comparable e, float p) : element(e), priority(p){}    
    };    
    
    int                currentSize;  // Number of elements in heap
    vector<HeapElement> array;        // The heap array
    HashTable< TableElement<Comparable> > table;      // The hash table

   


    /**
     * Establish heap order property from an arbitrary
     * arrangement of items. Runs in linear time.
     */
    void buildHeap( )
    {
        for( int i = currentSize / 2; i > 0; i-- )
            percolateDown( i );
    }
 
    void percolateUp(int hole){
        HeapElement x = array[hole];
        for( ; hole > 1 && x.priority < array[ hole / 2 ].priority; hole /= 2 ){    
            array[ hole ] = array[ hole / 2 ];                   
            tableUpdate(array[hole].element, hole);                      
        }  
        array[ hole ] = x;                                      
        tableInsert(array[hole].element, hole);                          
    }

    /**
     * Internal method to percolate down in the heap.
     * hole is the index at which the percolate begins.
     */
    void percolateDown( int hole )
    {
        int child;        
        HeapElement tmp = array[ hole ];

        for( ; hole * 2 <= currentSize; hole = child )    
        {
            child = hole * 2;
            if( child != currentSize && array[ child + 1 ].priority < array[ child ].priority )    
                child++;                                                         
            if( array[ child ].priority < tmp.priority ){                                          
                tableRemove(array[hole].element);                                        
                array[ hole ] = array[ child ];                                
                tableUpdate(array[child].element,hole);                                  
            }
            else
                break;
        }
        array[ hole ] = tmp;                                                     
        tableInsert(array[hole].element, hole);                                          
    }
    
    void display(int index, string indent, bool last){
        if(!isEmpty()){
            cout << indent;
            if(last){
                cout << "\\-";
                indent += " ";  
            } else {
                cout << "|-";
                indent += "| ";
            }
            cout << array[index].element << " : " << array[index].priority << "\n";
            if(2*index <= currentSize)
                display(2*index, indent, false);
            if(2*index+1 <= currentSize)
                display(2*index+1, indent, true); 
        } else
            cout << "HEAP IS EMPTY" << "\n\n";
    }
    
    void tableRemove(Comparable element){
        TableElement<Comparable> temp(element,-1);
        table.remove(temp);    
    }
    
    void tableInsert(Comparable element, int index){
        TableElement<Comparable> temp(element, index);
        table.insert(temp);    
    }
    
    void tableUpdate(Comparable element, int newIndex){
        TableElement<Comparable> * temp;
        temp = new TableElement<Comparable>(element, -1);  
        table.remove(*temp);
        delete temp;
        temp = new TableElement<Comparable>(element, newIndex);
        table.insert(*temp);
    }
    
};

#endif
