/**
 * @file: edge.cpp 
 * EdgeImpl class implementation
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * EdgeImpl destructor.
 * delete edge from GraphImpl's list of edges
 */
EdgeImpl::~EdgeImpl()
{
    //out("Deleted edge");
    GraphImpl_p->detachEdge( this);
    detachFromNode( GRAPH_DIR_UP);
    detachFromNode( GRAPH_DIR_DOWN);
}

/**
 * Print edge in DOT format to stdout
 */
void
EdgeImpl::debugPrint()
{
    /**
     * Check that edge is printable
     * TODO: Implements GraphImpl states and in 'in process' state print node as '?'
     *       Examples of such prints: 4->? ?->3 ?->?
     */
    assert( isNotNullP( pred()));
    assert( isNotNullP( succ()));

    out("%llu->%llu;", pred()->id(), succ()->id());
}

/** NodeImpl checking routine */
bool EdgeImpl::checkNodes( NodeImpl* _pred, NodeImpl* _succ)
{
    return isNotNullP( _pred)
           && isNotNullP( _succ)
           && areEqP( this->GraphImpl(), _pred->GraphImpl())
           && areEqP( _pred->GraphImpl(), _succ->GraphImpl());
}
