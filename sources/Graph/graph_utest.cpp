/**
 * @file: GRAPH_utest.cpp 
 * Implementation of testing of Graph library
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "graph_iface.h"

static const int GRAPH_TEST_NUM_NODES = 100000;

 /**
  * TODO: Check Graph's data structures being consistent with node and edge functionality
  */
bool uTestGraphImplOwn( UnitTest* utest_p)
{
    {
        AGraph graph;
        ANode *dummy = graph.newNode();
        graph.deleteNode( dummy);
        ANode *pred = graph.newNode();
        ANode *succ = graph.newNode();
        AEdge *edge = graph.newEdge( pred, succ);
        
        /** Check node insertion */
        ANode *new_node = edge->insertNode();
        AEdge *edge2 = new_node->firstSucc();
        UTEST_CHECK( utest_p, areEqP( new_node->graph(), &graph)); 
        UTEST_CHECK( utest_p, areEqP( new_node->firstPred(), pred->firstSucc())); 
        UTEST_CHECK( utest_p, areEqP( new_node->firstSucc(), succ->firstPred()));
        UTEST_CHECK( utest_p, areEqP( edge->pred(), pred));
        UTEST_CHECK( utest_p, areEqP( pred->firstSucc(), edge));
        UTEST_CHECK( utest_p, areEqP( edge->succ(), new_node));
        UTEST_CHECK( utest_p, areEqP( new_node->firstPred(), edge));
        UTEST_CHECK( utest_p, areEqP( edge2->pred(), new_node));
        UTEST_CHECK( utest_p, areEqP( edge2->succ(), succ));
        UTEST_CHECK( utest_p, areEqP( succ->firstPred(), edge2));
    }

    /** Test iterators */
    {
        AGraph graph;
        ANode *node1 = graph.newNode();
        ANode *node2 = graph.newNode();
        ANode *node3 = graph.newNode();
        AEdge *edge1 = graph.newEdge( node1, node2);
        AEdge *edge2 = graph.newEdge( node2, node3);
        
        for ( ANode::Succ succ_iter = node2->succsBegin(),
                         succ_iter_end = node2->succsEnd();
              succ_iter != succ_iter_end;
              ++succ_iter )
        {
            UTEST_CHECK( utest_p, areEqP( *succ_iter, edge2));
        }
        for ( ANode::Pred pred_iter = node2->predsBegin(),
                         pred_iter_end = node2->predsEnd();
              pred_iter != pred_iter_end;
              pred_iter++ )
        {
            UTEST_CHECK( utest_p, areEqP( *pred_iter, edge1));
        }
        ANode::EdgeIter edge_iter = node2->edgesBegin();
        ANode::EdgeIter edge_iter_end = node2->edgesEnd();
        UTEST_CHECK( utest_p, edge_iter != edge_iter_end);
        UTEST_CHECK( utest_p, areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), edge1->pred()));
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), node1));
        } else
        {
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), edge2->succ()));
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        UTEST_CHECK( utest_p, edge_iter != edge_iter_end);
        UTEST_CHECK( utest_p, areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), edge1->pred()));
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), node1));
        } else
        {
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), edge2->succ()));
            UTEST_CHECK( utest_p, areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        UTEST_CHECK( utest_p, edge_iter == edge_iter_end);
    }
    return true;
}



struct DummyNodeProps
{
    int i;
};

struct DummyEdgeProps
{
    int i;
};


/**
 * TODO: Check consistency of NodeImpl and EdgeImpl classes interoperation
 */
bool uTestNodeEdge( UnitTest* utest_p)
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
bool uTestMarkers( UnitTest* utest_p)
{
    AGraph graph;
    ANode *dummy = graph.newNode();
    graph.deleteNode( dummy);
    ANode *pred = graph.newNode();
    ANode *succ = graph.newNode();
    AEdge *edge = graph.newEdge( pred, succ);
    Marker m = graph.newMarker();
    Marker m2 = graph.newMarker();

    Marker m_array[ MAX_GRAPH_MARKERS];
    
    UTEST_CHECK( utest_p, !pred->isMarked( m));
    UTEST_CHECK( utest_p, !succ->isMarked( m));
    UTEST_CHECK( utest_p, !edge->isMarked( m));
    UTEST_CHECK( utest_p, !pred->isMarked( m2));
    
    pred->mark( m);
    succ->mark( m);
    edge->mark( m);
    edge->mark( m2);

    UTEST_CHECK( utest_p, pred->isMarked( m));
    UTEST_CHECK( utest_p, succ->isMarked( m));
    UTEST_CHECK( utest_p, edge->isMarked( m));
    UTEST_CHECK( utest_p, edge->isMarked( m2));
    edge->unmark( m);

    /** Check that different markers have different behaviour */
    UTEST_CHECK( utest_p, edge->isMarked( m2));
    UTEST_CHECK( utest_p, !edge->isMarked( m));
    
    graph.freeMarker( m);
    graph.freeMarker( m2);
    
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        m_array [ i] = graph.newMarker();
    }
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        graph.freeMarker( m_array[ i]);
    }
    m = graph.newMarker();
    graph.freeMarker( m);
    
    ANode *n;
    for (  n = graph.firstNode(); isNotNullP( n);)
    {
        ANode *tmp = n;
        n = n->nextNode();
        graph.deleteNode( tmp);
    }
    return true;
}

/**
 * Check marker functionality
 */
static bool uTestNumerations( UnitTest* utest_p)
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
        UTEST_CHECK( utest_p, error == NUM_ERROR_OUT_OF_INDEXES);
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
        UTEST_CHECK( utest_p, error == NUM_ERROR_NUMBER_OUT_OF_RANGE);
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
    UTEST_CHECK( utest_p, obj.number( num) == NUMBER_NO_NUM);
    UTEST_CHECK( utest_p, obj.number( num_unused) == NUMBER_NO_NUM);
    obj.setNumber( num, 1);
    UTEST_CHECK( utest_p, obj.isNumbered( num));
    UTEST_CHECK( utest_p, obj.number( num) == 1);
    UTEST_CHECK( utest_p, obj.number( num_unused) == NUMBER_NO_NUM);
    obj.unNumber( num);
    UTEST_CHECK( utest_p, obj.number( num) == NUMBER_NO_NUM);
    UTEST_CHECK( utest_p, obj.number( num_unused) == NUMBER_NO_NUM);    
    return true;
}

#if 0
/**
 * Create Graph and save it to XML
 */
bool uTestSave()
{
    AGraph graph;

    /** 
     *  Check basic operation of graph library
     */
    std::vector<ANode *> nodes;

    /** Create nodes and edges */
    for ( int i =0; i<20; i++)
    {
        nodes[i] = graph.newNode();
        if ( i > 0)
        {
            graph.newEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.newEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    graph.newEdge( nodes[ 8], nodes[ 4]);
    graph.deleteNode( nodes[ 8]);
    graph.debugPrint();
}

#endif

#if 1
/**
 * Unit tests for Graph library
 */
bool uTestGraph( UnitTest* utest_p)
{
    /**
     * Check Graph's data structures consistency
     */
    uTestGraphImplOwn( utest_p);
    /**
     * Check node-edge consistency
     */
    uTestNodeEdge( utest_p);
    
    uTestNumerations( utest_p);

    uTestMarkers( utest_p);

    return utest_p->result();
}
#endif

