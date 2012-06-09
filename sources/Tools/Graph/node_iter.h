/**
 * @file: node_iter.h
 * Declaration of iterators for node's adjacent edges traversal and related classes
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef NODE_ITER_H
#define NODE_ITER_H


/** Baseclass for implementing iterator template parameters */
class IterImplBase
{
public:
    /** Get edge */
    inline EdgeImpl* edge() const { return edge_p;}
    /** Set edge */
    inline void setEdge( EdgeImpl *e){ edge_p = e;}
    /** Default constructor */
    inline IterImplBase(): edge_p( NULL) {}
protected:
    EdgeImpl *edge_p;
};

/** Parameter for iterator template specialization (pred traversal) */
class PredIterImpl: public IterImplBase
{
public:
    inline void nextEdge();       /**< Move on to the next pred */
    inline NodeImpl *node() const;    /**< Get node */
    inline PredIterImpl(){};      /**< Default constructor */
    inline PredIterImpl( NodeImpl *n);/**< Configures iterator with node's first pred */
    inline bool operator==(const PredIterImpl& o) const /**< Comparison operator */
    { 
        return edge_p == o.edge_p;
    }
};
/** Parameter for iterator template specialization (succ traversal) */
class SuccIterImpl: public IterImplBase
{
public:
    inline void nextEdge();       /**< Move on to the next succ */
    inline NodeImpl *node() const;    /**< Get node */
    inline SuccIterImpl(){};      /**< Default constructor */
    inline SuccIterImpl( NodeImpl *n);/**< Configures iterator with node's first succ */
    inline bool operator==(const SuccIterImpl& o) const /**< Comparison operator */
    { 
        return edge_p == o.edge_p;
    }
};

/** Parameter for iterator template specialization (undirected traversal) */
class UnDirIterImpl: public IterImplBase
{
public:
    inline void nextEdge();        /**< Move on to the next edge */
    inline NodeImpl *node() const;     /**< Get node */
    inline UnDirIterImpl():is_pred( false){};      /**< Default consturctor */
    inline UnDirIterImpl( NodeImpl *n);/**< Configures iterator with node's first edge */
    inline bool operator==(const UnDirIterImpl& o) const /**< Comparison operator */
    { 
        return edge_p == o.edge_p 
               && is_pred == o.is_pred;
    }
private:
    bool is_pred;
};

/**
 * Convinience template for iterating through node's adjacent edges
 */
template < class EdgeIterImpl> class EdgeIterIface
{   
public:
    /** Default constructor */
    inline EdgeIterIface();
    /** Copy constructor */
    inline EdgeIterIface( const EdgeIterIface& proto);
    /** Destructor */
    inline ~EdgeIterIface();
    /** Preincrement operator */
    inline EdgeIterIface & operator++();
    /** Postincrement operator */
    inline EdgeIterIface operator++( int);
    /** Dereferenece operator*/
    inline EdgeImpl * operator*();
    /** Comparison operator */
    inline bool operator==(const EdgeIterIface& o) const; 
    /** Not equals operator */
    inline bool operator!=(const EdgeIterIface& o) const;
    /** Get EdgeImpl */
    inline EdgeImpl *edge() const;
    /** Get node on the end of edge */
    inline NodeImpl *node() const;
private:
    /** Constructor from node */
    inline EdgeIterIface( NodeImpl *e);
    friend class NodeImpl;

    /** Parameter-dependent implementation */
    EdgeIterImpl impl;
};

#endif