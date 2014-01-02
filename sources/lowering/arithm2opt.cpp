/**
 * @file lowering/arithm2opt.cpp
 * Implementation of conversion of arithmetics parse tree to optimizer IR
 */
#include "low_iface.h"

namespace Lowering
{
    ArithmConverter::ArithmConverter( TreeNode *root): tree_root( root), last_oper( 0)
    {
        module = new Opt::Module();
    }
    
    void ArithmConverter::tree2Opt()
    {
        //Create the function
        func = module->newFunction("compute");

        //Walk the parse tree
        tree_root->visit( *this);

        //Make return operation
        if ( last_oper) 
            func->newOperAfter( Opt::Ret, last_oper);
        else
        {
            func->setFirstOper( func->newOper( Opt::Ret));
        }
    }

    void ArithmConverter::doPre( TreeNode *node)
    {
    
    }

    Opt::OptOperName ArithmConverter::selectOperName( TreeNode *node)
    {
        switch ( node->token().value())
        {
        case Syntax::Arithmetics::NUMBER: return Opt::Mov;
        case Syntax::Arithmetics::PLUS: return Opt::Add;
        case Syntax::Arithmetics::MINUS: return Opt::Sub;
        case Syntax::Arithmetics::MUL: return Opt::Mul;
        case Syntax::Arithmetics::DIV: return Opt::Div;

        case Syntax::Arithmetics::END:
        case Syntax::Arithmetics::NO_TOKEN:
        case Syntax::Arithmetics::LP:
        case Syntax::Arithmetics::RP:
        default:
            LOW_ASSERTXD( 0, "These tokens should not be in parse tree");
            break;
        }
        return Opt::OPT_OPERS_NUM;
    }
    
    void ArithmConverter::doPost( TreeNode *node)
    {
        Syntax::Arithmetics::Token token = node->token();
            
        //Make operation
        Opt::OptOperName name = selectOperName( node);
        Opt::Oper *oper = 
            (last_oper)? func->newOperAfter( name, last_oper) : func->newOper( name);
        if ( !last_oper)
            func->setFirstOper( oper);
        last_oper = oper;
        
        //Make temporary variable for result
        Opt::Obj *res = func->newObject( Opt::Reg);
        

        //Set args. Only NUMBER token has one argument
        if ( token.value() == Syntax::Arithmetics::NUMBER)
        {
            //Number is converted into mov imm->res
            oper->setArgImm( 0, (Int64)token.number());
        } else
        {
            Opt::Obj *arg_obj1 = tmp_objects.top(); 
            tmp_objects.pop();

            Opt::Obj *arg_obj2 = tmp_objects.top(); 
            tmp_objects.pop();

            //Stack holds objects in reverse order
            oper->setArgObj( 1, arg_obj1);
            oper->setArgObj( 0, arg_obj2);
        }

        //Set res object for operation
        oper->setResObj( 0, res);
        
        //Save the temporary variable that represents the result
        tmp_objects.push( res);
    }
    
    bool uTestArithm( UnitTest *utest)
    {
        std::string str("10-(5+5)*2");
        
        //Parse input
        Syntax::Arithmetics::Parser parser( str);
        
        //Make optimizer ir from parse tree
        ArithmConverter converter( parser.rootNode());
        converter.tree2Opt();

        //Print IR
        utest->out() << converter.function() << endl;
        return true;
    }
}