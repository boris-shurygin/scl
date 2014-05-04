/**
 * @file: ir/cf.hpp
 * Declaration of the control flow representation classes
 * @defgroup CF Control flow
 * @ingroup IR
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{
    /**
     * @class CFEdge
     * @brief Control flow edge class
     * @ingroup CF
     */
    template <class MDesType> class CFEdge: 
        public Edge< CFG<MDesType>, CFNode<MDesType>, CFEdge<MDesType> >
    {
    public:
        /** Control flow edge constructor */
        inline CFEdge( CFNode<MDesType> *pred, CFNode<MDesType> *succ);
        inline Operation<MDesType> *srcOper() const;       /**< Get control source operation */
        inline void setSrcOper( Operation<MDesType> *oper);/**< Set control source operation */

        /** Print edge to stream */
        inline void toStream(ostream& os);

    private:
        Operation<MDesType> *src_oper_p;
    };

    /**
     * @class CFNode
     * @brief Control flow node
     * @ingroup CF
     */
    template <class MDesType> class CFNode:
        public Node< CFG<MDesType>, CFNode<MDesType>, CFEdge<MDesType> >
    {
    public:
        inline CFNode(); /**< Constructor of CF node */

        inline Operation<MDesType> *firstOper() const;       /**< Get first operation of the node */
        inline void setFirstOper( Operation<MDesType> *oper);/**< Set first operation of the node */

        inline Operation<MDesType> *lastOper() const;        /**< Get last operation of the node */
        inline void setLastOper( Operation<MDesType> *oper); /**< Set last operation of the node */

        inline void append( Operation<MDesType> *oper); /**< Insert given operation to the end of the node */
        inline void prepend( Operation<MDesType> *oper);/**< Insert given operation in the begining of the node */

        /** Print node to stream */
        inline void toStream(ostream& os);

        inline bool isStart() const; /**< Check that the given node is the start node of the CFG */
        inline bool isStop() const;  /**< Check that the given node is the stop node of the CFG  */
    private:
        Operation<MDesType> *first;
        Operation<MDesType> *last;
    };

    /**
     * @class CFG
     * @brief Control flow graph representation class
     * @ingroup CF
     */
    template <class MDesType> class CFG:
        public Graph< CFG<MDesType>, CFNode<MDesType>, CFEdge<MDesType> >
    {
    public:
        /** Default constructor */
        CFG();

        /** Set graph as valid (corresponds to operations in the function) */
        inline void setValid( bool valid);

        /** Check if graph is valid */
        inline bool isValid() const;

        /** Print CFG to stream */
        inline void toStream(ostream& os);

        inline CFNode<MDesType> *startNode() const; /**< Get the start node of the CFG */
        inline CFNode<MDesType> *stopNode() const;  /**< Get the stop node of the CFG  */

        /** Number nodes in topological order. Nodes that are not reachable from start will have arbitrary order */
        Numeration makeTopologicalNumeration();

    private:
        bool is_valid;

        /* Pseudo nodes on top and on bottom of graph */
        CFNode<MDesType> *start_node;
        CFNode<MDesType> *stop_node;
    };

template <class MDesType> 
std::ostream& operator<<(std::ostream& os, CFEdge<MDesType> &e) 
{   
    e.toStream(os);
    return os;
} 

template <class MDesType> 
std::ostream& operator<<(std::ostream& os, CFNode<MDesType> &n) 
{   
    n.toStream(os);
    return os;
} 

template <class MDesType> 
std::ostream& operator<<(std::ostream& os, CFG<MDesType> &cfg) 
{   
    cfg.toStream(os);
    return os;
} 

};
