//
//  main.cpp
//  test Graphs
//
//  Created by Carlos Jiménez-Bravo on 2017-06-08.
//  Copyright © 2017 Galois Group. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

class EdgeProperty {
public:
    EdgeProperty(){
        _u = 0;
        _v = 0;
        _weight = 0;
    }
    EdgeProperty(int u, int v, int =0) {
        _u = u;
        _v = v;
        _weight = 0;
    }
    int _u, _v, _weight;
    
};

typedef std::vector<EdgeProperty> vec;
typedef std::vector<int> vec_int;

class Graph {
private:
    vec_int _head;
    vec _edge;
    vec _adj;
    vec_int _storePath;
    int _nVertex, _nEdge;
public:
    Graph() {
        _nVertex = 0;
        _nEdge = 0;
    }
    
    Graph(int nVertex, int nEdge) {
        initialize(nVertex, nEdge);
    }
    
    void initialize(int nVertex, int nEdge) {
        _nVertex = nVertex;
        _nEdge = nEdge;
        
        // Store from 1 .. nVertex
        _head.resize(_nVertex + 2);
        
        // Store from 0 .. nEdge - 1
        _edge.resize(_nEdge);
        
        // Store from 0 .. nEdge * 2 - 1
        _adj.resize(_nEdge * 2);
    }
    
    void buildWithWeight(string filename) {
        build(filename, 1);
    }
    
    void buildWithoutWeight(string filename) {
        build(filename, 0);
    }
    
    void build(string filename, bool haveWeight) {
        ifstream fi(filename);
        if (fi.good()) {
            fi >> _nVertex >> _nEdge;
            initialize(_nVertex, _nEdge);
            int u, v;
            
            // Read list of edge from file
            if (haveWeight) {
                int w;
                for (int i = 0; i < _nEdge; i++) {
                    fi >> u >> v >> w;
                    _edge[i] = EdgeProperty(--u, --v, w);
                    _head[u] += 1;
                    _head[v] += 1;
                }
            } else {
                for (int i = 0; i < _nEdge; i++) {
                    fi >> u >> v;
                    _edge[i] = EdgeProperty(--u, --v);
                    _head[u] += 1;
                    _head[v] += 1;
                }
            }
            
            // Start to build nested vertex list
            for (int i = 0; i <= _nVertex; i++) _head[i] += _head[i - 1];
            
            for (int i = 0; i < _nEdge; i++) {
                u = _edge[i]._u;
                v = _edge[i]._v;
                _adj[_head[u]--] = _edge[i];
                
                swap(_edge[i]._u, _edge[i]._v);
                _adj[_head[v]--] = _edge[i];
            }
        }
    }
    
    vec_int headAt(int u) {
        vec_int temp;
        temp.push_back(_head[u] + 1);
        temp.push_back(_head[u + 1]);
        return temp;
    }
    
    EdgeProperty edgeAt(int position) {
        return _edge[position];
    }
    
    EdgeProperty adjAt(int position) {
        return _adj[position];
    }
    
    vec getNeighborOf(int u) {
        vec result;
        for (int i = _head[u] + 1; i <= _head[u + 1]; i++)
            result.push_back(_adj[i]);
        return result;
    }
    
    void BFS(int startPoint, int endPoint) {
        // Create value
        queue<int> Queue;
        vector<bool> visited(_nVertex);
        
        // Initial value
        startPoint -= 1;
        endPoint -= 1;
        
        _storePath.clear();
        _storePath.resize(_nVertex);
        
        Queue.push(startPoint);
        visited[startPoint] = true;
        
        do {
            // Start BFS algorithm
            int u = Queue.front();
            Queue.pop();
            
            for (int i = _head[u] + 1; i <= _head[u + 1]; i++) {
                int v = _adj[i]._v;
                if (!visited[v]) {
                    visited[v] = true;
                    _storePath[v] = u;
                    if (v == endPoint) {
                        print(startPoint, endPoint);
                        return;
                    }
                    Queue.push(v);
                }
            }
        } while (!Queue.empty());
    }
    
    void print(int startPoint, int endPoint) {
        int temp = endPoint;
        vec_int path;
        do {
            path.push_back(endPoint);
            endPoint = _storePath[endPoint];
        } while (endPoint != startPoint);
        path.push_back(startPoint);
        
        cout << "The shortest connection between " << startPoint + 1 << " and " << temp + 1 << " is: " << path.size();
        cout << "\nList of connection: ";
        for (long i = path.size() - 1; i >= 0; i--)
            cout << path[i] + 1 << " ";
        cout << endl;
    }
    
    ~Graph()
    {
        _head.clear();
        _adj.clear();
        _edge.clear();
        _storePath.clear();
        _nEdge = 0;
        _nVertex = 0;
    }
};

template<class T>
struct Node{
    Node<T> *next;
    T item;
    Node(T value)
    {
        item = value;
        next = nullptr;
    }
};

template <class T>
class queue {
private:
    Node<T> *head;
public:
T front()
{
    return head->item;
    head = head->next;
}

void push(T value, Node<T> *ptr)
{
    Node<T> *entry = new Node<T>(value);
    if (ptr->next == nullptr) {
        ptr->next = entry;
    } else {
        push(entry, ptr->next);
    }
}
};

template<class T>
class Dynamicarray
{
private:
    int size;
    T *array;
public:
void resize(int value)
{
    T *newArray = new T[value];
    for (int i=0; i<size; i++)
        *(newArray+i) = *(array+i);
    delete []array ;
    array = newArray;
}

void fillarray(T value)
{
    for(int i=0; i<size ; i++)
        array[i] = value;
}

void add(T value, T array[])
{
    size = size+1;
    resize(size);
    array[size-1] = value;
}

void replace(T value, int position)
{
    array[position] = value;
}

T operator[](int position)
{
    return array[position];
}

};

int main() {
    Graph model;
    model.buildWithoutWeight("/Users/agustin/Desktop/input.txt");
    model.BFS(1, 6);
    return 0;
}







