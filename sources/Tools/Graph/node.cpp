/**
 * @file: node.cpp
 * NodeImpl class implementation
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
/** 
 * Destructor. Corrects list of nodes in corresponding GraphImpl and deletes adjacent edges
 */
#include "graph_iface.h"

NodeImpl::~NodeImpl()
{
    EdgeImpl *edge;
    //out("Deleted NodeImpl");

    /** delete incidient edges */
    for ( edge = firstSucc(); isNotNullP( edge);)
    {
        EdgeImpl* next = edge->nextSucc();
        //edge->detachFromNode( GRAPH_DIR_DOWN);// EdgeImpl is detached from succ node
        GraphImpl()->deleteEdge( edge);
        edge = next;
    }
    for ( edge = firstPred(); isNotNullP( edge);)
    {
        EdgeImpl* next = edge->nextPred();
        //edge->detachFromNode( GRAPH_DIR_UP);// EdgeImpl is detached from pred node
        GraphImpl()->deleteEdge( edge);
        edge = next;
    }
    
    /** delete myself from GraphImpl */
    GraphImpl_p->detachNode( this);
}

/**
 * Print node in Dot format to stdout
 */
void
NodeImpl::debugPrint()
{
    out("%llu;", id());
}
