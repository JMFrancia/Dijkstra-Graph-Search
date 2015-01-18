/*
Assignment 5: Dijkstra
Jordan Francia

HashTable class from Data Structures and Analysis in C++ by Mark Allen Weiss,
modified to use the Heap Element object in order to have easier interactions with 
the modified BinaryHeap class. 

CONSTRUCTION: an approximate initial size or default of 101

******************PUBLIC OPERATIONS*********************
void display()         --> Displays active contents of hash table
HashedObj get(int key) --> Returns Hashed Object with key "key" 
bool insert( x )       --> Insert x
bool remove( x )       --> Remove x
bool contains( x )     --> Return true if x is present
void makeEmpty( )      --> Remove all items
int hash( string str ) --> Global method to hash strings
*/

#ifndef QUADRATIC_PROBING_H
#define QUADRATIC_PROBING_H

#include <vector>
#include <string>
#include "TableElement.h"
#include <iostream>

using namespace std;

int nextPrime( int n );

template <typename HashedObj>
class HashTable
{
  public:
    explicit HashTable( int size = 101 ) : array( nextPrime( size ) )
      { makeEmpty( ); }
    
    void display(){         //Displays active contents of hash table for debugging purposes
        cout << "Hash Index\tHeap Element\tHeap Index\n";
        cout << "----------------------------------------------\n";
        for(int n = 0; n < array.size(); n++){
            if(array[n].info == ACTIVE){
               cout << n << "\t\t" << array[n].element.element << "\t\t";
               cout << array[n].element.index << "\n";
            }
        }    
        cout << "\n\n";
    }  
      
    HashedObj get(int key){
        HashedObj result;
        int hashVal = hash(key);
        
        hashVal %= array.size( );
        if( hashVal < 0 )
            hashVal += array.size( );
        if(array[hashVal].info == EMPTY)
            cout << "\nERROR: get() called on nonexistant key " << key << "\n";
        else{ 
            result.element = array[hashVal].element.element; //Shouldn't have to do this, but it fixes a bug
            result.index = array[hashVal].element.index;     //Same here
        }
        return result;
    }

    bool contains( const HashedObj & x ) 
    {
        return isActive( findPos( x ) );
    }

    void makeEmpty( )
    {
        currentSize = 0;
        for( int i = 0; i < array.size( ); i++ )
            array[ i ].info = EMPTY;
    }

    bool insert( const HashedObj & x )
    {
            // Insert x as active
        int currentPos = findPos( x );
        if( isActive( currentPos ) )
            return false;

        HashEntry temp(x, ACTIVE);
        array[ currentPos ] = temp;
        array[currentPos].element.element = temp.element.element;  //Shouldn't need to do this, but fixes a bug
        array[currentPos].element.index = temp.element.index;      // Same here
            // Rehash; see Section 5.5
        if( ++currentSize > array.size( ) / 2 )
            rehash( );

        return true;
    }

    bool remove( const HashedObj & x )
    {
        int currentPos = findPos( x );
        if( !isActive( currentPos ) )
            return false;

        array[ currentPos ].info = DELETED;
        return true;
    }

    enum EntryType { ACTIVE, EMPTY, DELETED };

  private:
    struct HashEntry
    {
        HashedObj element;
        EntryType info;

        HashEntry( const HashedObj & e = HashedObj( ), EntryType i = EMPTY )
          : element( e ), info( i ) {}
    };
    
    vector<HashEntry> array;
    int currentSize;

    bool isActive( int currentPos ) const
      { return array[ currentPos ].info == ACTIVE; }

    int findPos( const HashedObj & x )
    {
        int offset = 1;
        int currentPos = myhash( x );

          // Assuming table is half-empty, and table length is prime,
          // this loop terminates
        while( array[ currentPos ].info != EMPTY &&
                array[ currentPos ].element != x )
        {
            currentPos += offset;  // Compute ith probe
            offset += 2;
            if( currentPos >= array.size( ) )
                currentPos -= array.size( );
        }

        return currentPos;
    }

    void rehash( )
    {
        vector<HashEntry> oldArray = array;

            // Create new double-sized, empty table
        array.resize( nextPrime( 2 * oldArray.size( ) ) );
        for( int j = 0; j < array.size( ); j++ )
            array[ j ].info = EMPTY;

            // Copy table over
        currentSize = 0;
        for( int i = 0; i < oldArray.size( ); i++ )
            if( oldArray[ i ].info == ACTIVE )
                insert( oldArray[ i ].element );
    }
    int myhash( const HashedObj & x ) 
    {
        int hashVal = hash( x );

        hashVal %= array.size( );
        if( hashVal < 0 )
            hashVal += array.size( );

        return hashVal;
    }

int nextPrime( int n )
{
    if( n <= 0 )
        n == 3;

    if( n % 2 == 0 )
        n++;

    for( ; !isPrime( n ); n += 2 )
        ;

    return n;
}

    bool isPrime( int n ){
        if( n == 2 || n == 3 )
            return true;
        if( n == 1 || n % 2 == 0 )
            return false;
        for( int i = 3; i * i <= n; i += 2 )
            if( n % i == 0 )
                return false;
        return true;
    }


int hash( int key ){
    return key;
}

int hash(const TableElement<int> key){
    return hash(key.element);    
}


};

#endif
