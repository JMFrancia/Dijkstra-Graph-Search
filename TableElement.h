/*
Jordan Francia
Assignment 5: Dijkstra

TableElement holds an element (T) and an index (int) for a modified hashtable (QuadraticProbing.h) 
which is used by BinaryHeap.H.
*/

#ifndef TABLE_ELEMENT_H
#define TABLE_ELEMENT_H

template <typename T>
class TableElement{
    public: 
        T element;
        int index;    
    
        TableElement(T e, int i):index(i),element(e){}
        TableElement(){}

        bool operator==(const TableElement &rhs){
            return (element == rhs.element);
        }
        
        bool operator!=(const TableElement &rhs){
            return (element != rhs.element);    
        }
        
        TableElement& operator=(const TableElement &rhs){
            TableElement * result = new TableElement(rhs.element, rhs.index);
            return *result;    
        }
        
        bool operator<(const TableElement &rhs){
            return (element < rhs.element);    
        }
        
        bool operator>(const TableElement &rhs){
            return (element > rhs.element);    
        }

};


#endif
