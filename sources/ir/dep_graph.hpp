/**
 * @file:ir/cf.hpp
 * Declaration of the dependence graph representation classes
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{
    /**
     * @class DepEdge
     * @brief Dependence edge class
     * @ingroup DG
     */
    template <class MDesType> class DepEdge: 
        public Edge< DG <MDesType>, Operation< MDesType>, DepEdge<MDesType> >
    {
    public:
        /** Control flow edge constructor */
        inline DepEdge( DG<MDesType> *g, Operation<MDesType> *pred, Operation<MDesType> *succ);
    private:
    };

    /**
     * @class DG
     * @brief Dependence graph representation
     * @defgroup DG
     * @ingroup IR
     */
    template <class MDesType> class DG: 
        public Graph< DG <MDesType>, Operation< MDesType>, DepEdge<MDesType> >
    {
        
    };

    /** Dependence edge constructor */
    template < class MDesType> DepEdge<MDesType>::DepEdge( DG <MDesType> *g, Operation<MDesType> *pred, Operation<MDesType> *succ):
        Edge< DG <MDesType>, Operation< MDesType>, DepEdge<MDesType> >( g, pred, succ)
    {
    
    }


};