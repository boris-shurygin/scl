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
        for ( Transitions::const_iterator it  = transitions.begin(),
                                          end = transitions.end(); it != end; ++it)
        {
            Transition *t_p = *it;
            stm << *t_p << "\n";
        }
    }
    
    /**
     * Printing routine of NFA, prints NFA as a graph in Graphviz's DOT format
     */
    void NFA::dotToStream( std::ostream &stm) const
    {
        stm << "digraph {\n"; 
        stm << "rankdir=LR;\n";
        stm << START_STATE << "[ rank=0];\n";
            
        if ( !use_table)
        {
            for ( Transitions::const_iterator it  = transitions.begin(),
                                              end = transitions.end(); it != end; ++it)
            {
                Transition *t_p = *it;
                stm << *t_p << "[ label=\"" << t_p->condChar() << "\"];\n";
            }
        } else
        {
            /* For each state */
            for ( State s = 0; s < numStates(); ++s)
            {
                /* For each character */
                for ( std::map<Character, UInt32>::const_iterator it  = char_index.begin(), 
                                                            end = char_index.end(); it != end; ++it)
                {
                    Character ch = (*it).first;
                    const std::set<State> &next_states = getNextStates( s, ch);
                    for ( std::set<State>::const_iterator dst_it  = next_states.begin(),
                                                    dst_end = next_states.end(); dst_it != dst_end; ++dst_it)
                    {
                        stm << s << "->" << *dst_it <<" [ label=\"" << ch << "\"];\n";
                    }
                }
            }
        }
        for ( std::set<State>::const_iterator it  = accepting_states.begin(),
                                                end = accepting_states.end(); it != end; ++it)
        {
            stm << *it << "[ shape=doublecircle];";
        }
        stm << "}";
    }
    
    /** Turns transitions into table */
    void NFA::buildTable()
    {
        char_index[ Character() ] = 0; // add epsilon to alphabet
        
        // Calclulate number of rows = number of states
        UInt32 num_rows = numStates();

        // Calculate number of colums = number of input Characters + 1 (for eps)
        UInt32 num_columns = 0;
        for ( std::map<Character, UInt32>::iterator it  = char_index.begin(), 
                                                    end = char_index.end(); it != end; ++it)
        {
            Character ch = (*it).first;
            char_index[ ch] = num_columns++;
        }

        // Resize table
        row_size = num_columns;
        table.resize( num_columns * num_rows);

        // Fill in the table
        for ( Transitions::iterator it  = transitions.begin(),
                                    end = transitions.end(); it != end; ++it)
        {
            Transition *t = *it;
            std::set<State> &next_states = table[ getTableVectorIndex( t->src(), t->condChar())];
            next_states.insert( t->dst());
        }
        use_table = true;
    }

    /** Construct empty NFA */
    NFA::NFA(): state_index( 1), row_size( 0), use_table( false)
    {
    
    }
    
    /** Construct NFA accepting one character */
    NFA::NFA( Character ch): state_index( 1), row_size( 0), use_table( false)
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
        RE_ASSERTXD( !use_table, "We should not modify the NFA after the table creation");
            
        SubNFA res( *this, left.begin(), right.end());
        addEpsTransition( left.end(), right.begin());
        return res;
    }

    /** Make new NFA subgraph that corresponds to result of alternation operation on the given subgraphs */
    SubNFA NFA::alter( const SubNFA& left, const SubNFA& right)
    {
        RE_ASSERTXD( !use_table, "We should not modify the NFA after the table creation");
        
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
        RE_ASSERTXD( !use_table, "We should not modify the NFA after the table creation");
        
        SubNFA res( *this);
        
        addEpsTransition( res.begin(), snfa.begin() );
        addEpsTransition( res.begin(), res.end() );
        addEpsTransition( snfa.end(), res.end() );
        addEpsTransition( snfa.end(), snfa.begin() );

        return res;
    }

    void NFA::setAsMain( const SubNFA &snfa)
    {
        RE_ASSERTXD( !use_table, "We should not modify the NFA after the table creation");
        
        State final = newState();
        addEpsTransition( START_STATE, snfa.begin());
        addEpsTransition( snfa.end(), final);
        accepting_states.insert( final);
    }

    
    /** Test if the input string is accepted by this NFA */
    bool NFA::test( const char* str)
    {
        std::string std_str( str);
        return test( std_str);
    }

    /** Test if the input string is accepted by this NFA */
    bool NFA::test( const std::string &str)
    {
        return findIn( str) != 0;
    }    
    
    UInt32
    NFA::findIn( const std::string &str)
    {
        return findIn( str, str.begin());
    }

    UInt32
    NFA::findIn( const std::string &str, std::string::const_iterator start_iterator)
    {
        RE_ASSERTD( use_table);
    
        std::string::const_iterator in_it = start_iterator;
        
        std::set<State> current_states;
        std::string::const_iterator acpt_it = str.end();

        current_states.insert( State(START_STATE));
        
        UInt32 num_matched = 0;
        UInt32 res = 0;

        RE_LOGS( "Testing string " << str << endl);
        RE_LOG_INC_INDENT;

        FOREVER
        {
            if ( areSetsIntersected( accepting_states, current_states))
            {
                acpt_it = start_iterator;
                res = num_matched;
            }
        
            std::set<State> new_states;
            bool shift_input = false;

            if ( in_it != str.end())
            {
                Character curr_char = *in_it;
                
                /* Current symbol won't be recognized by the automaton */
                if ( !isCharInSet( curr_char) )
                    break;
                
                RE_LOGS( "Current character: " << curr_char << ", states: ");

                /* Check all outgoing transitions from each of the current states */
                for ( std::set<State>::iterator set_it  = current_states.begin(),
                                                set_end = current_states.end();
                                                set_it != set_end; ++set_it )
                {
                    State curr_state = *set_it;
                    const std::set<State> &next_states = getNextStates( curr_state, curr_char);

                    RE_LOGS( "state " << curr_state << endl);

                    if ( next_states.size() > 0)
                    {
                        /* Move the string on */
                        shift_input = true;
                        std::set_union( new_states.begin(), new_states.end(), 
                                        next_states.begin(), next_states.end(), 
                                        std::inserter( new_states, new_states.begin()) );
                        RE_LOGS( "   new states: ");

                        for ( std::set<State>::iterator add_set_it  = new_states.begin(),
                                                    add_set_end = new_states.end();
                                                    add_set_it != add_set_end; ++add_set_it )
                        {
                            RE_LOGS( *add_set_it << " ");
                        }
                        RE_LOGS( endl);
                    }
                }
                RE_LOGS( endl);
            }
            
            if ( shift_input)
            {
                ++in_it; 
                num_matched++;
            }
            
            /* Try epsilon transitions */
            {
                RE_LOGS( "trying epsilon transitions, states: ");

                for ( std::set<State>::iterator set_it  = current_states.begin(),
                                            set_end = current_states.end();
                                            set_it != set_end; ++set_it )
                {
                    Character eps;
                    State curr_state = *set_it;
                    
                    RE_LOGS( "  state " << curr_state << endl);

                    const std::set<State> &next_states_eps = getNextStates( curr_state, eps);
                    
                    std::set_union( new_states.begin(), new_states.end(), 
                                    next_states_eps.begin(), next_states_eps.end(), 
                                    std::inserter( new_states, new_states.begin()) );

                    RE_LOGS( "   new states: ");

                    for ( std::set<State>::iterator add_set_it  = new_states.begin(),
                                                add_set_end = new_states.end();
                                                add_set_it != add_set_end; ++add_set_it )
                    {
                        RE_LOGS( *add_set_it << " ");
                    }
                    RE_LOGS( endl);
                }
                RE_LOGS( endl);
            }

            current_states = new_states;

            RE_LOGS( "States number " <<  current_states.size() << endl);

            if ( current_states.size() == 0) 
                break;
        }
        RE_LOG_DEC_INDENT;
        RE_LOGS( "result: " << res << endl);
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
        
        // Should recognize word 'ab'
        SubNFA nfa_concat = nfa_a + nfa_b;

        // Should recognize pattern (ab)|c
        SubNFA nfa_alter = nfa_concat | nfa_c;

        // Should recognize pattern ((ab)|c)*
        //SubNFA nfa_closure = *nfa_alter;

        nfa.setAsMain( nfa_alter);
        nfa.buildTable();// Now the NFA is ready to test strings
       
        UTEST_CHECK( utest, nfa.test("ab")); // The resulting NFA should recognize the expression
        UTEST_CHECK( utest, nfa.test("c")); // The resulting NFA should recognize the expression
        UTEST_CHECK( utest, !nfa.test("d")); // The resulting NFA should recognize the expression
        UTEST_CHECK( utest, !nfa.test("b")); // The resulting NFA should recognize the expression
        
        NFA nfa2;

        // Should recognize letter 'a'
        SubNFA nfa_a2( nfa2, 'a'); 
        
        // Should recognize letter 'b'
        SubNFA nfa_b2( nfa2, 'b'); 
        
        // Should recognize letter 'c'
        SubNFA nfa_c2( nfa2, 'c'); 
        
        // Should recognize word 'ab'
        SubNFA nfa_concat2 = nfa_a2 + nfa_b2;

        // Should recognize pattern (ab)|c
        SubNFA nfa_alter2 = nfa_concat2 | nfa_c2;

        // Should recognize pattern ((ab)|c)*
        SubNFA nfa_closure2 = *nfa_alter2;

        nfa2.setAsMain( nfa_closure2);
        nfa2.buildTable();// Now the NFA is ready to test strings

        UTEST_CHECK( utest, nfa2.test("cabcabab")); // The resulting NFA should recognize the expression
       
        NFA nfa3;
        
        nfa3.buildTable();

        UTEST_CHECK( utest, !nfa3.test("cabcabab")); // The resulting NFA should recognize the expression
       

        NFA nfa4;
        
        SubNFA nfa_a4( nfa4, 'a');
        SubNFA nfa_b4(nfa4, 'b');
        SubNFA nfa_b4_2(nfa4, 'b');
        SubNFA nfa_c4(nfa4, 'c');
        SubNFA nfa_d4(nfa4, 'd');

        SubNFA nfa_concat_ab = nfa_a4 + nfa_b4;
        SubNFA nfa_concat_bc = nfa_b4_2 + nfa_c4;
        SubNFA nfa_alter_abbc = nfa_concat_ab | nfa_concat_bc;
        SubNFA nfa_star_d = *nfa_d4;
        SubNFA nfa_concat_abbcd = nfa_alter_abbc + nfa_star_d;
        nfa4.setAsMain( nfa_concat_abbcd);
        nfa4.buildTable();
        
        //NFA should recognize (ab|bc)d*"
        UTEST_CHECK( utest, nfa4.test("ab")); // The resulting NFA should recognize the expression
#if 1
        std::ofstream out2;
        out2.open("nfa_fa.dot");
        nfa4.dotToStream( out2);
        out2.close();
#endif
        return utest->result();
    }
}