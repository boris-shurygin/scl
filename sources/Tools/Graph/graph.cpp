/**
 * @file: graph.cpp
 * Graph class implementation
 */
/*
 * Graph library, internal representation of graphs in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * Constructor.
 */
Graph::Graph( bool create_pools):
    node_next_id( 0),
    edge_next_id( 0),
    node_num( 0),
    edge_num( 0),
    first_node( NULL),
    first_edge( NULL),
    node_pool( NULL),
    edge_pool( NULL)
{
    if ( create_pools)
        createPools();
}

/**
 * Destructor - removes all nodes
 */
Graph::~Graph()
{
    for ( Node *node = firstNode();
          isNotNullP( node);
          )
    {
        Node* next = node->nextNode();
        deleteNode( node);
        node = next;
    }
    destroyPools();
}

/** Pools' creation routine */
void Graph::createPools()
{
    node_pool = new FixedPool< Node>();
    edge_pool = new FixedPool< Edge>();
}

/** Pools' destruction routine */
void Graph::destroyPools()
{
    delete node_pool;
    delete edge_pool;
}

/** Node/Edge creation routines can be overloaded by derived class */
Node * 
Graph::createNode( GraphUid _id)
{
    return new ( node_pool) Node ( this, _id);
}

Edge * 
Graph::createEdge( GraphUid _id, Node *_pred, Node* _succ)
{
    return new ( edge_pool) Edge( this, _id, _pred, _succ);
}

/**
 * Print graph to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
void 
Graph::debugPrint()
{
    Node *n;
    Edge *e;
    out( "digraph{");
    /** Print nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->debugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->debugPrint();
    }
    out( "}");
}

/**
 * Implementation for numerations cleanup
 */
void 
Graph::clearNumerationsInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for ( n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedNumerations( n);
    }
    /** Clean markers in edges */
    for ( e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedNumerations( e);
    }
}

/**
 * Implementation for markers cleanup
 */
void 
Graph::clearMarkersInObjects()
{
    Node *n;
    Edge *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
}
