/**
 * @file opt/opt_utest.cpp
 * Unit testing of optimizer
 */
#include "opt_iface.hpp"

namespace Opt
{
    bool uTest(UnitTest *utest)
    {
        Module m;
        Function *f = m.newFunction();
        CFG & cfg = f->cfg();
        CFNode *node1 = cfg.newNode();
        CFNode *node2 = cfg.newNode();
        CFNode *node3 = cfg.newNode();
        cfg.newEdge(cfg.startNode(), node1);
        cfg.newEdge(node3, cfg.stopNode());

        Obj* v1 = f->newObject(Reg);
        Obj* v2 = f->newObject(Reg);
        Obj* v3 = f->newObject(Reg);
        Obj* v4 = f->newObject(Reg);

        /* {0} add vt2 = vt0, vt1*/
        Oper *add = f->newOper(Add);
        add->setArgObj(0, v1);
        add->setArgObj(1, v2);
        add->setResObj(0, v3);
        f->setFirstOper(add);
        node1->append(add);

        /* {1} brg vt2, 512, <label_4> */
        Oper *brg = f->newOper(Brg);
        brg->setArgObj(0, v3);
        brg->setArgImm(1, (Int64)0x200);
        node1->append(brg);
        CFEdge *edge1 = cfg.newEdge(node1, node2);
        CFEdge *edge2 = cfg.newEdge(node1, node3);
        edge2->setSrcOper(brg);
        UTEST_CHECK(utest, isOperCf(brg));
        UTEST_CHECK(utest, brg->isCf());

        /* {2} sub vt3 = vt2, 512 */
        Oper *sub = f->newOper(Sub);
        sub->setArgObj(0, v3);
        sub->setArgImm(1, (Int64)0x200);
        sub->setResObj(0, v4);
        node2->append(sub);
        brg->OperSeqList::attach(sub); // to preserve the function's operation chain


        /* {3} call unknown_routine, vt3 */
        Oper *call = f->newOper(Call);
        call->setArgImm(0, string("unknown_routine"));
        call->addArgToSet(v4);
        call->addArgToSet(v3);
        call->addResToSet(v4);
        node2->append(call);
        cfg.newEdge(node2, node3);

        /* {4} st 74565, vt3 */
        Oper *st = f->newOper(St);
        brg->setArgTarget(2, st);
        st->setArgImm(0, (Int64)0x12345);
        st->setArgObj(1, v4);
        node3->append(st);
        call->OperSeqList::attach(st); // to preserve the function's operation chain

        Oper *ret = f->newOper(Ret);
        node3->append(ret);

        /* Unit test of store operation representation */
        MDes::OperDes *st_des = MDes::operDescription(st->name());

        UTEST_CHECK(utest, st_des->num_args == 2);
        UTEST_CHECK(utest, st_des->num_ress == 0);

        CFNode* cf_node = cfg.newNode();

        /* Iterating over CFG nodes */
        for (CFNode *node = cfg.firstNode();
            isNotNullP(node);
            node = node->nextNode())
        {
            UTEST_CHECK(utest, isNotNullP(node->graph()));
        }

        utest->out() << "Operations as they go in linear order" << endl;
        utest->out() << f; // Print the whole function

        cfg.setValid(true);

        utest->out() << "Control flow graph print" << endl;
        utest->out() << f; // After CFG is valid the 'operator <<' should print the whole CFG

        /* Unit test of operation representation */
        MDes::OperDes *des = MDes::operDescription(Add);

        UTEST_CHECK(utest, des->num_args == 2);
        UTEST_CHECK(utest, des->num_ress == 1);
        return utest->result();
    }
}
