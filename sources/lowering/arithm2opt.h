/**
 * @file:lowering/arithm2opt.h
 * Interface of conversion of arithmetics parse tree to optimizer IR
 * @ingroup LOW
 */
#pragma once

namespace Lowering
{
    class ArithmConverter
    {
    public:
        typedef Syntax::Arithmetics::Parser::TreeNode TreeNode;
        ArithmConverter( TreeNode *root);
        void tree2Opt();
    private:
        TreeNode *tree_root;
        Opt::Module* module;
    };

    bool uTestArithm( UnitTest *utest);
}