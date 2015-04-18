/**
 * @file:regexp/fa.h
 * Implementation of finite automata
 * @ingroup REGEXP
 */

#pragma once

namespace RegExp
{
    using namespace Utils;

    /**
     * Describes the possible input character in FA and regular expressions
     * @ingroup REGEXP
     */
    class Character: public Printable<Character>
    {
    public:
        /** Default constructor. Creates 'epsilon' pseudo character */
        Character(): ch_( 0), is_eps(true){};
        
        /** Constructor from char. Creates Character that matches given char */
        Character( char ch): ch_( ch), is_eps(false){};
        
        /** Tests if the given char matches this character */
        bool isMatch( char ch) const 
        {
            if ( is_eps)
                return false;
            return ch == ch_;
        }

        /** Tests if this character is epsilon */
        bool isEps() const
        {
            return is_eps;
        };

        /** Printing routine */
        void toStream( std::ostream &stm) const
        {
            if ( is_eps)
            {
                stm << "eps";
            } else
            {
                stm << ch_;
            }
        }
        
        /** Needed to insert characters into map */
        bool operator <( const Character &rhs) const
        {
            if ( is_eps)
            {
                if ( rhs.isEps())
                {
                    return false;
                }
                return true;
            }
            return ch_ < rhs.ch_;
        }
    private:
        char ch_;
        bool is_eps;
    };
    

    /**
     * NFA State representation
     * @ingroup REGEXP
     */
    typedef UInt32 State;

    /**
     * Represents the final automaton state transition
     * @ingroup REGEXP
     */
    class Transition: public Printable<Transition>
    {
    public:
        /** Constructor with all fields specification */
        Transition( State s, State d, Character c = Character() ):
            src_( s), dst_( d), cond( c){};

        /** Printing routine */
        void toStream( std::ostream &stream) const
        {
            stream << src() << "->" << dst();
        }

        State src() const { return src_;}; /**< Get transition initial state */
        State dst() const { return dst_;}; /**< Get transition destination state */
        
        bool isEps() const { return cond.isEps();} /**< Check if the transition is epsilon */

        /** Get the transition's condition */
        const Character &condChar() const { return cond;}
    private:
        /* No indirection in representation so the default copy constructor and assignment should work */
        Character cond;
        State src_;
        State dst_;
    };

    /* Predeclaration */
    class NFA;

    /**
     * Representation of NFA subgraph for composing NFA using Thompson's algorithm
     * @ingroup REGEXP
     */
    class SubNFA: public Printable<SubNFA>
    {
    public:
        SubNFA( NFA& parent); /**< Construct empty NFA subgraph*/
        SubNFA( NFA& nfa, Character ch); /**< Construct NFA subgraph accepting one character */
        SubNFA( NFA& parent, State b, State e); /**< Construct empty NFA subgraph with given start and stop states */
        
        /** Text printing routine */
        void toStream( std::ostream &stm) const 
        {
            stm << "start " << begin_ << "end " << end_;
        }
        
        inline State begin() const { return begin_;} /**< Return the initial state of this NFA subgraph */
        inline State end() const { return end_;}  /**< Return the accepting state of this NFA subgraph */
        inline NFA& parentNFA() const { return nfa;} /**< Get the parent NFA */
        
        /** Kleene star operation */
        inline SubNFA operator *();

    private:
        /* Internal state representation */
        State begin_;
        State end_;
        NFA& nfa; /**< Parent NFA reference */
    };

    /**
     * Non-deterministic finite automaton representation
     */
    class NFA: public Printable<NFA>
    {
    public:
        /** Starting state of the NFA */
        static const State START_STATE = 0;

        /** Container for transitions */
        typedef std::vector<Transition *> Transitions;
        
        NFA(); /**< Construct empty NFA */
        NFA( Character ch); /**< Construct NFA accepting one character */
        virtual ~NFA(); /**< Destructor */
        void toStream( std::ostream &stm) const; /**< Text printing routine */
        void dotToStream( std::ostream &stm) const; /**< Dump to given stream as a dot graph */
        void dot2File( const char *fname) const;  /**< Dump to given file as a dot graph  */

        /** Create transition from one state to another with condition of transition */
        void addTransition( State s, State d, Character c);
        
        /** Create epsilon transition from one state to another */
        void addEpsTransition( State s, State d);
        
        /** Add a new state to the NFA */
        State newState()
        {
            RE_ASSERTXD( !use_table, "We should not create new state after the table creation");
            return state_index++;
        }

        /** Make new NFA subgraph that corresponds to result of concatenation operation of the given subgraphs */
        SubNFA concat( const SubNFA& left, const SubNFA& right);

        /** Make new NFA subgraph that corresponds to result of alternation operation on the given subgraphs */
        SubNFA alter( const SubNFA& left, const SubNFA& right);

        /** Make new NFA subgraph that corresponds to Kleene closure operation result on the given subgraph */
        SubNFA star( const SubNFA& snfa);

        /** Turns transitions into table */
        void buildTable();

        /** Connect the given SubNFA to the NFA's initial and accepting states */
        void setAsMain( const SubNFA &snfa);

        /** Check that the given state is one of the accepting states */
        bool isStateFinal( State s)
        {
            return accepting_states.find( s) != accepting_states.end();
        }

        /** Test if the input string is accepted by this NFA */
        bool test( const char* str);

        /** Test if the input string is accepted by this NFA */
        bool test( const std::string &str);

        /** Search for matches in given string starting from the beginning, returns number of matched symbols */
        Int32 findIn( const std::string &str);

        /** Search for matches in given string starting from given position, returns number of matched symbols */
        Int32 findIn( const std::string &str,
                       std::string::const_iterator start_iterator);

        bool isCharInSet( Character ch)
        {
            std::map<Character, UInt32>::const_iterator it = char_index.find( ch);
            
            return it != char_index.end();
        }
    private:

        NFA( const NFA& orig){}; /**< Copy constructor closed */
        NFA& operator=( const NFA& orig){ return *this;}; /**< Assignment closed */

        inline UInt32 numStates() const
        {
            return state_index;
        }
        inline void addTransition( Transition *t)
        {
            transitions.push_back(t);
            char_index[ t->condChar()] = 0;
        };
        
        inline UInt32 getCharIndex( Character ch) const
        {
            std::map<Character, UInt32>::const_iterator it = char_index.find( ch);
            
            RE_ASSERTD( it != char_index.end());

            return (*it).second;
        }

        inline UInt32 getTableVectorIndex( State s, Character ch) const
        {
            return (s * row_size) // Selects row for given state
                   + getCharIndex( ch); // offset in a row
        }

        inline const std::set<State> &getNextStates( State s, Character ch) const
        {
            return table[ getTableVectorIndex( s, ch)];
        }

        /* Internal state representation */
        State state_index;
        std::set<State> accepting_states;
        Transitions transitions;
        std::map<Character, UInt32> char_index;
        std::vector< std::set<State> > table;
        UInt32 row_size;
        bool use_table;
    };

    /**
     * Build concatenation of two SubNFAs
     */
    inline SubNFA operator+( const SubNFA& left, const SubNFA& right)
    {
        RE_ASSERTD( areEqP( &left.parentNFA(), &right.parentNFA()));
        return left.parentNFA().concat( left, right);
    }

    /**
     * Build alternation of two SubNFAs
     */
    inline SubNFA operator|( const SubNFA& left, const SubNFA& right)
    {
        RE_ASSERTD( areEqP( &left.parentNFA(), &right.parentNFA()));
        return left.parentNFA().alter( left, right);
    }

    /**
     * Build Kleene closure of SubNFA
     */
    inline SubNFA SubNFA::operator*()
    {
        return parentNFA().star( *this);
    }

    /** Test finite automaton operation */
    bool uTestFA( UnitTest *utest);

} // namespace RegExp