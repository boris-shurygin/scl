/**
 * @file: node.h 
 * NodeImpl class definition
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef NODE_H
#define NODE_H

#include "node_iter.h"

namespace Graph
{
    class NodeListTag{};
/**
 * @class NodeImpl
 * @brief Representation of GraphImpl node. 
 * 
 * @details
 * A graph node has two lists of edges which represent predecessors and successors. 
 * NodeImpl's predecessors and successors can be traversed by using three interfaces:
 * -# Get first edge in direction of interest via firstSucc(),firstPred() and then 
 *    use EdgeImpl's interface EdgeImpl::nextSucc(), EdgeImpl::nextPred()
 * -# Use iterators NodeImpl::Succ and NodeImpl::Pred wich are used for successor and predecessor traversal of edges. 
 *    Routines succsBegin(), succsEnd(), predsBegin() and predsEnd() are used for creating iterators.
 * -# EdgeIter can be used for iterating trough all node's adjacent edges without respect to 
 *    their direction.
 * 
 * @code
  // Traversing edges simple
  for ( EdgeImpl* e = firstPred();
        isNotNullP( e);
        e = e->nextPred())
  {
     ...
  }
  //Same using macro foreachPred
  EdgeImpl* e; 
  foreachPred( e, node)
  {
     ...
  }
  //Traversal via iterators
  for ( NodeImpl::Succ s = node->succsBegin(),
                   s_end = node->succsEnd();
        s != s_end;
        s++ )
  {
     ...
  }
  @endcode
 *
 * A node can be @ref Marked "marked" and @ref Numbered "numbered". @ref Mark "Markers" and
 * @ref Nums "numerations" are managed by @ref GraphImpl "GraphImpl". Note that @ref EdgeImpl "edges" can be marked with the
 * same marker or numbered in the same numeration.
 * 
 * All nodes in graph are linked in a list. Previous and next nodes can be obtained
 * through prevNode and nextNode routines. Also for debug purposes all nodes in a GraphImpl
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * Nodes have associated QDomElement for XML export support. The updateElement() routine should be called before 
 * export to get element in sync with node's properties.
 *
 * @sa GraphImpl
 * @sa EdgeImpl
 * @sa Mark
 * @sa Nums
 */
class NodeImpl: 
    public Marked, public Numbered,
    public ListItem< NodeListTag>
{
public:
    typedef ListItem< NodeListTag> NodeList;
    /**
     * @brief Destructor.
     * Destructs the node. Operator delete shouldn't be called directly.
     * Use GraphImpl::deleteNode for freeing memory and destruction
     */
    virtual ~NodeImpl();
    
    inline GraphUid id() const;  /**< Get node's unique ID           */
    inline GraphImpl * graph() const;/**< Get node's corresponding graph */
    inline NodeImpl* nextNode() const;     /**< Next node in graph's list      */
    inline NodeImpl* prevNode() const;     /**< Prev node in graph's list      */
    
    /** Add edge to node in specified direction */
    inline void AddEdgeInDir( EdgeImpl *edge, GraphDir dir);
    
    inline void AddPred( EdgeImpl *edge); /**< Add predecessor edge */
    inline void AddSucc( EdgeImpl *edge); /**< Add successor edge   */
 
    /** Get first edge in given direction */
    inline EdgeImpl* firstEdgeInDir( GraphDir dir) const;
    
    inline EdgeImpl* firstSucc() const; /**< Get first successor edge    */
    inline EdgeImpl* firstPred() const; /**< Get first predecessor edge  */
     
    /** Deletion of edge in specified direction */
    void deleteEdgeInDir( GraphDir dir, EdgeImpl* edge);

    inline void deletePred( EdgeImpl* edge); /**< Delete predecessor edge */
    inline void deleteSucc( EdgeImpl* edge); /**< Delete successor edge   */
    
    virtual void debugPrint(); /**< Print node in DOT format to stdout */

protected:
    /** We can't create nodes separately, do it through newNode method of graph */
    inline NodeImpl();
private:
    /** Detach this node from GraphImpl's node list */
    inline void detachFromGraph();
    
    inline void setId( GraphUid uid);   /**< Set node's unique ID           */
    inline void setGraph( GraphImpl *g);/**< Set node's pointer to graph */
    
    /* Connection with inclusive graph */
    GraphUid uid;       /**< Unique id        */
    GraphImpl * graph_p;/**< Pointer to graph */

    /** First edges in GraphImpl's directions */
    EdgeImpl *first_edge[ GRAPH_DIRS_NUM];

    /** GraphImpl class controls nodes */
    friend class GraphImpl; 
};


/**
 * @class Node
 * @brief Template for making user object a node of directed graph, parameterized by the graph, node and edge types
 * @ingroup GraphBase
 * @details
 * The user can create his custom node class by subclassing Node template instance in the following manner:
 @code
class MyNode: public Node< MyGraph, MyNode, MyEdge>
{
    int dummy; //user data
public:
    MyNode( MyGraph* g): 
        Node< MyGraph, MyNode, MyEdge>( g){};//A constructor with graph pointer parameter must be implemented
};
 @endcode
 *
 * A graph node has two lists of edges which represent predecessors and successors. 
 * NodeImpl's predecessors and successors can be traversed by using three interfaces:
 * -# Get first edge in direction of interest via firstSucc(),firstPred() and then 
 *    use EdgeImpl's interface EdgeImpl::nextSucc(), EdgeImpl::nextPred()
 * -# Use iterators NodeImpl::Succ and NodeImpl::Pred wich are used for successor and predecessor traversal of edges. 
 *    Routines succsBegin(), succsEnd(), predsBegin() and predsEnd() are used for creating iterators.
 * -# EdgeIter can be used for iterating trough all node's adjacent edges without respect to 
 *    their direction.
 * 
@code
  // Traversing edges simple
  for ( MyEdge* e = firstPred();
        isNotNullP( e);
        e = e->nextPred())
  {
     ...
  }
  //Same using macro foreachPred
  MyEdge* e; 
  foreachPred( e, node)
  {
     ...
  }
  //Traversal via iterators
  for ( MyNode::Succ s = node->succsBegin(),
                   s_end = node->succsEnd();
        s != s_end;
        s++ )
  {
     ...
  }
@endcode
 * A node can be @ref Marked "marked" and @ref Numbered "numbered". @ref Mark "Markers" and
 * @ref Nums "numerations" are managed by @ref GraphImpl "GraphImpl". Note that @ref EdgeImpl "edges" can be marked with the
 * same marker or numbered in the same numeration.
 * 
 * All nodes in GraphImpl are linked in a list. Previous and next nodes can be obtained
 * through prevNode and nextNode routines. Also for debug purposes all nodes in a GraphImpl
 * have unique id, which can be usefull for printing to console or setting breakpoint conditions.
 *
 * A node resides in memory pool that is controlled by Graph. Operator new can't be called 
 * directly. Nodes can be only created by newNode method of Graph class. 
 *
 * Nodes have associated QDomElement for XML export support. The updateElement() routine should be called before 
 * export to get element in sync with node's properties.
 * 
 * @sa ANode
 * @sa Graph
 * @sa Edge
 * @sa Mark
 * @sa Nums
 */
template < class G, class N, class E> class Node: 
    public NodeImpl,
    public PoolObj< N, UseCustomFixedPool>
{
public:
    virtual ~Node();

    inline G * graph()  const;/**< Get node's corresponding GraphImpl */
    inline N* nextNode() const;     /**< Next node in GraphImpl's list      */
    inline N* prevNode() const;     /**< Prev node in GraphImpl's list      */
    
    inline E* firstSucc() const; /**< Get first successor edge    */
    inline E* firstPred() const; /**< Get first predecessor edge  */

    /* Iterator types */
    typedef EdgeIterIface< N, E, SuccIterImpl> Succ;      /**< Iterator for successors       */
    typedef EdgeIterIface< N, E, PredIterImpl> Pred;      /**< Iterator for predecessors     */
    typedef EdgeIterIface< N, E, UnDirIterImpl> EdgeIter; /**< Undirected iterator for edges */
     
    inline Succ succsBegin(); /**< Create iterator for first succ       */
    inline Succ succsEnd();   /**< Create iterator pointing to succ end */
    
    inline Pred predsBegin(); /**< Create iterator for first succ       */
    inline Pred predsEnd();   /**< Create iterator pointing to succ end */

    inline EdgeIter edgesBegin(); /**< Create iterator for first succ       */
    inline EdgeIter edgesEnd();   /**< Create iterator pointing to succ end */

};

} // namespace Graph    
#endif /* NODE_H */
