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
        
        void addTransition( State s, State d, Character c);
        void addEpsTransition( State s, State d);
        
        State newState() { return state_index++;}
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

    /** Test finite automaton operation */
    bool uTestFA( UnitTest *utest);

} // namespace RegExp