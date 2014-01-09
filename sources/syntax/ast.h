/**
 * @file:syntax/ast.h
 * Implementation of abstract syntax tree
 * @ingroup STX
 */

#pragma once


namespace Syntax
{
    /**
     * Node of abstract syntax tree
     */
    template <class Token> class ASTNode
    {
        typedef typename std::list< ASTNode* > NodeList;
        typedef typename NodeList::iterator iterator;
    public:
        void toStream( std::ostream &stream);
        ASTNode( Token t = Token(), ASTNode *parent_node = 0);
        virtual ~ASTNode();
 
        void connect( ASTNode *parent_node);
        void disconnect();
        void removeChild( ASTNode* n);
        inline ASTNode* parent(){ return parent_;};
        
        inline iterator childrenBeg()
            { return children_.begin();};
        inline iterator childrenEnd()
            { return children_.end();};
        inline UInt32 id(){ return id_;}

        template <class Visitor> void visit(Visitor &visitor);

        bool isLeaf(){ return children_.size() == 0;}
        bool isRoot(){ return parent_ == 0;}
        Token token() const { return token_;}
        void setToken( Token t) { token_ = t;}
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
    
    template <class Token> UInt32 ASTNode<Token>::next_id = 0;


    template <class Node> class ASTVisitor
    {
    public:
        virtual void doPre( Node *n) {}
        virtual void doPost( Node *n){}
    };

    template <class Node> class ASTPrinter: public ASTVisitor< Node>
    {
        std::ostream &stream;
    public:
        ASTPrinter( std::ostream& s): stream( s){};
        virtual void doPre( Node *n) { stream << "{ " << *n;}
        virtual void doPost( Node *n){ stream << '}';}
    };

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

    template <class Token> ASTNode< Token>::ASTNode( Token t, ASTNode *parent_node):
        token_( t), id_( makeNewId() ) 
    {
        connect( parent_node);
    }

    template <class Token> ASTNode< Token>::~ASTNode()
    {
        for ( iterator it = childrenBeg(), end = childrenEnd();
              it != end;
              ++it)
        {
             delete (*it);
        } 
    }

    template <class Token> void ASTNode< Token>::connect( ASTNode *n)
    {
         parent_ = n;
         if ( n)
         {
            n->children_.push_back( this);
         }
    }
    
    template <class Token> void ASTNode< Token>::removeChild( ASTNode* n)
    {
        children_.remove( n);
    }
       
    template <class Token> void ASTNode< Token>::disconnect()
    {
        if ( parent_)
            parent_->removeChild( this);
        parent_ = 0;
    }
    template <class Token> void ASTNode< Token>::toStream( std::ostream &stream)
    {
        stream 
            //<< id() << ": "
            << token_;
    }
    template <class Token> std::ostream & operator << ( std::ostream &stream, ASTNode< Token> &t)
    {
        t.toStream( stream);
        return stream;
    }
} // Syntax
