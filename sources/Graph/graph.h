/**
 * @file: GraphImpl.h 
 * GraphImpl class definition/implementation.
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H

/**
 * @class GraphImpl
 * @brief Basic representation of graph
 *
 * @details
 * The GraphImpl class represents graph as a whole. As one can expect GraphImpl has @ref NodeImpl "nodes"
 * and @ref EdgeImpl edges which impement the directed GraphImpl data structure. For traversing nodes
 * and edges they are linked in two lists. One can traverse these lists using firstNode() and firstEdge()
 * routines with calling NodeImpl::nextNode() EdgeImpl::nextEdge() in a loop.
 * Example:
 * @code
 //Traversing nodes
 for ( NodeImpl *n = firstNode(); isNotNullP( n); n = n->nextNode())
 {
     ...
 }
 //Traversing edges
 for ( NodeImpl *e = firstEdge(); isNotNullP( e); e = e->nextEdge())
 {
     ...
 }
 @endcode
 * The same result can be achieved using macros 
 * 
 @code
 // GraphImpl *g;
 // Traversing nodes
 NodeImpl *n;
 foreachNode( n, g)
 {
    ...
 }
 
 // Traversing edges
 EdgeImpl *e;
 foreachEdge( e, g)
 {
    ...
 }
 @endcode
 *
 * GraphImpl is also manager of @ref Mark "markers" and @ref Nums "numerations" for nodes and edges.
 * New @ref Marker "marker" can be obtained by newMarker() routine. New @ref Numeration "numeration" is
 * created by newNum().
 * Example:
@code
 //GraphImpl *GraphImpl; 
 NodeImpl *n;
 Marker m = GraphImpl->newMarker();
 Numeration num = GraphImpl->newNum();
    
 GraphNum i = 0; //unsigned int 32
 //Mark nodes without predecessors
 foreachNode( n, g)
 {
    if ( isNullP( n->firstPred()))
    {
        n->mark( m);
        n->setNumber( num, i++);
    }
 }
 ...

 //Checking markers and numbers
 foreachNode( n, g)
 {
     if ( n->isMarked( m) && n->number( num) > 10)
     {
        ...
     }
 }
 GraphImpl->freeMarker( m);
 GraphImpl->freeNum( num);
@endcode
 *
 * Deriving classes from GraphImpl
 * To make a GraphImpl-like data structure one can use GraphImpl as a base class. Most likely
 * it will be also necessary to implement two more classes to make a useful implementation.
 * These two class should be derived from NodeImpl and EdgeImpl to represent information  
 * associated with nodes and edges. You can see an example of such inheritance in AGraph,
 * ANode and AEdge classes.
 *
 * @sa Graph
 * @sa NodeImpl
 * @sa EdgeImpl
 * @sa Mark
 * @sa Nums
 * @sa AGraph
 */
class GraphImpl: public MarkerManager, public NumManager
{
public:
    /** Default constructor */
    GraphImpl();
    
    /** Destructor */
    virtual ~GraphImpl();

    /** Remove node from node list of GraphImpl */
    inline void detachNode( NodeImpl* node);

    /** Remove edge from edge list of GraphImpl */
    inline void detachEdge( EdgeImpl * edge);

    /** Return number of nodes in GraphImpl */
    inline GraphNum numNodes() const;

    /** Return number of edges in GraphImpl */
    inline GraphNum numEdges() const;
    
    /** Get first edge */
    inline EdgeImpl* firstEdge();

    /** Get first node */
    inline NodeImpl* firstNode();
    
    /** Print GraphImpl to stdout in DOT format */
    virtual void debugPrint();

    /** Add existing node */
    inline void addNode( NodeImpl *n);
    
    /** Add existing edge */
    inline void addEdge( EdgeImpl *e);
private:
    
    /** Clear unused markers from marked objects */
    virtual void clearMarkersInObjects();

    /** Clear unused numerations from numbered objects */
    virtual void clearNumerationsInObjects();

    /** First node */
    NodeImpl* first_node;
    /** Number of nodes */
    GraphNum node_num;
    /** First edge pointer */
    EdgeImpl* first_edge;
    /** Number of edge in graph */
    GraphNum edge_num; 

protected:
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphUid node_next_id;

    /**
     * Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphUid edge_next_id;

    friend EdgeImpl::EdgeImpl( GraphImpl *_graph_p, NodeImpl *_pred, NodeImpl* _succ);
    friend NodeImpl::NodeImpl( GraphImpl *_graph_p);
};

/**
 * @class Graph
 * @brief Conveniece template for graph creation.
 * @ingroup GraphBase
 *
 * @details
 * The Graph template is used for easy turning an object into a graph. It automates inheritance from GraphImpl 
 * and makes all the necessary routines. The usage should look like:
 @code
 class MyGraph: public Graph< MyGraph, MyNode, MyEdge>
 {
    // user data
 };
 @endcode
 * 
 * The Graph class represents graph as a whole. As one can expect the graph has @ref Node "nodes"
 * and @ref Edge edges which impement the directed graph data structure. For traversing nodes
 * and edges they are linked in two lists. One can traverse these lists using firstNode() and firstEdge()
 * routines with calling Node::nextNode() Edge::nextEdge() in a loop.
 * Example:
 * @code
 
 MyGraph *g = ...;//came from above

 //Traversing nodes
 for ( MyNode *n = g->firstNode(); isNotNullP( n); n = n->nextNode())
 {
     ...
 }
 //Traversing edges
 for ( MyEdge *e = g->firstEdge(); isNotNullP( e); e = e->nextEdge())
 {
     ...
 }
 @endcode
 * The same result can be achieved using macros 
 * @code
 
 // MyGraph *g;
 // Traversing nodes
 MyNode *n;
 foreachNode( n, g)
 {
    ...
 }
 
 // Traversing edges
 MyEdge *e;
 foreachEdge( e, g)
 {
    ...
 }
 @endcode
 *
 * The Graph is also the owner of the memory allocated for its nodes and edges. This is
 * implemented via @ref FixedPool "memory pools" with the records of fixed size. The nodes and
 * edges should be created through the newNode() and newEdge() routines. They can be deleted by
 * the deleteNode() and deleteEdge() routines. 
 * IMPORTANT: Do not use operators new/delete for graph's  nodes and edges.
 * 
 * The Graph is also the manager of @ref Mark "markers" and @ref Nums "numerations" for the nodes and edges.
 * New @ref Marker "marker" can be obtained by the newMarker() routine. The new @ref Numeration "numeration" is
 * created by the newNum() routine.
 * Example:
@code
 MyGraph *g = ...;//From above
 MyNode *n;
 Marker m = g->newMarker();
 Numeration num = g->newNum();
    
 GraphNum i = 0; //unsigned int 32
 //Mark nodes without predecessors
 foreachNode( n, g)
 {
    if ( isNullP( n->firstPred()))
    {
        n->mark( m);
        n->setNumber( num, i++);
    }
 }
 ...

 //Checking markers and numbers
 foreachNode( n, g)
 {
     if ( n->isMarked( m) && n->number( num) > 10)
     {
        ...
     }
 }
 g->freeMarker( m);
 g->freeNum( num);
@endcode
 *
 * @sa Node
 * @sa Edge
 * @sa AGraph
 * @sa GraphImpl
 */
template < class G, class N, class E> class Graph: public GraphImpl
{
public:

    /** Deafult constructor */
    Graph();

    /** Destructor */
    ~Graph();

    /** Delete node from memory pool */
    inline void deleteNode( N *n);
    
    /** Delete edge from memory pool */
    inline void deleteEdge( E *e);

    /** Create new node in GraphImpl */
    inline N *newNode();

    /** Create edge between two nodes */
    inline E *newEdge( N *pred, N *succ);

    /** Get first edge */
    inline E* firstEdge();

    /** Get first node */
    inline N* firstNode();
private:
    /** Get pool of nodes */
    inline TypedPool<N> &nodePool() const;
    /** Get pool of edges */
    inline TypedPool<E> &edgePool() const;

    /** Memory pool for nodes */
    TypedPool<N> node_pool;
    /** Memory pool for edges */
    TypedPool<E> edge_pool;
};

#endif /* GRAPH_H*/
