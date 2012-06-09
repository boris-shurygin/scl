/**
 * @file: GraphImpl.cpp
 * GraphImpl class implementation
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * Constructor.
 */
GraphImpl::GraphImpl():
    node_next_id( 0),
    edge_next_id( 0),
    node_num( 0),
    edge_num( 0),
    first_node( NULL),
    first_edge( NULL)
{

}

/**
 * Destructor - removes all nodes
 */
GraphImpl::~GraphImpl()
{
    for ( NodeImpl *node = firstNode();
          isNotNullP( node);
          )
    {
        NodeImpl* next = node->nextNode();
        deleteNode( node);
        node = next;
    }
}

/** NodeImpl/EdgeImpl creation routines can be overloaded by derived class */
NodeImpl * 
GraphImpl::createNode( GraphUid _id)
{
    return new ( node_pool) NodeImpl ( this, _id);
}

EdgeImpl * 
GraphImpl::createEdge( GraphUid _id, NodeImpl *_pred, NodeImpl* _succ)
{
    return new ( edge_pool) EdgeImpl( this, _id, _pred, _succ);
}

/**
 * Print GraphImpl to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
void 
GraphImpl::debugPrint()
{
    NodeImpl *n;
    EdgeImpl *e;
    out( "diGraphImpl{");
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
GraphImpl::clearNumerationsInObjects()
{
    NodeImpl *n;
    EdgeImpl *e;
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
GraphImpl::clearMarkersInObjects()
{
    NodeImpl *n;
    EdgeImpl *e;
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
