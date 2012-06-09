/**
 * @file: edge.h
 * Implementation of EdgeImpl class inline routines
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef EDGE_INLINE_H
#define EDGE_INLINE_H

/**
 * Low level correction of node's edge list in corresponding direction
 */
inline void
EdgeImpl::detachFromNode( GraphDir dir)
{
    if ( isNotNullP( node( dir)))
    {
        NodeImpl *n = node( dir);
        n->deleteEdgeInDir( RevDir( dir), (EdgeImpl* )this);
        detach( RevDir( dir));
        nodes[ dir] = 0;
    }
}

/**
 * Connect edge to a node in specified direction.
 * Note that node treats this edge in opposite direction. I.e. an edge that has node in
 * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
 */
inline void 
EdgeImpl::setNode( NodeImpl *n, GraphDir dir)
{
    assert( isNotNullP( n));
    nodes[ dir] = n;
    if ( n != NULL)
    {
        n->AddEdgeInDir( (EdgeImpl *)this, 
            ((dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN : GRAPH_DIR_UP));
    }
}

/**
 * Get edge's unique ID
 */
inline GraphUid EdgeImpl::id() const
{
    return uid;
}

/**
 * Get edge's corresponding GraphImpl
 */
inline GraphImpl * EdgeImpl::graph() const
{
    return graph_p;
}

/**
 * Connect edge with given node as a predecessor
 */
inline void EdgeImpl::setPred( NodeImpl *n)
{
    setNode( n, GRAPH_DIR_UP);
}
/**
 * Connect edge with given node as a successor
 */
inline void EdgeImpl::setSucc( NodeImpl *n)
{
    setNode( n, GRAPH_DIR_DOWN);
}

/**
 * Get node in specified direction
 */
inline NodeImpl *EdgeImpl::node( GraphDir dir) const
{
    return nodes[ dir];
}
/**
 * Get predecessor of edge
 */
inline NodeImpl *EdgeImpl::pred() const
{
    return node( GRAPH_DIR_UP);
}
/**
 * Get successor of edge
 */
inline NodeImpl *EdgeImpl::succ() const
{
    return node( GRAPH_DIR_DOWN);
}

/**
 * Return next edge of the GraphImpl
 */
inline EdgeImpl* EdgeImpl::nextEdge()
{
    return next( EDGE_LIST_GraphImpl);
}

/**
 * Return next edge of the same node in given direction
 */
inline EdgeImpl* EdgeImpl::nextEdgeInDir( GraphDir dir)
{
    GRAPH_ASSERTD( dir < GRAPH_DIRS_NUM, "Wrong direction parameter");
    GRAPH_ASSERTD( (int) GRAPH_DIR_DOWN == (int) EDGE_LIST_SUCCS,
                   "Enums of direction and edge lists are not having right values");
    GRAPH_ASSERTD( (int) GRAPH_DIR_UP == (int) EDGE_LIST_PREDS,
                   "Enums of direction and edge lists are not having right values");
    return next( dir);
}

/**
 * Next successor
 */
inline EdgeImpl* EdgeImpl::nextSucc()
{
    return nextEdgeInDir( GRAPH_DIR_DOWN);
}

/**
 * Next predecessor
 */
inline EdgeImpl* EdgeImpl::nextPred()
{
    return nextEdgeInDir( GRAPH_DIR_UP);
}

/**
 * Insert a node on this edge
 *
 * Creates a node on edge and a new edge from new node to former successor of original edge.
 * Original edge goes to new node. 
 * Return new node.
 */
inline NodeImpl *
EdgeImpl::insertNode()
{
    NodeImpl *tmp_succ = succ();
    NodeImpl *new_node = graph()->newNode();
    detachFromNode( GRAPH_DIR_DOWN);
    setSucc( new_node);
    graph()->newEdge( new_node, tmp_succ);
    return new_node;
}


#endif