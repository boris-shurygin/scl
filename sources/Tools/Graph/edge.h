/**
 * @file: edge.h
 * EdgeImpl class definition
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef EDGE_H
#define EDGE_H

/**
 * EdgeImpl lists identificators
 * @ingroup GraphBase 
 */
enum EdgeListType
{
    EDGE_LIST_PREDS,
    EDGE_LIST_SUCCS,
    EDGE_LIST_GraphImpl,
    EDGE_LISTS_NUM
};

/**
 * @class EdgeImpl
 * @brief  Representation of GraphImpl edge
 * @ingroup GraphBase
 *
 * @par
 * EdgeImpl class implements basic concept of GraphImpl edge. Every edge has two adjacent nodes.
 * Use pred() and succ() routines to get them. EdgeImpl is a member of 3 lists: edge list in GraphImpl,
 * pred list in succ node and succ list in pred node. To traverse these lists use nextEdge(),
 * nextPred() and  nextSucc() routines. Also for debug purposes all edges in a GraphImpl
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * @par
 * An edge can be @ref Marked "marked" and @ref Numbered "numbered". @ref Mark "Markers" and
 * @ref Nums "numerations" are managed by @ref GraphImpl "GraphImpl". Note that @ref NodeImpl "nodes" can be marked with the
 * same marker or numbered in the same numeration.
 * Also for debug purposes all nodes in a GraphImpl
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * @par
 * Edges reside in memory pool that is controlled by GraphImpl. Operator new can't be called
 * directly. Edges can be only created by calling GraphImpl::newEdge().
 *
 * @par
 * Every edge have associated QDomElement for XML export support. The updateElement() routine should be called before
 * export to get element in sync with edge's properties.
 *
 * @sa GraphImpl
 * @sa NodeImpl
 * @sa Mark
 * @sa Nums
 */
class EdgeImpl: 
    public MListIface< EdgeImpl, // List item
                       MListItem< EDGE_LISTS_NUM>, // base class: pure multi-list item
                       EDGE_LISTS_NUM >, // Lists number                      
    public Marked,
    public Numbered,
    public PoolObj<EdgeImpl, UseCustomFixedPool>
{
public:
    /** Get edge's unique ID */
    inline GraphUid id() const;

    /** Get edge's GraphImpl */
    inline GraphImpl * graph() const;

    /** 
     *  Destructor.
     *  Delete edge from list in GraphImpl.
     *  Deletion from node lists MUST be performed manually.
     */
    virtual ~EdgeImpl();

    /**
     * Connect edge to a node in specified direction.
     * Note that node treats this edge in opposite direction. I.e. an edge that has node in
     * GRAPH_DIR_UP is treated as edge in GRAPH_DIR_DOWN directions inside that node
     */
    inline void setNode( NodeImpl *n, GraphDir dir);
    
    /** Connect edge with given node as a predecessor */
    inline void setPred( NodeImpl *n);
    /** Connect edge with given node as a successor   */
    inline void setSucc( NodeImpl *n);

    /** Get node in specified direction  */
    inline NodeImpl *node( GraphDir dir) const;
    inline NodeImpl *pred() const;/**< Get predecessor node of edge */
    inline NodeImpl *succ() const;/**< Get successor node of edge   */

    /** Return next edge of the GraphImpl */
    inline EdgeImpl* nextEdge();

    /** Return next edge of the same node in given direction  */
    inline EdgeImpl* nextEdgeInDir( GraphDir dir);
    inline EdgeImpl* nextSucc();/**< Next successor */
    inline EdgeImpl* nextPred();/**< Next predecessor */
    
    /** Print edge in dot fomat to stdout */
    virtual void debugPrint();

    /**
     * Insert a node on this edge
     *
     * Creates a node on edge and a new edge from new node to former successor of original edge.
     * Original edge goes to new node.
     * Return new node.
     */
    virtual NodeImpl *insertNode();
private:
    /** GraphImpl part */
    GraphUid uid; //Unique ID
    GraphImpl * graph_p; //GraphImpl

    /** Nodes */
    NodeImpl *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes
    /** NodeImpl checking routine */
    bool checkNodes( NodeImpl* _pred, NodeImpl* _succ);

protected:
    /** GraphImpl should have access to EdgeImpl's members */
    friend class GraphImpl;
    /** NodeImpl should have access to EdgeImpl's members */
    friend class NodeImpl;

    /** Constructors are made private, only nodes and GraphImpl can create edges */
    EdgeImpl( GraphImpl *_graph_p, GraphUid _id, NodeImpl *_pred, NodeImpl* _succ):
        uid(_id), graph_p(_graph_p)
    {
        GRAPH_ASSERTD( checkNodes( _pred, _succ),
                       "Predecessor and sucessor used in edge construction belong to different GraphImpls");
        setPred( _pred);
        setSucc( _succ);
    }

    /**
     * Detach edge from a node.
     * Made private as it is low-level routine needed for implementation of edge-node relationship
     */
    inline void detachFromNode( GraphDir dir);

    /**
     * Remove myself from GraphImpl's list of edges
     */
    inline void detachFromgraph()
    {
        detach( EDGE_LIST_GraphImpl);
    }
};


template < class G, class N, class E> class Edge: 
    public EdgeImpl,
    public PoolObj<E, UseCustomFixedPool>
{

};


#endif
