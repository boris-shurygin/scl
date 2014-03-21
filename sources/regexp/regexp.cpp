/**
 * @file regexp/regexp.cpp
 * Implementation of regular expressions
 */
#include "regexp_iface.h"

namespace RegExp
{
    const char * op2Str( REOper op)
    {
        static const char *op_str[ ] = 
        {
           /* RE_LP_STACK */
           "left_paren_stack",
           /* RE_ALTER */
           "alter",
           /* RE_CONCAT */
           "concat",
           /* RE_STAR */ 
           "star",
           /* RE_LP */
           "left_paren",
            /* RE_RP */
            "right_paren"
        };

        return op_str[ op];
    }

    /**
     * Get the operator corresponding to the given symbol
     */
    inline REOper char2REOper( char c)
    {
        switch (c)
        {
        case '|': return RE_ALTER;
        case '*': return RE_STAR;
        case '(': return RE_LP;
        case ')': return RE_RP;
        default:  return RE_OPER_NUM;
        }
        return RE_OPER_NUM;
    }

    /** Evaluate the expression consisting of top of the operation stack with operands from nfa stack */
    inline void eval( std::stack<SubNFA>& nfa_stack,
                      std::stack<REOper>& op_stack,
                      NFA &nfa)
    {
        REOper op = op_stack.top();
        op_stack.pop();

        switch ( op)
        {
            case RE_ALTER:
            case RE_CONCAT:
            {
                if ( nfa_stack.size() < 2)
                    throw WrongArgsNumInStack();

                /* Pop two operands */
                SubNFA b = nfa_stack.top(); 
                nfa_stack.pop();
        
                SubNFA a = nfa_stack.top();
                nfa_stack.pop();
                
                if ( op == RE_ALTER)
                { 
                    SubNFA res = a | b;
                    nfa_stack.push( res);
                } else
                {
                    SubNFA res = a + b;
                    nfa_stack.push( res);
                }
                break;
            }
            case RE_STAR:
            {
                if ( nfa_stack.size() < 1)
                    throw WrongArgsNumInStack();
    
                /* Pop one operand */
                SubNFA a = nfa_stack.top(); 
                nfa_stack.pop();
               
                SubNFA res = *a;
                nfa_stack.push( res);
                break;
            }
            case RE_LP_STACK: /* Do nothing, we popped the op_stack already */
                break;
            case RE_LP:
            case RE_RP:
            case RE_OPER_NUM:
            default:
                throw REError();
        }
    }

    /** Constructor from std::string */
    RegExp::RegExp( std::string str)
    {
        /* 
         * Evaluating the regular expression using two stacks: 
         * the stack of operands which are the NFA subgraphs and the stack
         * of the operations the alternation, the concatenation and the Kleene star.
         */
        std::stack<SubNFA> nfa_stack;
        std::stack<REOper> op_stack;

        std::string::iterator it = str.begin();
        std::string::iterator end = str.end();

        char prev_char = '(';
        Int32 brackets = 0;

        /**
         * We read input and turn non-operator characters to simple NFA. These NFAs are pushed
         * into nfa_stack. In expression evaluation it corresponds to the operand stack. When we
         * find an operator we compare its precedence (defined by value in the enumeration of REOper)
         * with the operator in the top of the stack. If its precedence is higher than we push it on the
         * stack, if not we evaluate the topmost operator with the topmost NFAs in the nfa stack. The result
         * is put back to the NFA stack. This evaluation is continued until the current operator has higher
         * priority than the topmost operator.
         */
        while ( it != end)
        {
            char c = *it; //read current symbol
            REOper op = char2REOper( c);
            switch ( op)
            {
            case RE_LP:
                brackets++;
            /* fallthrough is correct here */

            case RE_ALTER:
            case RE_STAR:
            {
                while ( !op_stack.empty() && op < op_stack.top())
                {
                    eval( nfa_stack, op_stack, nfa);
                }
                if ( op == RE_LP)
                    op = RE_LP_STACK;
                op_stack.push( op);
                break;
            }
            case RE_RP:
            {
                brackets--;
                while ( !op_stack.empty() )
                {
                    if ( op_stack.top() == RE_LP_STACK)
                    {
                        eval( nfa_stack, op_stack, nfa);
                        break;
                    } else
                    {
                        eval( nfa_stack, op_stack, nfa);
                    }
                }
                if ( brackets < 0)
                {
                    throw UnmatchedParens();
                }
                break;
            }
            case RE_OPER_NUM:
            default:
                if ( prev_char != '(' && prev_char != '|')
                {
                    /*
                     * We detect concatenation when we have a non-operator symbol
                     * and if the previous character exists and it was not the '(' or the '|'
                     */
                    op = RE_CONCAT;
                    while ( !op_stack.empty() && op < op_stack.top())
                    {
                        eval( nfa_stack, op_stack, nfa);
                    }
                    op_stack.push( op);
                }
                SubNFA mini_nfa( nfa, c);
                nfa_stack.push( mini_nfa);
                break;
            }
            prev_char = c;
            ++it;
        }

        while ( !op_stack.empty() )
        {
            eval( nfa_stack, op_stack, nfa);
        }
        if ( brackets != 0)
        {
            throw UnmatchedParens();
        }

        nfa.setAsMain( nfa_stack.top());
        nfa.buildTable();// Now the NFA is ready to test strings

#if 1
        std::ofstream out2;
        out2.open("nfa_re.dot");
        nfa.dotToStream( out2);
        out2.close();
#endif
    }
    
    /** Search for first match in string starting from given pos */
    Int32 RegExp::indexIn( const char* str_p, Int32 pos = 0)
    {
        const std::string str( str_p);
        return indexIn( str, pos);
    }

    /** Search for first match in string starting from given pos */
    Int32 RegExp::indexIn( std::string in_str, Int32 pos = 0)
    {
        if ( pos < 0)
            return -1;

        std::string::const_iterator it = in_str.begin() + pos;
        std::string::const_iterator end = in_str.end();
        
        Int32 match_pos = 0;

        while ( it != end)
        {
            UInt32 matched = nfa.findIn( in_str, it);
        
            if ( matched)
                return match_pos;
            ++match_pos;
            ++it;
        }
        return -1;
    }
    
    /**
     * Unit testing of regular expressions
     */
    bool uTest( UnitTest *utest)
    {
        RegExp rx("(ab|bc)d*");

        UTEST_CHECK( utest, rx.indexIn( "ab") != -1);
        UTEST_CHECK( utest, rx.indexIn( "fffffffab") != -1);
        UTEST_CHECK( utest, rx.indexIn( "bcddd") != -1);
        UTEST_CHECK( utest, rx.indexIn( "asdf   asdf bcddd  ") != -1);
        UTEST_CHECK( utest, rx.indexIn( "ddddffff") == -1);
        UTEST_CHECK( utest, rx.indexIn( "addd") == -1);
        return true;
    }
}