/**
 * @file: GraphImpl.h 
 * GraphImpl class definition/implementation.
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef GRAPH_H
#define GRAPH_H

/**
 * @class GraphImpl
 * @brief Basic representation of GraphImpl
 * @ingroup GraphBase
 *
 * @par 
 * The GraphImpl class represents GraphImpl as a whole. As one can expect GraphImpl has @ref NodeImpl "nodes"
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
 * @code
 
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
 * @par
 * The GraphImpl is also owner of memory allocated for its nodes and edges. This is
 * implemented via @ref FixedPool "memory pools" with records of fixed size. Nodes and
 * Edges should be created through newNode() and newEdge() routines. They can be deleted by
 * deleteNode() and deleteEdge() routines. Do not use operators new/delete for GraphImpl's 
 * nodes and edges.
 * 
 * @par
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
 * @par Deriving classes from GraphImpl
 * To make a GraphImpl-like data structure one can use GraphImpl as a base class. Most likely
 * it will be also necessary to implement two more classes to make a useful implementation.
 * These two class should be derived from NodeImpl and EdgeImpl to represent information  
 * associated with nodes and edges. You can see an example of such inheritance in AGraph,
 * ANode and AEdge classes.
 *
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

    /** Create new node in GraphImpl */
    inline NodeImpl * newNode();

    /**
     * Create edge between two nodes.
     * We do not support creation of edge with undefined endpoints
     */
    inline EdgeImpl * newEdge( NodeImpl * pred, NodeImpl * succ);
    
    /** Remove node from node list of GraphImpl */
    inline void detachNode( NodeImpl* node);

    /** Remove edge from edge list of GraphImpl */
    inline void detachEdge( EdgeImpl * edge);

    /** Return number of nodes in GraphImpl */
    inline GraphNum nodeCount() const;

    /** Return number of edges in GraphImpl */
    inline GraphNum edgeCount() const;
    
    /** Get first edge */
    inline EdgeImpl* firstEdge();

    /** Get first node */
    inline NodeImpl* firstNode();
    
    /** Print GraphImpl to stdout in DOT format */
    virtual void debugPrint();

private:
    /**
     * Implementation of node creation
     */
    inline NodeImpl * newNodeImpl( GraphUid id);
    /**
     * Implementation of edge creation
     */
    inline EdgeImpl * newEdgeImpl( NodeImpl * pred, NodeImpl * succ);

    /** Clear unused markers from marked objects */
    void clearMarkersInObjects();

    /** Clear unused numerations from numbered objects */
    void clearNumerationsInObjects();

    /** First node */
    NodeImpl* first_node;
    /** Number of nodes */
    GraphNum node_num;
    
    /** 
     *  Id of next node. Incremented each time you create a node,
     *  needed for nodes to have unique id. In DEBUG mode node id is not reused.
     */
    GraphUid node_next_id;

    /* List of edges and its iterator */
    EdgeImpl* first_edge;
    GraphNum edge_num;
    
    /** Id of next edge. Incremented each time you create an edge,
     *  needed for edges to have unique id. In DEBUG mode edge id is not reused.
     */
    GraphUid edge_next_id;
};


template < class G, class N, class E> class Graph: public GraphImpl
{
public:

    /** Deafult constructor */
    Graph();

    /** Node creation routine. Uses node's default constructor */
    inline N * createNode( GraphUid _id);
    /** Edge creation routine. Uses edge's default constructor */
    inline E * createEdge( GraphUid _id, N *_pred, N* _succ);

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


template < class G, class N, class E> Graph< G, N, E>::Graph()
{

};

/** NodeImpl creation overload */
template < class G, class N, class E> 
    N * Graph< G, N, E>::createNode( GraphUid _id)
{
    return new ( &node_pool) ANode( this, _id);
}
/** EdgeImpl creation overload */
template < class G, class N, class E> 
    E * Graph< G, N, E>::createEdge( GraphUid _id, N *_pred, N* _succ)
{
    return new ( &edge_pool) AEdge( this, _id, static_cast<ANode*>( _pred), static_cast< ANode *>(_succ));
} 
#endif /* GRAPH_H*/
