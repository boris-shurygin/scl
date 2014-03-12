/**
 * @file regexp/fa.cpp
 * Implementation of finite automata
 */
#include "regexp_iface.h"

namespace RegExp
{
    /**
     * Printing routine of NFA, prints NFA as a graph in Graphviz's DOT format
     */
    void NFA::toStream( std::ostream &stm) const
    {
        RE_ASSERTXD( 0, "Not implemented");
    }

    /** Construct empty NFA */
    NFA::NFA(): state_index( 1)
    {
    
    }
    
    /** Construct NFA accepting one character */
    NFA::NFA( Character ch): state_index( 1)
    {
    
    }

    void NFA::addTransition( State s, State d, Character c)
    {
        Transition *t_p  = new Transition( s, d, c);
        addTransition( t_p);
    }

    void NFA::addEpsTransition( State s, State d)
    {
        Transition *t_p  = new Transition( s, d);
        addTransition( t_p);
    }

    NFA::~NFA()
    {
        for ( Transitions::iterator it  = transitions.begin(),
                                    end = transitions.end(); it != end; ++it)
        {
            delete *it;        
        }
    }


    /** Test finite automaton operation */
    bool uTestFA( UnitTest *utest)
    {
        // Should recognize letter 'a'
        NFA nfa_a('a'); 
        
        // Should recognize letter 'b'
        NFA nfa_b('b'); 
        
        // Should recognize letter 'c'
        NFA nfa_c('c'); 
#if 0        
        // Should recognize word 'ab'
        NFA nfa_concat = nfa_a + nfa_b;

        // Should recognize pattern (ab)|c
        NFA nfa_alter = nfa_concat | nfa_c;

        // Should recognize pattern ((ab)|c)*
        NFA nfa_closure = *nfa_alter;

        RE_ASSERTXD( nfa_closure.test("cabcabab"), "The resulting NFA should recognize the expression");
#endif
        return true;
    }
}