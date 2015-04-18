/**
 * @file:syntax/arithmetics.h
 * Sample parser for arithmetic expressions
 * @ingroup STX
 */
#pragma once

namespace Syntax
{

/**
 * Parsing of arithmetic expressions
 * @defgroup ARTHM Arithmetic Parser
 * @ingroup STX
 */
namespace Arithmetics
{
    /**
     * Maximal token size constant
     * @ingroup ARTHM
     */
    static const UInt32 MAX_TOKEN_SIZE = 256;
    
    /**
     * Exception class for parser errors
     * @ingroup ARTHM
     */
    class ParseError: public exception
    {};

    /**
     * Exception class for lexer errors
     * @ingroup ARTHM
     */
    class LexError: public exception
    {};

    /**
     * Enumeration for token values in arithmetic expressions parsing
     * @ingroup ARTHM
     */
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

    /**
     * Token class represents results of the lexical analysis
     * @ingroup ARTHM
     */
     class Token : public Printable<Token>
    {
    public:
        /** Simple constructor from token value and (optionally) a constant (for NUMBER tokens) */
        Token( TokenVal value = NO_TOKEN, UInt32 number = 0):
            val_( value), number_( number) {}

        TokenVal value()  const { return val_;   } /**< Get the token value       */
        UInt32   number() const { return number_;} /**< Get the associated number */
        
        /** Dump token to stream */
        void toStream( std::ostream &stream) const;
    private:
        TokenVal val_;
        UInt32 number_;
    };

    /**
     * Lexer class implements lexical analysis for the parsing of the arithmetic expressions
     * @ingroup ARTHM
     */
    class Lexer
    {
    public:    
        Token next();            /**< Get the next token */
        Lexer( std::string &str);/**< Lexer constructor  */
    private:    
        std::string str_copy;
        UInt32 pos;
        std::istringstream in;
    };


    /**
     * @brief Parser for the arithmetic expressions
     * @ingroup ARTHM
     * @details
     * Parses the input string by TopDown parsing method according to the grammar
     * below and builds abstract syntax tree.
     * Left-recursive grammar:
     @code
         expr: expr + prod | expr - prod | prod
         prod: prod * prim | prod / prim | prim
         prim: NUMBER | - prim | ( expr )
     @endcode
     * Equivalent right-recursive grammar:
     @code 
         expr: prod expr_r
         expr_r: + prod expr_r | - prod expr_r | epsilon
         prod: prim prod_r
         prod_r: * prim prod_r | / prim prod_r | epsilon
         prim: NUMBER | - prim | ( expr )
     @endcode
     */
    class Parser: public Printable<Parser>
    {
    public:
        typedef ASTNode< Token> TreeNode;
        
        /** Parser constructor starts parsing and creates AST as a result */
        Parser( std::string &str); 
        virtual ~Parser() { delete root; } /**< Destructor */

        /** Dump parser state to stream */
        void toStream( std::ostream &stream) const;
        
        /** Get the root node of the built AST */
        TreeNode *rootNode() const { return root;}
    private:
        /** Try to match a part of the input to the expr non-terminal */
        TreeNode* expr();
        /** Try to match a part of the input to the prod non-terminal */
        TreeNode* prod();
        /** Try to match a part of the input to the prim non-terminal */
        TreeNode* prim();

        /** Get the next token */
        Token next()
        {
            if ( curr_token.value() == NO_TOKEN)
            {
                Token t = lex.next();
                curr_token = t;
            }
            return curr_token;
        }
        /** Signal that the current token was successfully matched */
        void matched() { curr_token = Token();} 

        /* Parser internal state */
        Token curr_token;
        TreeNode *root;
        
        /* Lexer object. Used internally for lexical anlysis */
        Lexer lex;
    };

    bool uTest( UnitTest *utest);
} // Arithmetics
} // Syntax
