/**
 * @file:lowering/arithm2opt.h
 * Interface of conversion of arithmetics parse tree to optimizer IR
 * @ingroup LOW
 */
#pragma once

namespace Lowering
{
    using namespace Utils;

    /** Typedef for shortening the node type name */
    typedef Syntax::Arithmetics::Parser::TreeNode TreeNode;
    
    /**
     * @brief Converter of the AST that resulted from parsing of the arithmetic expressions
     * to the Optimizer IR
     * @ingroup LOW
     * @details The conversion is implemented using the Visitor pattern in routines doPre and doPost
     */
    class ArithmConverter: public Syntax::ASTVisitor< TreeNode>
    {
    public:
        ArithmConverter( TreeNode *root);    /**< Constructor        */
        void tree2Opt();                     /**< Run the conversion */
        virtual void doPre( TreeNode *node); /**< Does nothing       */
        virtual void doPost( TreeNode *node);/**< Convert one node into the operation */
        Opt::Function *function() const { return func;} /**< Get the created function */

    private:
        /** Helper routine to select operation name by the token associated with AST node */
        Opt::OptOperName selectOperName( TreeNode *node);

        // Internal state
        TreeNode *tree_root;
        Opt::Module* module;
        Opt::Function *func;
        Opt::Oper *last_oper;
        std::stack<Opt::Obj *> tmp_objects;
    };
    
    /** 
     * Test arithmetic expression parsing and lowering
     * @ingroup LOW
     */
    bool uTestArithm( UnitTest *utest);
} /* Lowering */
