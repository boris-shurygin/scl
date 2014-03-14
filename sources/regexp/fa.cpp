/**
 * @file regexp/fa.cpp
 * Implementation of finite automata
 */
#include "regexp_iface.h"

namespace RegExp
{ 
    /** Construct empty NFA subgraph*/
    SubNFA::SubNFA( NFA& parent): nfa( parent)
    {
        begin_ = parent.newState();
        end_ = parent.newState();
    }
    
    /** Construct empty NF\A subgraph with given start and stop states */
    SubNFA::SubNFA( NFA& parent, State b, State e): 
        nfa( parent), begin_( b), end_( e)
    {
    }
        

    /** Construct NFA subgraph accepting one character */
    SubNFA::SubNFA( NFA& parent, Character ch):nfa( parent)
    {
        begin_ = parent.newState();
        end_ = parent.newState();
        parent.addTransition( begin_, end_, ch);
    }
        
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

    /** Make new NFA subgraph that corresponds to result of concatenation operation of the given subgraphs */
    SubNFA NFA::concat( const SubNFA& left, const SubNFA& right)
    {
        SubNFA res( *this, left.begin(), right.end());
        addEpsTransition( left.end(), right.begin());
        return res;
    }

    /** Make new NFA subgraph that corresponds to result of alternation operation on the given subgraphs */
    SubNFA NFA::alter( const SubNFA& left, const SubNFA& right)
    {
        SubNFA res( *this);
        
        addEpsTransition( res.begin(), left.begin());
        addEpsTransition( res.begin(), right.begin());
        
        addEpsTransition( left.end(), res.end());
        addEpsTransition( right.end(), res.end());
        
        return res;
    }

    /** Make new NFA subgraph that corresponds to Kleene closure operation result on the given subgraph */
    SubNFA NFA::star( const SubNFA& snfa)
    {
        SubNFA res( *this);
        
        addEpsTransition( res.begin(), snfa.begin() );
        addEpsTransition( res.begin(), res.end() );
        addEpsTransition( snfa.end(), res.end() );
        addEpsTransition( snfa.end(), snfa.begin() );

        return res;
    }


    /** Test finite automaton operation */
    bool uTestFA( UnitTest *utest)
    {
        NFA nfa;

        // Should recognize letter 'a'
        SubNFA nfa_a( nfa, 'a'); 
        
        // Should recognize letter 'b'
        SubNFA nfa_b( nfa, 'b'); 
        
        // Should recognize letter 'c'
        SubNFA nfa_c( nfa, 'c'); 
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