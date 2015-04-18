/**
 * @file:syntax/ast.h
 * Implementation of abstract syntax tree
 * @ingroup STX
 */

#pragma once


namespace Syntax
{

    /**
     * Node of the abstract syntax tree
     * @ingroup STX
     */
    template <class Token> class ASTNode: public Printable< ASTNode< Token> >
    {
        //Conveniece typedefs
        typedef typename std::list< ASTNode* > NodeList;
        typedef typename NodeList::iterator iterator;

    public:
        void toStream( std::ostream &stream) const;                  /**< Text dump of the node */
        ASTNode( Token t = Token(), ASTNode *parent_node = 0); /**< AST node constructor  */
        virtual ~ASTNode();            /**< AST node destructor. Deletes children as well */
 
        /** Make node a child of the given parent node */
        void connect( ASTNode *parent_node);
        void disconnect();             /**< Disconnect from parent. Children connection is preserved */
        void removeChild( ASTNode* n); /**< Remove the given node from the list of the children      */
        
        /** Get the parent node */
        inline ASTNode* parent(){ return parent_;}; 
        
        /** Create the iterator pointing to the first child */
        inline iterator childrenBeg()
            { return children_.begin();};
        /** Create the child list 'end' iterator */
        inline iterator childrenEnd()
            { return children_.end();};
        
        inline UInt32 id(){ return id_;} /**< Get the node's id (for the debug purposes) */

        /**
         * Routine for AST traversal
         * @param Visitor The visitor class type
         */
        template <class Visitor> void visit(Visitor &visitor);

        bool isLeaf(){ return children_.size() == 0;} /**< Check that the node is a leaf   */
        bool isRoot(){ return parent_ == 0;}          /**< Check that the node is the root */
        
        Token token() const { return token_;} /**< Get the associated token */
        void setToken( Token t) { token_ = t;}/**< Set the associated token */
    private:
        /* No copy constructor or asignment available */
        ASTNode( ASTNode &n){}
        ASTNode &operator = (ASTNode &n)
        {
            if ( &n != this)
            {
                *this = n;
            }
            return *this;
        }
        /** Create unique id for the newly created node */
        UInt32 makeNewId()
        {
            return next_id++;
        }

        /* Tree implementation data */
        ASTNode *parent_;
        NodeList children_;

        /* Debug info */
        UInt32 id_;
        static UInt32 next_id;

        /* Saved token */
        Token token_;
    };
    
    //Definition for the next_id field
    template <class Token> UInt32 ASTNode<Token>::next_id = 0;

    /**
     * AST Visitor base class
     * @ingroup STX
     */
    template <class Node> class ASTVisitor
    {
    public:
        virtual void doPre( Node *n) {}
        virtual void doPost( Node *n){}
    };

    /**
     * Class for AST priniting
     * @ingroup STX
     */
    template <class Node> class ASTPrinter: public ASTVisitor< Node>
    {
        std::ostream &stream;
    public:
        ASTPrinter( std::ostream& s): stream( s){};
        virtual void doPre( Node *n) { stream << "{ " << *n;}
        virtual void doPost( Node *n){ stream << '}';}
    };

    /**
     * Routine for AST traversal
     */
    template <class Token> 
        template <class Visitor> void ASTNode<Token>::visit( Visitor &visitor)
    {
        visitor.doPre( this);
        bool first = true;
        for ( typename std::list< ASTNode* >::iterator it = childrenBeg(),
                                                end = childrenEnd();
                it != end;
                ++it)
        {
             (*it)->visit<Visitor>( visitor);
        }
        visitor.doPost( this);
    }

    /**
     * AST constructor. Makes the new node a child of the given parent node
     */
    template <class Token> ASTNode< Token>::ASTNode( Token t, ASTNode *parent_node):
        token_( t), id_( makeNewId() ) 
    {
        connect( parent_node);
    }

    /**
     * AST destructor. Deletes all the children nodes as well
     */
    template <class Token> ASTNode< Token>::~ASTNode()
    {
        for ( iterator it = childrenBeg(), end = childrenEnd();
              it != end;
              ++it)
        {
             delete (*it);
        } 
    }

    /**
     * Makes the node a child of the given parent node
     * @param n The parent node
     */
    template <class Token> void ASTNode< Token>::connect( ASTNode *n)
    {
         parent_ = n;
         if ( n)
         {
            n->children_.push_back( this);
         }
    }
    
    /**
     * Remove the given child node from the list of this node's children
     */
    template <class Token> void ASTNode< Token>::removeChild( ASTNode* n)
    {
        children_.remove( n);
    }
    
    /**
     * Disconnect from the parent. The node's children remain connected to the node.
     */
    template <class Token> void ASTNode< Token>::disconnect()
    {
        if ( parent_)
            parent_->removeChild( this);
        parent_ = 0;
    }

    /** 
     * Dump the node to the given stream
     */
    template <class Token> void ASTNode< Token>::toStream( std::ostream &stream) const
    {
        stream 
            //<< id() << ": "
            << token_;
    }
} // Syntax
