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
    using namespace Graph;
    /**
     * @class DepEdge
     * @brief Dependence edge class
     * @ingroup DG
     */
    template <class MDesType> class DepEdge: 
        public Edge< DG <MDesType>, typename MDesType::OperationType, DepEdge<MDesType> >
    {
    public:
        /** Control flow edge constructor */
        inline DepEdge( DG<MDesType> *g, typename MDesType::OperationType *pred, typename MDesType::OperationType *succ);
    private:
    };

    /**
     * @class DG
     * @brief Dependence graph representation
     * @defgroup DG
     * @ingroup IR
     */
    template <class MDesType> class DG: 
    public Graph< DG <MDesType>, typename MDesType::OperationType, DepEdge<MDesType> >
    {
        
    };

    /** Dependence edge constructor */
    template < class MDesType> DepEdge<MDesType>::DepEdge( DG <MDesType> *g, typename MDesType::OperationType *pred, typename MDesType::OperationType *succ):
        Edge< DG <MDesType>, typename MDesType::OperationType, DepEdge<MDesType> >( g, pred, succ)
    {
    
    }


};