/**
 * @file syntax/arithmetics.cpp
 * Implementation of sample parser for the arithmetic expressions
 */
#include "stx_iface.h"

namespace Syntax
{
namespace Arithmetics
{
    TokenVal op2TokenVal( char op)
    {
        switch ( op)
        {
        case '+': return PLUS;
        case '-': return MINUS;
        case '*': return MUL;
        case '/': return DIV;
        case '(': return LP;
        case ')': return RP;
        default:
            STX_ASSERTD( 0);
        }
        return END;
    }

    /** Dump token to stream */
    void Token::toStream( std::ostream &stream)
    {
        //stream << "(";
        switch ( val_)
        {
        case NUMBER:
            //stream << "NUM,";
            stream << number_;
            break;
        case END: stream << "END"; break;
        case PLUS: stream << "ADD"; break;
        case MINUS: stream << "SUB"; break;
        case MUL: stream << "MUL"; break;
        case DIV: stream << "DIV"; break;
        case LP: stream << "LP"; break;
        case RP: stream << "RP"; break;
        case NO_TOKEN: break;
        }
        //stream << ")";
    }

    Lexer::Lexer( std::string &str):
        pos( 0), str_copy( str)
    {
        in.str( str_copy);   
    }

    Token Lexer::next()
    {
        char symb;
        if ( in >> symb)
        {
            switch ( symb)
            {
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case '0':
                {
                    in.putback( symb);
                    UInt32 numb;
                    in >> numb;
                    if ( in.fail())
                    {
                        throw LexError();
                    }
                    return Token( NUMBER, numb);
                }
                case '+':
                case '-':
                case '*':
                case '/':
                case '(':
                case ')':
                    return Token( op2TokenVal( symb));
            }
        }
        return Token( END);
    }

    Parser::Parser( std::string &str): lex( str), root(0)
    {
        root = expr();// Start parsing
    }

    void Parser::toStream( std::ostream &stream)
    {
        ASTPrinter< TreeNode> printer( stream);
        if ( root)
            root->visit( printer);
    }
    
    /**
     * @brief Implementation of grammar rule for expressions
     *
     * Implements two rules in one routine
     * expr: prod expr_r
     * expr_r: + prod expr_r | - prod expr_r | epsilon
     *
     * The resuting tree should correspond to the left-recursive rule 
     *  expr: expr + prod | expr - prod | prod
     *  (to respect the left associativity of minus operator)
     */
    Parser::TreeNode* Parser::expr()
    {
        // expr: prod expr_r
        TreeNode *n = prod();
        
        if ( n)
        {
            // expr_r: + prod expr_r | - prod expr_r | epsilon
            Token t = next();
            FOREVER // recursion on the 'expr_r' non-terminal turned into loop
            {
                t = next();
                switch ( t.value() )
                {
                case PLUS:
                case MINUS:  
                {
                    matched();
                    TreeNode* prod_node = prod();
                    if ( prod_node)
                    {
                        TreeNode *op_node = new TreeNode( t);
                        n->connect( op_node);
                        prod_node->connect( op_node);
                        n = op_node;
                    } else
                    {
                        delete n;
                        return 0;
                    }
                    break;
                }
                case END:
                default:
                    return n;
                }
            }
        }
        return n;
    }

    /**
     * @brief Implementation of grammar rule for product expressions
     *
     * Implements two rules in one routine
     * prod: prim prod_r
     * prod_r: * prim prod_r | / prim prod_r | epsilon
     */
    Parser::TreeNode* Parser::prod()
    {
        // prod: prim prod_r
        TreeNode *n = prim();
        
        if ( n)
        {
            // prod_r: * prim prod_r | / prim prod_r | epsilon
            Token t = next();
            
            FOREVER // recursion on the 'prod_r' non-terminal turned into loop
            {
                t = next();
                switch ( t.value() )
                {
                case MUL:
                case DIV:  
                {    
                    matched();
                    TreeNode* prim_node = prim();
                    if ( prim_node)
                    {
                        TreeNode *op_node = new TreeNode( t);
                        n->connect( op_node);
                        prim_node->connect( op_node);
                        n = op_node;
                    } else
                    {
                        delete n;
                        return 0;
                    }
                    break;
                }
                case END:
                default:
                    return n;
                }
            }
        }
        return n;
    }

    /**
     * @brief Implementation of grammar rule for primary expressions
     *
     * Implements the rule
     * prim: NUMBER | - prim | ( expr )
     */
    Parser::TreeNode* Parser::prim()
    {
        Token t = next();
        switch ( t.value())
        {
        case NUMBER:
            matched();
            return new TreeNode( t);
        case MINUS:
        {
            matched();
            TreeNode *prim_node = prim();
            //Successfull parsing of prim
            if ( prim_node)
            {
                TreeNode *curr_node = new TreeNode( t);
                prim_node->connect( curr_node);
                return curr_node;
            }
            break;
        }
        case LP:
        {
            matched();
            TreeNode *expr_node = expr();
            if ( expr_node)
            {
                Token t2 = next();
                if ( t2.value() == RP)
                {
                    matched();
                    return expr_node;
                }
            }
            break;
        }
        default:
            break;
        }
        return 0;
    }
    
    /**
     * Unit testing for the arithmetics parser
     */
    bool uTest( UnitTest *utest)
    {
        std::string str("((1-2)+(2+3))");
        Parser parser( str);
        utest->out() << str << endl;
        utest->out() << parser << endl;

        std::string str2("1-2+(2+3)*8/2+10");
        Parser parser2( str2);
        utest->out() << str2 << endl;
        utest->out() << parser2 << endl;

        return true;
    }
} // Arithmetics
} // Syntax