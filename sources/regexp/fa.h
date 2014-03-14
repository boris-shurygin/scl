/**
 * @file:regexp/fa.h
 * Implementation of finite automata
 * @ingroup REGEXP
 */

#pragma once


namespace RegExp
{
    /**
     * Describes the possible input character in FA and regular expressions
     * note that
     */
    class Character
    {
    public:
        Character(): ch_( 0), is_eps(true){};
        Character( char ch): ch_( ch), is_eps(false){};
        bool isMatch( char ch) const 
        {
            if ( is_eps)
                return false;
            return ch == ch_;
        };
        bool isEps() const
        {
            return is_eps;
        };
    private:
        char ch_;
        bool is_eps;
    };
    

    /**
     * State representation
     */
    typedef UInt32 State;

    /**
     * Represents the final automaton state transition
     */
    class Transition: public Printable<Transition>
    {
    public:
        /** Constructor with all fields specification */
        Transition( State s, State d, Character c = Character() ):
            src_( s), dst_( d), cond( c){};
        void toStream( std::ostream &stream) const
        {
            stream << src() << "->" << dst();
        }
        State src() const { return src_;};
        State dst() const { return dst_;};
        bool isEps() const { return cond.isEps();}
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
        
        /**
         * Build concatenation of two SubNFAs
         */
        inline SubNFA operator *();

    private:
        /* Internal state representation */
        State begin_;
        State end_;
        NFA& nfa;
    };

    /**
     * Non-deterministic finite automaton representation
     */
    class NFA: public Printable<NFA>
    {
    public:
        static const State START_STATE = 0;
        typedef std::vector<Transition *> Transitions;
        
        NFA(); /**< Construct empty NFA */
        NFA( Character ch); /**< Construct NFA accepting one character */
        virtual ~NFA(); /**< Destructor */
        void toStream( std::ostream &stm) const; /**< Text printing routine */
        
        /** Create transition from one state to another with condition of transition */
        void addTransition( State s, State d, Character c);
        
        /** Create epsilon transition from one state to another */
        void addEpsTransition( State s, State d);
        
        /** Add a new state to the NFA */
        State newState() { return state_index++;}

        /** Make new NFA subgraph that corresponds to result of concatenation operation of the given subgraphs */
        SubNFA concat( const SubNFA& left, const SubNFA& right);

        /** Make new NFA subgraph that corresponds to result of alternation operation on the given subgraphs */
        SubNFA alter( const SubNFA& left, const SubNFA& right);

        /** Make new NFA subgraph that corresponds to Kleene closure operation result on the given subgraph */
        SubNFA star( const SubNFA& snfa);

        /** Turns transitions into table */
        void buildTable();

    private:
        NFA( const NFA& orig){}; /**< Copy constructor closed */
        NFA& operator=( const NFA& orig){ return *this;}; /**< Assignment closed */
        UInt32 numStates(){ state_index; }
        void addTransition( Transition *t){ transitions.push_back(t);};

        /* Internal state representation */
        State state_index;
        std::set<State> accepting_states;
        Transitions transitions;
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