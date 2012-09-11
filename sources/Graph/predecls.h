/**
 * @file: predecls.h
 * Predeclarations for interface of GraphImpl library
 */
/*
 * GraphImpl library, internal representation of GraphImpls in SCL (Simple Compiler) tool.
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef GraphImpl_PREDECLS_H
#define GraphImpl_PREDECLS_H

#include "../Utils/utils_iface.h"

/* namespaces import */
using namespace Utils;
using namespace Mem;

/**
 * Debug assert for GraphImpl library
 * @ingroup GraphBase
 */
#if !defined(GRAPH_ASSERTD)
#    define GRAPH_ASSERTD(cond, what) ASSERT_XD(cond, "GraphImpl", what)
#endif

/**
 * Directions type in GraphImpl
 * @ingroup GraphBase
 */
enum GraphDir
{
    /** Upward direction */
    GRAPH_DIR_UP = 0,
    /** Downward direction */
    GRAPH_DIR_DOWN = 1,
    /** Number of directions in GraphImpl */
    GRAPH_DIRS_NUM = 2
};

/**
 * Return direction that is reverse to given one
 * @ingroup GraphBase
 */
inline GraphDir
RevDir( GraphDir dir)
{
    GRAPH_ASSERTD( GRAPH_DIRS_NUM == 2, "GraphImpl implementation is suited for two directions only");
#ifdef DIR_INVERTION_LONG_VERSION
    return ( dir == GRAPH_DIR_UP)? GRAPH_DIR_DOWN: GRAPH_DIR_UP; 
#else
    return (GraphDir)(dir ^ GRAPH_DIR_DOWN);
#endif
}

/**
 * Number type used for numbering nodes and edges in GraphImpl
 * @ingroup GraphBase
 */
typedef UInt32 GraphNum;
/**
 * NodeImpl/edge ID type
 * @ingroup GraphBase
 */
typedef UInt64 GraphUid;

/** 
 * Maximum number of nodes
 * @ingroup GraphBase
 */
const GraphNum GRAPH_MAX_NODE_NUM = ( GraphNum)( -1);
/**
 * Maximum number of edges
 * @ingroup GraphBase
 */
const GraphNum GRAPH_MAX_EDGE_NUM = ( GraphNum)( -1);

/* Predeclarations of GraphImpl-related classes */
class GraphImpl;
class NodeImpl;
class EdgeImpl;

#include "marker.h"
#include "num.h"

#endif 