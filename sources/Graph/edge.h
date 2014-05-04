/**
 * @file: edge.h
 * EdgeImpl class definition
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
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
    EDGE_LIST_GRAPH,
    EDGE_LISTS_NUM
};

/**
 * @class EdgeImpl
 * @brief  Representation of GraphImpl edge
 *
 * @details
 * EdgeImpl class implements basic concept of an edge of the directed graph. Every edge has two adjacent nodes.
 * Use pred() and succ() routines to get them. An edge is a member of 3 lists: edge list in graph,
 * pred list in succ node and succ list in pred node. To traverse these lists use nextEdge(),
 * nextPred() and  nextSucc() routines. Also for debug purposes all edges in a graph
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * An edge can be @ref Marked "marked" and @ref Numbered "numbered". @ref Mark "Markers" and
 * @ref Nums "numerations" are managed by @ref GraphImpl "GraphImpl". Note that @ref NodeImpl "nodes" can be marked with the
 * same marker or numbered in the same numeration.
 * Also for debug purposes all nodes in a GraphImpl
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
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
                       EDGE_LISTS_NUM >, // Lists number                      
    public Marked,
    public Numbered
{
public:
    /** Get edge's unique ID */
    inline GraphUid id() const;

    /** Get edge's graph */
    inline GraphImpl * graph() const;

    /** 
     *  Destructor.
     *  Delete edge from list in graph.
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

    /** Return next edge of the graph */
    inline EdgeImpl* nextEdge() const;

    /** Return next edge of the same node in given direction  */
    inline EdgeImpl* nextEdgeInDir( GraphDir dir) const;
    inline EdgeImpl* nextSucc() const;/**< Next successor */
    inline EdgeImpl* nextPred() const;/**< Next predecessor */
    
    /** Print edge in dot fomat to stdout */
    virtual void debugPrint();
protected:
    /** Constructors are made private, only nodes and GraphImpl can create edges */
    inline EdgeImpl( NodeImpl *_pred, NodeImpl* _succ);

    /**
     * Detach edge from a node.
     * Made private as it is low-level routine needed for implementation of edge-node relationship
     */
    inline void detachFromNode( GraphDir dir);

    /**
     * Remove myself from graph's list of edges
     */
    inline void detachFromGraph()
    {
        detach( EDGE_LIST_GRAPH);
    }
private:
    /** NodeImpl checking routine */
    bool checkNodes( NodeImpl* _pred, NodeImpl* _succ);
    
    inline void setId( GraphUid uid);   /**< Set edge's unique ID           */
    inline void setGraph( GraphImpl *g);/**< Set edge's pointer to graph */
    
    /** Graph part */
    GraphUid uid; //Unique ID
    GraphImpl * graph_p; //Graph

    /** Nodes */
    NodeImpl *nodes[ GRAPH_DIRS_NUM]; //Adjacent nodes

    /** GraphImpl should have access to EdgeImpl's members */
    friend class GraphImpl;
    /** NodeImpl should have access to EdgeImpl's members */
    friend class NodeImpl;

};

/**
 * @class Edge
 * @brief Template for making user object an edge of a directed graph, parameterized by the graph, node and edge types.
 * @ingroup GraphBase
 * @details
 *
 * The user can create his custom edge class by subclassing Node template instance in the following manner:
@code
class MyEdge: public Edge< MyGraph, MyNode, MyEdge>
{
    int dummy; //User data
public:
    MyEdge( MyGraph *g, MyNode *pred, MyNode *succ): 
        Edge< MyGraph, MyNode, MyEdge>( g, pred, succ) {} //Constructor must be implemented
};
@endcode
 * Edge class implements basic concept of graph edge. Every edge has two adjacent nodes.
 * Use pred() and succ() routines to get them. An edge is a member of 3 lists: edge list in graph,
 * pred list in succ node and succ list in pred node. To traverse these lists use nextEdge(),
 * nextPred() and  nextSucc() routines. Also for the debug purposes all edges in a graph
 * have unique id, which can be usefull for printing to the console or setting the breakpoint conditions.
 *
 * An edge can be @ref Marked "marked" and @ref Numbered "numbered". @ref Mark "Markers" and
 * @ref Nums "numerations" are managed by the @ref GraphImpl "graph". Note that @ref Node "nodes" can
 * be marked with the same marker or numbered in the same numeration.
 *
 * Also for the debug purposes all edge in a graph
 * have unique id, which can be usefull for printing to the console or setting the breakpoint conditions.
 *
 * Edges reside in memory pool that is controlled by Graph. Operator new can't be called
 * directly. Edges can be only created by calling Graph::newEdge().
 *
 * Every edge have associated QDomElement for XML export support. The updateElement() routine should be called before
 * export to get element in sync with edge's properties.
 *
 * @sa GraphImpl
 * @sa NodeImpl
 * @sa Mark
 * @sa Nums
 */
template < class G, class N, class E> class Edge: 
    public EdgeImpl,
    public PoolObj<E, UseCustomFixedPool>
{
public:    

    Edge( N* pred, N* succ): EdgeImpl( pred, succ){};
    ~Edge(){};

    /** Insert node on this edge */
    inline N *insertNode();

    /** Get node in specified direction  */
    inline N *node( GraphDir dir) const;
    inline N *pred() const; /**< Get predecessor node of edge */
    inline N *succ() const; /**< Get successor node of edge   */

    /** Return next edge of the graph */
    inline E* nextEdge() const;

    /** Return next edge of the same node in given direction  */
    inline E* nextEdgeInDir( GraphDir dir) const;
    inline E* nextSucc() const; /**< Next successor */
    inline E* nextPred() const; /**< Next predecessor */
};

#endif
