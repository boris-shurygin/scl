/**
 * @file:lowering/arithm2opt.h
 * Interface of conversion of arithmetics parse tree to optimizer IR
 * @ingroup LOW
 */
#pragma once

namespace Lowering
{
    typedef Syntax::Arithmetics::Parser::TreeNode TreeNode;
    
    class ArithmConverter: public Syntax::ASTVisitor< TreeNode>
    {
    public:
        ArithmConverter( TreeNode *root);
        void tree2Opt();
        virtual void doPre( TreeNode *node);
        virtual void doPost( TreeNode *node);
        Opt::Function *function() const { return func;} 
    private:

        Opt::OptOperName selectOperName( TreeNode *node);

        TreeNode *tree_root;

        Opt::Module* module;
        Opt::Function *func;
        Opt::Oper *last_oper;
        std::stack<Opt::Obj *> tmp_objects;
    };

    bool uTestArithm( UnitTest *utest);
} /* Lowering */