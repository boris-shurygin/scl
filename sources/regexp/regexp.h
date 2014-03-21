/**
 * @file:regexp/fa.h
 * Implementation of regular expressions
 * @ingroup REGEXP
 */

#pragma once

namespace RegExp
{
    /**
     * Exception class for regular expressions errors
     * @ingroup ARTHM
     */
    class REError: public exception
    {
    public:
        const char *what() const throw()
        {
            return "Regular expression syntax error";
        }
    };

    class WrongArgsNumInStack: public REError
    {
    public:
        const char *what() const throw()
        {
            return "Wrong number of arguments in stack";
        }   
    };

    class UnmatchedParens: public REError
    {
    public:
        const char *what() const throw()
        {
            return "Unmatched parentheses";
        }   
    };

    /** Regular expressions' operators in order of precedence */
    enum REOper
    {
        /** Left parenthesis in the stack*/
        RE_LP_STACK,
        /** Alternation */
        RE_ALTER,
        /** Concatenation */
        RE_CONCAT,
        /** Kleene star */
        RE_STAR,
        /** Left parethesis */
        RE_LP,
        /** Right parethesis */
        RE_RP,
        /** Number of operator types/invalid operator */
        RE_OPER_NUM
    };

    /**
     * Representation of a regular expression
     */
    class RegExp
    {
    public:
        RegExp( std::string str); /**< Constructor from std::string */
        Int32 indexIn( const char *str_p, Int32 pos); /**< Search for first match in string starting from given pos */
        Int32 indexIn( std::string in_str, Int32 pos); /**< Search for first match in string starting from given pos */
    private:
        RegExp( const RegExp & orig);/**< Copy constructor. Not yet implemented, so closed for now */
        RegExp operator=( const RegExp & orig);/**< Assignment. Not yet implemented, so closed for now */
        std::string str_; /** String that was used to define the regular expression */
        NFA nfa;
    };

    /**
     * Unit testing of regular expressions
     */
    bool uTest( UnitTest *utest);
} // namespace RegExp