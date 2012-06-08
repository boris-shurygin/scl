/**
 * @file: mem_pool.cpp 
 * Implementation of memory pool
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"

using namespace Mem;

class Graph;

class Node
{
public:
    Node (Graph *g);
};

class Graph
{
public:
    void registerNode( Node* n)
    {
        node = n;
    }
private:
    Node *node;
};

Node::Node( Graph *g)
{
    g->registerNode( this);
}

class AGraph;

class ANode:public Node, public PoolObj<ANode>
{
public:
    ANode( AGraph *);
public:
    int a;
};

class AGraph: public Graph
{

};

ANode::ANode( AGraph *g): Node( g)
{

}

void foo()
{
    AGraph a;
    ANode *n = new ANode( &a);
}