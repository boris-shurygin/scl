/**
 * @file:syntax/arithmetics.h
 * Sample parser for arithmetic expressions
 * @ingroup STX
 */
#pragma once

namespace Syntax
{
namespace Arithmetics
{
    static const UInt32 MAX_TOKEN_SIZE = 256;
    
    class ParseError: public exception
    {};

    class LexError: public exception
    {};

    enum TokenVal
    {
        NUMBER,
        END,
        NO_TOKEN,
        PLUS = '+',
        MINUS = '-',
        MUL = '*',
        DIV = '/',
        LP = '(',
        RP = ')',
       
    };

    class Token
    {
    public:
        Token( TokenVal value = NO_TOKEN, UInt32 number = 0):
            val_( value), number_( number) {}

        TokenVal value()  const { return val_;   }
        UInt32   number() const { return number_;}
        void toStream( std::ostream &stream)
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
    private:
        TokenVal val_;
        UInt32 number_;
    };

    inline std::ostream &operator << ( std::ostream &stream, Token &t)
    {
        t.toStream( stream);
        return stream;
    }

    class Lexer
    {
    public:    
        Token next();
        Lexer( std::string &str);
    private:    
        std::string str_copy;
        UInt32 pos;
        std::istringstream in;
    };


    /**
     * Parser for the arithmetic expressions
     * 
     * Left-recursive grammar:
     *
     * expr: expr + prod | expr - prod | prod
     * prod: prod * prim | prod / prim | prim
     * prim: NUMBER | - prim | ( expr )
     *
     *  Right-recursive grammar
     * 
     * expr: prod expr_r
     * expr_r: + prod expr_r | - prod expr_r | epsilon
     * prod: prim prod_r
     * prod_r: * prim prod_r | / prim prod_r | epsilon
     * prim: NUMBER | - prim | ( expr )
     *
     */
    class Parser
    {
    public:
        typedef ASTNode< Token> TreeNode;
        
        Parser( std::string &str);
        virtual ~Parser() { delete root; }
        void toStream( std::ostream &stream);
        TreeNode *rootNode() const { return root;}
    private:
        TreeNode* expr();
        TreeNode* prod();
        TreeNode* prim();
        Token next()
        {
            if ( curr_token.value() == NO_TOKEN)
            {
                Token t = lex.next();
                curr_token = t;
            }
            return curr_token;
        }
        void matched() { curr_token = Token();} 
        Token curr_token;
        TreeNode *root;
        
        Lexer lex;
    };

    inline std::ostream & operator <<( std::ostream &stream, Parser &p)
    {
        p.toStream( stream);
        return stream;
    }


    bool uTest( UnitTest *utest);
} // Arithmetics
} // Syntax
