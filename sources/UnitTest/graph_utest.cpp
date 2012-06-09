/**
 * @file: GraphImpl_utest.cpp 
 * Implementation of testing of GraphImpl library
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utest_impl.h"
#include <vector>

 /**
  * TODO: Check GraphImpl's data structures being consistent with node and edge functionality
  */
bool uTestGraphImplOwn()
{
    {
        AGraph GraphImpl( true);
        ANode *dummy = GraphImpl.newNode();
        GraphImpl.deleteNode( dummy);
        ANode *pred = GraphImpl.newNode();
        ANode *succ = GraphImpl.newNode();
        AEdge *edge = GraphImpl.newEdge( pred, succ);
        
        /** Check node insertion */
        ANode *new_node = edge->insertNode();
        AEdge *edge2 = new_node->firstSucc();
        assert( areEqP( new_node->firstPred(), pred->firstSucc())); 
        assert( areEqP( new_node->firstSucc(), succ->firstPred()));
        assert( areEqP( edge->pred(), pred));
        assert( areEqP( pred->firstSucc(), edge));
        assert( areEqP( edge->succ(), new_node));
        assert( areEqP( new_node->firstPred(), edge));
        assert( areEqP( edge2->pred(), new_node));
        assert( areEqP( edge2->succ(), succ));
        assert( areEqP( succ->firstPred(), edge2));
    }

    /** Test iterators */
    {
        AGraph GraphImpl( true);
        ANode *node1 = GraphImpl.newNode();
        ANode *node2 = GraphImpl.newNode();
        ANode *node3 = GraphImpl.newNode();
        AEdge *edge1 = GraphImpl.newEdge( node1, node2);
        AEdge *edge2 = GraphImpl.newEdge( node2, node3);
        
        for ( NodeImpl::Succ succ_iter = node2->succsBegin(),
                         succ_iter_end = node2->succsEnd();
              succ_iter != succ_iter_end;
              ++succ_iter )
        {
            assert( areEqP( *succ_iter, edge2));
        }
        for ( NodeImpl::Pred pred_iter = node2->predsBegin(),
                         pred_iter_end = node2->predsEnd();
              pred_iter != pred_iter_end;
              pred_iter++ )
        {
            assert( areEqP( *pred_iter, edge1));
        }
        NodeImpl::EdgeIter edge_iter = node2->edgesBegin();
        NodeImpl::EdgeIter edge_iter_end = node2->edgesEnd();
        assert( edge_iter != edge_iter_end);
        assert( areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            assert( areEqP( edge_iter.node(), edge1->pred()));
            assert( areEqP( edge_iter.node(), node1));
        } else
        {
            assert( areEqP( edge_iter.node(), edge2->succ()));
            assert( areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        assert( edge_iter != edge_iter_end);
        assert( areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            assert( areEqP( edge_iter.node(), edge1->pred()));
            assert( areEqP( edge_iter.node(), node1));
        } else
        {
            assert( areEqP( edge_iter.node(), edge2->succ()));
            assert( areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        assert( edge_iter == edge_iter_end);
    }

    return true;
}
/**
 * TODO: Check consistency of NodeImpl and EdgeImpl classes interoperation
 */
bool uTestNodeEdge()
{
    /**
     * TODO: Check that node and edge remain correct after basic edge/node creation/deletion
     */
    /**
     * 
     */
    return true;
}

/**
 * Check marker functionality
 */
bool uTestMarkers()
{
    AGraph GraphImpl( true);
    ANode *dummy = GraphImpl.newNode();
    GraphImpl.deleteNode( dummy);
    ANode *pred = GraphImpl.newNode();
    ANode *succ = GraphImpl.newNode();
    AEdge *edge = GraphImpl.newEdge( pred, succ);
    Marker m = GraphImpl.newMarker();
    Marker m2 = GraphImpl.newMarker();

    Marker m_array[ MAX_GraphImpl_MARKERS];
    
    assert( !pred->isMarked( m));
    assert( !succ->isMarked( m));
    assert( !edge->isMarked( m));
    assert( !pred->isMarked( m2));
    
    pred->mark( m);
    succ->mark( m);
    edge->mark( m);
    edge->mark( m2);

    assert( pred->isMarked( m));
    assert( succ->isMarked( m));
    assert( edge->isMarked( m));
    assert( edge->isMarked( m2));
    edge->unmark( m);

    /** Check that different markers have different behaviour */
    assert( edge->isMarked( m2));
    assert( !edge->isMarked( m));
    
    GraphImpl.freeMarker( m);
    GraphImpl.freeMarker( m2);
    
    for ( MarkerIndex i = 0; i < MAX_GraphImpl_MARKERS; i++)
    {
        m_array [ i] = GraphImpl.newMarker();
    }
    for ( MarkerIndex i = 0; i < MAX_GraphImpl_MARKERS; i++)
    {
        GraphImpl.freeMarker( m_array[ i]);
    }
    m = GraphImpl.newMarker();
    GraphImpl.freeMarker( m);
    
    ANode *n;
    for (  n = GraphImpl.firstNode(); isNotNullP( n);)
    {
        ANode *tmp = n;
        n = n->nextNode();
        GraphImpl.deleteNode( tmp);
    }
    return true;
}

/**
 * Check marker functionality
 */
static bool uTestNumerations()
{
    /** 
     * Every class that can be a numerations manager should implement
     * the routine for clearing numerations in objects
     */
    class NumMgrInst: public NumManager
    {
        /** Implementation of clearing - empty TODO: implement and test it */
        void clearNumerationsInObjects()
        {
        
        }
    };
    /**
     * Check correct error reporting
     *  1. Too many numerations
     */
    NumMgrInst mgr1;
    try
    {
        for ( int i = 0; i < MAX_NUMERATIONS + 1; i++)
        {
            mgr1.newNum();
        }
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        assert( error == NUM_ERROR_OUT_OF_INDEXES);
    }

    /** 2. Too big number */
    NumMgrInst mgr2;
    Numeration num2 = mgr2.newNum();
    Numbered obj2; 
    try
    {
        obj2.setNumber( num2, -1);
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        assert( error == NUM_ERROR_NUMBER_OUT_OF_RANGE);
    }
    mgr2.freeNum( num2);

    /** 3. Functional testing */
    NumMgrInst mgr;
    for ( int i = 0; i < MAX_NUMERATIONS + 2; i++)
    {
        Numeration n = mgr.newNum();
        mgr.freeNum( n);
    } 
    Numeration num = mgr.newNum();
    Numeration num_unused = mgr.newNum();
    Numbered obj; 
    assert( obj.number( num) == NUMBER_NO_NUM);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.setNumber( num, 1);
    assert( obj.isNumbered( num));
    assert( obj.number( num) == 1);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.unNumber( num);
    assert( obj.number( num) == NUMBER_NO_NUM);
    assert( obj.number( num_unused) == NUMBER_NO_NUM);    
    return true;
}

/**
 * Create GraphImpl and save it to XML
 */
bool uTestSave()
{
    AGraph GraphImpl( true);

    /** 
     *  Check basic operation of GraphImpl library
     */
    std::vector<ANode *> nodes;

    /** Create nodes and edges */
    for ( int i =0; i<20; i++)
    {
        nodes.push_back( GraphImpl.newNode());
        if ( i > 0)
        {
            GraphImpl.newEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            GraphImpl.newEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    GraphImpl.newEdge( nodes[ 8], nodes[ 4]);
    GraphImpl.deleteNode( nodes[ 8]);
    GraphImpl.debugPrint();
    return true;
}

/**
 * Unit tests for GraphImpl library
 */
bool uTestGraphImpl()
{
    /**
     * Check GraphImpl's data structures consistency
     */
     if ( !uTestGraphImplOwn())
         return false;
    /**
     * Check node-edge consistency
     */
    if ( !uTestNodeEdge())
         return false;

    /**
     * Check numerations 
     */
    if ( !uTestNumerations())
        return false;

    /**
     * Check markers
     */
    if ( !uTestMarkers())
        return false;

//    assert<Error>( 0);
    return true;
}