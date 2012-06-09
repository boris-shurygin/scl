/**
 * @file: GraphImpl_inline.h
 * Implementation of GraphImpl's inline routines
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef GraphImpl_INLINE_H
#define GraphImpl_INLINE_H

/** Delete node from memory pool */
inline void GraphImpl::deleteNode( NodeImpl *n)
{
    node_pool->destroy( n);
}

/** Delete edge from memory pool */
inline void GraphImpl::deleteEdge( EdgeImpl *e)
{
    edge_pool->destroy( e);
}

/**
 * Remove node from node list of GraphImpl
 */
inline void GraphImpl::detachNode( NodeImpl* node)
{
    assert( isNotNullP( node));
    assert( node->GraphImpl() == this);

    if( first_node == node)
    {
       first_node = node->nextNode();
    }
    node->detachFromGraphImpl();
    node_num--;
}

/**
 * Remove edge from edge list of GraphImpl
 */
inline void GraphImpl::detachEdge( EdgeImpl * edge)
{
    assert( isNotNullP( edge));
    assert( edge->GraphImpl() == this);

    if( first_edge == edge)
    {
       first_edge = edge->nextEdge();
    }
    edge->detachFromGraphImpl();
    edge_num--;
}

/**
 * Return node quantity
 */
inline GraphNum GraphImpl::nodeCount() const
{
    return node_num;
}

/**
 * Return edge quantity
 */
inline GraphNum GraphImpl::edgeCount() const
{
    return edge_num;
}
/** 
 * Get first edge
 */
inline EdgeImpl* GraphImpl::firstEdge() 
{
    return first_edge;
}

/**
 * Get first node
 */
inline NodeImpl* GraphImpl::firstNode()
{
    return first_node;
}

/** Get pool of nodes */
inline Pool *GraphImpl::nodePool() const
{
    return node_pool;
}
/** Get pool of edges */
inline Pool *GraphImpl::edgePool() const
{
    return edge_pool;
}

/**
 * Creation node in GraphImpl implementation
 */
inline NodeImpl * 
GraphImpl::newNodeImpl( GraphUid id)
{
    /**
     * Check that we have available node id 
     */
    assert( edge_next_id < GraphImpl_MAX_NODE_NUM);
    
    /** Create node */
    NodeImpl *node_p = this->createNode( id);
    
    /** Add node to GraphImpl's list of nodes */
    node_p->attach( first_node);
    first_node = node_p;
    
    node_num++;
    
    /** Make sure that next automatically assigned id will be greater than given id */
    if ( node_next_id <= id)
        node_next_id = id + 1;
    return node_p;
}

/**
 * Creation node in GraphImpl
 */
inline NodeImpl * 
GraphImpl::newNode()
{
    NodeImpl * node_p = newNodeImpl( node_next_id);
    return node_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
inline EdgeImpl * 
GraphImpl::newEdgeImpl( NodeImpl * pred, NodeImpl * succ)
{
    /**
     * Check that we have available edge id 
     */
    assert( edge_next_id < GraphImpl_MAX_NODE_NUM);
    EdgeImpl *edge_p = this->createEdge( edge_next_id++, pred, succ);
    edge_p->attach( EDGE_LIST_GraphImpl, first_edge);
    first_edge = edge_p;
    edge_num++;
    return edge_p;
}

/**
 * Create edge between two nodes.
 * We do not support creation of edge with undefined endpoints
 */
inline EdgeImpl * 
GraphImpl::newEdge( NodeImpl * pred, NodeImpl * succ)
{
    EdgeImpl *edge_p = newEdgeImpl( pred, succ);
    return edge_p;
}

#endif /** GraphImpl_INLINE_H */