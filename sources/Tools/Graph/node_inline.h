/**
 * @file: node_inline.h
 * Implementation of NodeImpl and related classes' inline routines
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef NODE_INLINE_H
#define NODE_INLINE_H

/** We can't create nodes separately, do it through newNode method of GraphImpl */
inline NodeImpl::NodeImpl( GraphImpl *_GraphImpl_p, GraphUid _id): uid(_id), GraphImpl_p( _GraphImpl_p)
{
    first_edge[ GRAPH_DIR_UP] = NULL;
    first_edge[ GRAPH_DIR_DOWN] = NULL;
}

/**
 * Detach myself from GraphImpl's node list
 */
inline void NodeImpl::detachFromGraph()
{
    detach();
}

/**
 * Get node's unique ID
 */
inline GraphUid NodeImpl::id() const
{
    return uid;
}

/**
 * Get node's corresponding GraphImpl
 */
inline GraphImpl * NodeImpl::graph() const
{
    return GraphImpl_p;
}
/**
 * Next node in GraphImpl's list
 */
inline NodeImpl* NodeImpl::nextNode()
{
    return next();
}

/**
 * Prev node in GraphImpl's list
 */
inline NodeImpl* NodeImpl::prevNode()
{
    return prev();
}

/**
 * Add predecessor edge
 */
inline void NodeImpl::AddPred( EdgeImpl *edge)
{
    AddEdgeInDir( edge, GRAPH_DIR_UP);
}

/**
 * Add successor edge
 */
inline void NodeImpl::AddSucc( EdgeImpl *edge) 
{
    AddEdgeInDir( edge, GRAPH_DIR_DOWN);
}
/**
 * First edge in given direction
 */
inline EdgeImpl* NodeImpl::firstEdgeInDir( GraphDir dir)
{
    return first_edge[ dir];
}
/** 
 * First successor edge
 */
inline EdgeImpl* NodeImpl::firstSucc()
{
    return firstEdgeInDir( GRAPH_DIR_DOWN);
}
/** 
 * First predecessor edge
 */
inline EdgeImpl* NodeImpl::firstPred()
{
    return firstEdgeInDir( GRAPH_DIR_UP);
}

/**
 * delete predecessor edge
 */
inline void NodeImpl::deletePred( EdgeImpl* edge)
{
    deleteEdgeInDir( GRAPH_DIR_UP, edge);
}

/**
 * delete successor edge
 */
inline void NodeImpl::deleteSucc( EdgeImpl* edge)
{
    deleteEdgeInDir( GRAPH_DIR_DOWN, edge);
}

/**
 * Create iterator for first succ
 */
inline NodeImpl::Succ NodeImpl::succsBegin()
{
    return Succ( this);
}

/**
 * Create iterator pointing to succ end
 */
inline NodeImpl::Succ NodeImpl::succsEnd()
{
    return Succ();
}
/**
 * Create iterator for first succ
 */
inline NodeImpl::Pred NodeImpl::predsBegin()
{
    return Pred( this);
}
/**
 * Create iterator pointing to succ end
 */
inline NodeImpl::Pred NodeImpl::predsEnd()
{
    return Pred();
}

/**
 * Create iterator for first succ
 */
inline NodeImpl::EdgeIter NodeImpl::edgesBegin()
{
    return EdgeIter( this);
}
/**
 * Create iterator pointing to succ end
 */
inline NodeImpl::EdgeIter NodeImpl::edgesEnd()
{
    return EdgeIter();
}

/**
 * Add an edge to this node in specified direction
 */
inline void
NodeImpl::AddEdgeInDir( EdgeImpl *edge, GraphDir dir)
{
    assert( isNotNullP( edge));
    GRAPH_ASSERTD( (int) GRAPH_DIR_DOWN == (int) EDGE_LIST_SUCCS,
                   "Enums of direction and edge lists are not having right values");
    GRAPH_ASSERTD( (int) GRAPH_DIR_UP == (int) EDGE_LIST_PREDS,
                   "Enums of direction and edge lists are not having right values");
    edge->attach( dir, first_edge[ dir]); 
    first_edge[ dir] = edge;
}

/**
 * delete edge pointed by iterator in specidied direction
 */
inline void
NodeImpl::deleteEdgeInDir( GraphDir dir, EdgeImpl* edge)
{
    assert( isNotNullP( edge));
    if( first_edge[ dir] == edge)
    {
        first_edge[ dir] = edge->nextEdgeInDir( dir);
    }
}

/** Default Constructor: creates 'end' iterator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl>::EdgeIterIface()
{

}

/** Constructor from node: iterator points on first edge, i.e. 'begin' iterator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl>::EdgeIterIface( NodeImpl *n):
    impl( n)
{

}

/** Copy constructor */
template < class EdgeIterImpl> 
inline EdgeIterIface< EdgeIterImpl>::EdgeIterIface( const EdgeIterIface& proto)
{
    impl = proto.impl;
}

/** Destructor */
template < class EdgeIterImpl> 
inline EdgeIterIface< EdgeIterImpl>::~EdgeIterIface()
{

}

/** Postincrement operator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl> 
EdgeIterIface< EdgeIterImpl>::operator++( int)
{
    EdgeIterIface tmp = *this;
    ++*this;
    return tmp;
}

/** Dereferenece operator*/
template < class EdgeIterImpl>
inline EdgeImpl * 
EdgeIterIface< EdgeIterImpl>::operator*()
{
    return impl.edge();
}
    
/** Comparison operator */
template < class EdgeIterImpl> 
inline bool
EdgeIterIface< EdgeIterImpl>::operator==(const EdgeIterIface< EdgeIterImpl>& o) const
{
    return impl == o.impl;
} 

/** Not equals operator */
template < class EdgeIterImpl>
inline bool
EdgeIterIface< EdgeIterImpl>::operator!=(const EdgeIterIface< EdgeIterImpl>& o) const
{
    return !(*this == o);
}

/** Get EdgeImpl */
template < class EdgeIterImpl>
inline EdgeImpl *
EdgeIterIface< EdgeIterImpl>::edge() const
{
    return impl.edge();
}

/** Get EdgeImpl */
template < class EdgeIterImpl>
inline NodeImpl *
EdgeIterIface< EdgeIterImpl>::node() const
{
    return impl.node();
}

/** Preincrement operator */
template < class EdgeIterImpl>
inline EdgeIterIface< EdgeIterImpl> & 
EdgeIterIface< EdgeIterImpl>::operator++()
{
    GRAPH_ASSERTD( isNotNullP( impl.edge()), "EdgeImpl iterator is at end ( NULL in edge pointer)");
    impl.nextEdge();
    return *this;
}

/** Next pred */
inline void PredIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "EdgeImpl iterator is at end ( NULL in edge_p pointer)");
    edge_p = edge_p->nextPred();
}


/** Next succ */
inline void SuccIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "EdgeImpl iterator is at end ( NULL in edge_p pointer)");
    edge_p = edge_p->nextSucc();
}

/** Next edge in undirected edges traversal */
inline void UnDirIterImpl::nextEdge()
{
    GRAPH_ASSERTD( isNotNullP( edge_p), "EdgeImpl iterator is at end ( NULL in edge_p pointer)");
    
    if ( is_pred && isNullP( edge_p->nextPred()))
    {
        is_pred = false;
        edge_p = edge_p->succ()->firstSucc();
    } else
    {
        if ( is_pred)
        {
            edge_p = edge_p->nextPred();
        } else
        {
            edge_p = edge_p->nextSucc();
        }
    }
}

/** Get node on the end of edge */
inline NodeImpl * SuccIterImpl::node() const
{
    return edge()->succ();
}

/** Get node on the end of edge */
inline NodeImpl * PredIterImpl::node() const
{
    return edge()->pred();
}

/** Get node in UnDir traversal of node's edges */
inline NodeImpl * UnDirIterImpl::node() const
{
    if ( is_pred)
    {
        return edge()->pred();
    } else
    {
        return edge()->succ();
    }    
}

/** Constructor gets first succ */
inline SuccIterImpl::SuccIterImpl( NodeImpl *n)
{
    edge_p = n->firstSucc();
}

/** Constructor gets first pred */
inline PredIterImpl::PredIterImpl( NodeImpl *n)
{
    edge_p = n->firstPred();
}


/** Constructor gets first edge for undirected edges iteration */
inline UnDirIterImpl::UnDirIterImpl( NodeImpl *n)
{
    edge_p = n->firstPred();
    is_pred = true;
    if ( isNullP( edge_p)) 
    {
        is_pred = false;
        edge_p = n->firstSucc();
    } 
}

#endif /* NODE_INLINE_H */