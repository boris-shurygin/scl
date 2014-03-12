/**
 * @file:regexp/fa.h
 * Implementation of regular expressions
 * @ingroup REGEXP
 */

#pragma once

namespace RegExp
{
    /**
     * Representation of a regular expression
     */
    class RegExp
    {
    public:
        RegExp();/**< Default constructor */
        RegExp( std::string str); /**< Constructor from std::string */
        RegExp( const RegExp & orig);/**< Copy constructor */
    private:
        std::string str_; /** String that was used to define the regular expression */
        NFA nfa;
    };

    /**
     * Unit testing of regular expressions
     */
    bool uTest( UnitTest *utest);
} // namespace RegExp