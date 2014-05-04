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
class ANode: public Node< AGraph, ANode, AEdge>
{
public:
    int dummy;
};

/**
 * Abstract edge
 *
 * @ingroup AGr
 */
class AEdge: public Edge< AGraph, ANode, AEdge>
{
    int dummy;

public:
    
    AEdge( ANode *pred, ANode *succ): Edge< AGraph, ANode, AEdge>( pred, succ)
    {
        
    }
};

/**
 * Testing-purpose graph
 *
 * @ingroup AGr
 */
class AGraph: public Graph< AGraph, ANode, AEdge>
{
    int dummy; //Dummy class member
};
#endif
