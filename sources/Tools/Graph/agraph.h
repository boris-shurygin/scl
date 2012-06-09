/**
 * @file: aGraphImpl.h 
 * Abstract GraphImpl for testing of GraphImpl's properties and usage model.
 *
 * @defgroup AGr Test GraphImpl
 *
 * @ingroup GraphBase
 * AGraph, ANode and AEdge classes present mimnimal code
 * that you need to write to employ GraphImpl Library functionality.
 * AGraph classes differ from base in only one member of ( int) type
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef AGraph_H
#define AGraph_H

/* Predeclarations */
class ANode;
class AEdge;
class AGraph;

/**
 * Abstract node
 *
 * @ingroup AGr
 */
class ANode: public NodeImpl
{
    int dummy;
    /** We can't create nodes separately, do it through newNode method of GraphImpl */
    ANode( AGraph *GraphImpl_p, GraphUid _id);
    friend class AGraph;
public:
        /** Get next GraphImpl's node */
    inline ANode* nextNode()
    {
        return static_cast< ANode*>( NodeImpl::nextNode());
    }
    /** Get prev GraphImpl's node */
    inline ANode* prevNode()
    {
        return static_cast< ANode*>( NodeImpl::prevNode());
    }
    /** EdgeImpl connection reimplementation */
    inline void AddEdgeInDir( AEdge *edge, GraphDir dir);
    /** Add predecessor */
    inline void AddPred( AEdge *edge);
    /** Add successor */
    inline void AddSucc( AEdge *edge);
    /** Get first edge in given direction */
    inline AEdge* firstEdgeInDir( GraphDir dir);
    /** Get first successor */
    inline AEdge* firstSucc();
    /** Get first predecessor */
    inline AEdge* firstPred();

};

/**
 * Abstract edge
 *
 * @ingroup AGr
 */
class AEdge: public EdgeImpl
{
    int dummy;

    /** Constructors are made private, only nodes and GraphImpl can create edges */
    AEdge( AGraph *GraphImpl_p, GraphUid _id, ANode *_pred, ANode* _succ);
        
    friend class AGraph;
    friend class ANode;
public:
        /** Get node in given direction */
    inline ANode *node( GraphDir dir) const
    {
        return static_cast< ANode *>( EdgeImpl::node( dir));
    }
    /** Get predecessor */
    inline ANode *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /** Get successor */
    inline ANode *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }  
    /** insert node on this edge */
    virtual ANode *insertNode()
    {
        return static_cast< ANode *>( EdgeImpl::insertNode());
    }
    /** Next edge in GraphImpl's list */
    inline AEdge* nextEdge()
    {
        return static_cast< AEdge *>( EdgeImpl::nextEdge());
    }
    /** Next edge in give direction */
    inline AEdge* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< AEdge *>( EdgeImpl::nextEdgeInDir( dir));
    }
    /** Next successor */
    inline AEdge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Next predecessor */
    inline AEdge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    } 
};

/**
 * Testing-purpose GraphImpl
 *
 * @ingroup AGr
 */
class AGraph: public Graph< AGraph, ANode, AEdge>
{
    int dummy; //Dummy class member


    /** NodeImpl creation overload */
    NodeImpl * createNode( GraphUid _id);
    /** EdgeImpl creation overload */
    EdgeImpl * createEdge( GraphUid _id, NodeImpl *_pred, NodeImpl* _succ);

    public:
            
    /** New GraphImplical node */
    ANode* newNode()
    {
        return static_cast< ANode*>( GraphImpl::newNode());
    }

    /** New GraphImplical edge */
    AEdge* newEdge( ANode* pred, ANode* succ)
    {
        return static_cast< AEdge *>( GraphImpl::newEdge( pred, succ));
    }

    /** Get GraphImpl's first edge */
    inline AEdge* firstEdge() 
    {
        return static_cast< AEdge *>( GraphImpl::firstEdge());
    }
    /** Get GraphImpl's first node */
    inline ANode* firstNode()
    {
        return static_cast< ANode *>( GraphImpl::firstNode());
    }
    
    /** Constructor */
    AGraph( bool create_pools)
    {

    }
};

/** NodeImpl constructor */
inline ANode::ANode( AGraph *GraphImpl_p, GraphUid _id):
        NodeImpl( GraphImpl_p, _id)
{

}

/** EdgeImpl constructor */
inline AEdge::AEdge( AGraph *GraphImpl_p, GraphUid _id, ANode *_pred, ANode* _succ):
        EdgeImpl( GraphImpl_p, _id, _pred, _succ)
{

}
        
/** Get first edge in given direction */
inline AEdge*
ANode::firstEdgeInDir( GraphDir dir)
{
    return static_cast< AEdge*>( NodeImpl::firstEdgeInDir( dir));
}
/** Get first successor */
inline AEdge*
ANode::firstSucc()
{
    return firstEdgeInDir( GRAPH_DIR_DOWN);
}
/** Get first predecessor */
inline AEdge*
ANode::firstPred()
{
    return firstEdgeInDir( GRAPH_DIR_UP);
}

/** EdgeImpl connection reimplementation */
inline void
ANode::AddEdgeInDir( AEdge *edge, GraphDir dir)
{
    NodeImpl::AddEdgeInDir( edge, dir);
}
/** Add predecessor */
inline void
ANode::AddPred( AEdge *edge)
{
    AddEdgeInDir( edge, GRAPH_DIR_UP);
}
/** Add successor */
inline void
ANode::AddSucc( AEdge *edge) 
{
    AddEdgeInDir( edge, GRAPH_DIR_DOWN);
}
#endif
