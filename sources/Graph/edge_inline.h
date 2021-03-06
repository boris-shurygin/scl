/**
 * @file: edge.h
 * Implementation of EdgeImpl class inline routines
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef EDGE_INLINE_H
#define EDGE_INLINE_H

namespace Graph
{

/** Constructor implementation */
inline EdgeImpl::EdgeImpl( NodeImpl *_pred, NodeImpl* _succ)
{
    GRAPH_ASSERTXD( checkNodes( _pred, _succ),
                    "Predecessor and sucessor used in edge construction belong to different GraphImpls");
    setPred( _pred);
    setSucc( _succ);
}

/**
 * Low level correction of node's edge list in corresponding direction
 */
inline void
EdgeImpl::detachFromNode( GraphDir dir)
{
    if ( isNotNullP( node( dir)))
    {
        NodeImpl *n = node( dir);
        n->deleteEdgeInDir( RevDir( dir), this);
        if (dir == GRAPH_DIR_UP)
        {
            SuccList::detach();
        } else
        {
            GRAPH_ASSERTD(dir == GRAPH_DIR_DOWN);
            PredList::detach();
        }
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
    GRAPH_ASSERTD( isNotNullP( n));
    nodes[ dir] = n;
    if ( isNotNullP( n))
    {
        n->AddEdgeInDir( this, RevDir( dir));
    }
}

/**
 * Get edge's unique ID
 */
inline GraphUid EdgeImpl::id() const
{
    return uid;
}

/** Set edge's unique ID           */
inline void EdgeImpl::setId( GraphUid new_id)
{
    uid = new_id;
}

/**
 * Get edge's corresponding GraphImpl
 */
inline GraphImpl * EdgeImpl::graph() const
{
    return graph_p;
}

/** Set edge's pointer to graph */
inline void EdgeImpl::setGraph( GraphImpl *g)
{
    graph_p = g;
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
inline EdgeImpl* EdgeImpl::nextEdge() const
{
    return static_cast<EdgeImpl *>(EdgeList::next());
}

/**
 * Return next edge of the same node in given direction
 */
inline EdgeImpl* EdgeImpl::nextEdgeInDir( GraphDir dir) const
{
    GRAPH_ASSERTXD( dir < GRAPH_DIRS_NUM, "Wrong direction parameter");
    
    if (dir == GRAPH_DIR_DOWN) return static_cast<EdgeImpl *>( SuccList::next());
    if (dir == GRAPH_DIR_UP)   return static_cast<EdgeImpl *>( PredList::next());
    
    GRAPH_ASSERTXD(0, "Wrong directions number");
    return 0;
}

/**
 * Next successor
 */
inline EdgeImpl* EdgeImpl::nextSucc() const
{
    return nextEdgeInDir( GRAPH_DIR_DOWN);
}

/**
 * Next predecessor
 */
inline EdgeImpl* EdgeImpl::nextPred() const
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
template < class G, class N, class E> 
N * 
Edge< G, N, E>::insertNode()
{
    N *tmp_succ = succ();
    N *new_node = static_cast< G*>(graph())->newNode();
    detachFromNode( GRAPH_DIR_DOWN);
    setSucc( new_node);
    static_cast< G*>(graph())->newEdge( new_node, tmp_succ);
    return new_node;
}

/** Get node in specified direction  */
template < class G, class N, class E> 
inline N * 
Edge< G, N, E>::node( GraphDir dir) const
{
    return static_cast< N*>( EdgeImpl::node( dir));
}
    
/** Get predecessor node of edge */
template < class G, class N, class E> 
N * 
Edge< G, N, E>::pred() const
{
    return static_cast< N*>( EdgeImpl::pred());
}

/** Get successor node of edge   */
template < class G, class N, class E> 
N * 
Edge< G, N, E>::succ() const
{
return static_cast< N*>( EdgeImpl::succ());
}

/** Return next edge of the GraphImpl */
template < class G, class N, class E> 
E * 
Edge< G, N, E>::nextEdge() const
{
    return static_cast< E*>( EdgeImpl::nextEdge());
}

/** Return next edge of the same node in given direction  */
template < class G, class N, class E> 
E * 
Edge< G, N, E>::nextEdgeInDir( GraphDir dir) const
{
    return static_cast< E*>( EdgeImpl::nextEdgeInDir( dir));
}

/** Next successor */
template < class G, class N, class E> 
E * 
Edge< G, N, E>::nextSucc() const
{
    return static_cast< E*>( EdgeImpl::nextSucc());
}

/** Next predecessor */
template < class G, class N, class E> 
E * 
Edge< G, N, E>::nextPred() const
{
    return static_cast< E*>( EdgeImpl::nextPred());
}

} // namespace Graph
#endif
