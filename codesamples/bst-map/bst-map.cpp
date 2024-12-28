       
       /************************************************************
        * @file   bst-map.cpp
        * @author Evan Gray
        * 
        * @brief: Implemetation of a binary search tree map
       *************************************************************/
       #include "bst-map.h"
       
       //Macros to reduce repeated code
       #define BST BSTmap&ltKEY_TYPE, VALUE_TYPE>
       #define BSTI BST::BSTmap_iterator
       #define BSTN BST::Node
       #define tyBST typename BST
       #define tyBSTI typename BSTI
       
       namespace CS280 
       {
         //Static data members
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::BSTmap_iterator BST::end_it = BST::BSTmap_iterator(nullptr);
       
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::BSTmap_iterator_const BST::const_end_it = BST::BSTmap_iterator_const(nullptr);
       
         //--------------------------------------------------------------------------------------
         //Node functions------------------------------------------------------------------------
         //--------------------------------------------------------------------------------------
         /************************************************************
          * @brief: Construct a new Node
          * 
          * @param k: key 
          * @param val: value
          * @param p: parent node pointer
          * @param l: left node pointer
          * @param r: right node pointer
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE> 
         BSTN::Node(KEY_TYPE k, VALUE_TYPE val, Node* p, Node* l, Node* r)
           : key(k), value(val), parent(p), left(l), right(r)
         {}
       
         /************************************************************
          * @brief: Gets the key value of this node
          * 
          * @return KEY_TYPE const&: They key value of this node
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         KEY_TYPE const& BSTN::Key() const
         {
           return this->key;
         }
       
         /************************************************************
          * @brief: Gets the value of this node
          *    
          * @return VALUE_TYPE&: The value of this node
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         VALUE_TYPE& BSTN::Value()
         {
           return this->value;
         }
       
         /************************************************************
          * @brief: Gets the first node of this node by branching as
          *         far left as possible in this node
          *      
          * @return tyBST::Node*: First node
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::Node* BSTN::first()
         {
           //If this is as far left as possible
           if(this->left == NULL)
           {
             return this;
           }
       
           //Get first of left
           return this->left->first();
         }
       
         /************************************************************
          * @brief: Gets the last node of this node by branching as
          *         far right as possible in this node
          *               
          * @return tyBST::Node*: Last node
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::Node* BSTN::last()
         {
           //If this is as far right as possible
           if(this->right == NULL)
           {
             return this;
           }
       
           //Get last of right
           return this->right->last();
         }
       
         /************************************************************
          * @brief: Go one node forward, as in to the node with the
          *         closest greater key
          *             
          * @return tyBST::Node*: One node forward from passed node
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::Node* BSTN::increment()
         {
           //Go right if possible since always larger
           if(this->right != NULL)
           {
             //Get first of the right side
             return this->right->first();
           }
       
           //Go to parent if it exists
           if(this->parent != NULL)
           {
             //Loop up through all parents in tell larger
             Node* current = this->parent;
             while (current != NULL && current->key &lt this->key)
             {
               current = current->parent; 
             }
       
             return current;
           }
       
           //Return NULL if no option found
           return NULL;
         }
       
         /************************************************************
          * @brief: Go one node backwards, as in to the node with the
          *         closest lesser key
          *        
          * @return tyBST::Node*: One node backwards from passed node
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::Node* BSTN::decrement()
         {
           //Go left if possible since always smaller
           if(this->left != NULL)
           {
             //Loop down through all rights for the left
             return this->left->last();
           }
       
           //Go to parent if it exists
           if(this->parent != NULL)
           {
             //Loop up through all parents in tell smaller
             Node* current = this->parent;
             while (current != NULL && current->key > this->key)
             {
               current = current->parent; 
             }
       
             return current;
           }
       
           //Return NULL if no option found
           return NULL;
         }
       
         //--------------------------------------------------------------------------------------
         //Interator functions-------------------------------------------------------------------
         //--------------------------------------------------------------------------------------
         /************************************************************
          * @brief: Construct a new bstmap iterator
          *          
          * @param p: Pointer to node to set as p_node
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         BSTI::BSTmap_iterator(Node* p)
           : p_node(p)
         {}
       
         /************************************************************
          * @brief: Construct a new bstmap iterator from another
          *         iterator
          *        
          * @param rhs: Iterator to copy from
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         BSTI::BSTmap_iterator(const BSTmap_iterator& rhs)
           : p_node(rhs.p_node)
         {}
       
         /************************************************************
          * @brief: Assignment operator for interators
          *   
          * @param rhs: Right hand side of operation
          * @return tyBSTI&: Self after operation
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBSTI& BSTI::operator=(const BSTmap_iterator& rhs)
         {
           //Set p_node's value to value of rhs
           this->p_node = rhs.p_node;
           return *this;
         }
         
         /************************************************************
          * @brief: PlusPlus postfix operator for iterators, 
          *         increments the iterator by one
          *         
          * @return tyBSTI&: Self after operation
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBSTI& BSTI::operator++()
         {
           //Move one node forward
           p_node = p_node->increment();
           return *this;
         }
       
         /************************************************************
          * @brief: PlusPlus prefix operator for iterators,
          *         increments the iterator by one
          *          
          * @return tyBSTI: Self before operation
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBSTI BSTI::operator++(int)
         {
           //Make a copy of the original
           BSTmap_iterator orignal(*this);
       
           //Increment iterator
           ++(*this);       
       
           //Return original value
           return orignal;
         }
       
         /************************************************************
          * @brief: Dereference operator for iterator, gets reference
          *         to node in iterator
          *       
          * @return tyBST::Node&: Node in iterator
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::Node& BSTI::operator*()
         {
           return *(this->p_node);
         }
       
         /************************************************************
          * @brief: Arrow operator for iterator, gets pointer to node
          *         in iterator
          *          
          * @return tyBST::Node*: Pointer to node in iterator
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::Node* BSTI::operator->()
         {
           return this->p_node;
         }
       
         /************************************************************
          * @brief: Inverse Equality operator for iterators, checks
          *         if nodes dont mathc
          *         
          * @param rhs: Right hand side of operation
          * @return true: Nodes dont match
          * @return false:  Nodes match
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         bool BSTI::operator!=(const BSTmap_iterator& rhs)
         {
           //Check if pointers dont match
           return !(this->p_node == rhs.p_node);
         }
       
         /************************************************************
          * @brief: Equality operator for iterators, checks if nodes
          *         match
          *      
          * @param rhs: Right hand side of operaton
          * @return true: Nodes match
          * @return false: Nodes dont match
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         bool BSTI::operator==(const BSTmap_iterator& rhs)
         {
           //Check if pointers match
           return this->p_node == rhs.p_node;
         }
       
         //---------------------------------------------------------------------------------------
         //BSTmap Constructors/Destructors--------------------------------------------------------
         //---------------------------------------------------------------------------------------
         /************************************************************
          * @brief: Construct a new BSTmap   
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         BST::BSTmap()
         {}
       
         /************************************************************
          * @brief: Construct a new BSTmap from another BSTmap
          *       
          * @param rhs: BSTmap to copy from
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         BST::BSTmap(const BSTmap& rhs)
         {
           //Add all nodes from rhs
           addAll(rhs.pRoot);
         }
       
         /************************************************************
          * @brief: Destroy BSTmap, frees all nodes      
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         BST::~BSTmap()
         {
           //Free all nodes
           freeAll(&pRoot);
         }
       
         //---------------------------------------------------------------------------------------
         //BSTmap Members and operators-----------------------------------------------------------
         //---------------------------------------------------------------------------------------
         /************************************************************
          * @brief: Assignment operator for BSTmaps
          *         
          * @param rhs: Right hand side of the operation
          * @return tyBST::BSTmap&: Self after operation
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::BSTmap& BST::operator=(const BSTmap& rhs)
         {
           //Empty and replace with rhs
           freeAll(&pRoot);
           addAll(rhs.pRoot);
       
           //Return self
           return *this;
         }
       
         /************************************************************
          * @brief: Operator brackets for BSTmap, gets reference to
          *         node with passed key (created if not found)
          * 
          * @param key: Key to use to find node
          * @return VALUE_TYPE&: Reference to node with passed key
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         VALUE_TYPE& BST::operator[](KEY_TYPE const& key)
         {
           //Search for the key in the binary search tree
           Node* last;
           Node* node = findNode(key, &last);
       
           //If the key was not found, insert it into the tree
           if (node == NULL) 
           {
             //Insert node into the tree (with defualt value and no branches)
             node = new Node(key, VALUE_TYPE(), last, -1, -1, NULL, NULL);
             addNode(last, node);
           }
       
           //Return a reference to the value associated with the key
           return node->value;
         }     
         
         /************************************************************
          * @brief: Gets the start of this BSTmap as an iterator, 
          *         returns end if no starting node
          *    
          * @return tyBST::BSTmap_iterator: Begining of this BSTmap
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::BSTmap_iterator BST::begin() 
         {
           //If the root exists
           if (pRoot) 
           {
             //Return it as an iterator
             return BST::BSTmap_iterator(pRoot->first());
           }
           else
           {
             return end_it;
           }
         }
       
         /************************************************************
          * @brief: Returns the end of this BSTmap (which is always
          *         NULL)
          *        
          * @return tyBST::BSTmap_iterator: End of this BSTmap
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::BSTmap_iterator BST::end()
         {
           return BST::end_it;
         }
       
         /************************************************************
          * @brief: Finds node with passed key in BSTmap
          *     
          * @param key: Key of node to find
          * @return tyBST::BSTmap_iterator: Iterator to wanted node 
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::BSTmap_iterator BST::find(KEY_TYPE const& key)
         {
           //Find node with passed key
           Node* node = findNode(key);
       
           //If node was found
           if(node != NULL)
           {
             //Return interator to it
             return BSTmap_iterator(node);
           }
       
           //Return end of list if failed
           return this->end();
         } 
       
         /************************************************************
          * @brief: Gets the size of this BSTmap, which is the number
          *         of nodes in the map
          *      
          * @return unsigned int: Number of nodes in this BSTmap
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         unsigned int BST::size()
         {
           //Return the size of the map (number of nodes)
           return size_;
         }
       
         /************************************************************
          * @brief: Erased node at passed iterator from BSTmap
          *     
          * @param it: Iterator to node to erase
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         void BST::erase(BSTmap_iterator it)
         {
           //Get node out of interator
           Node* node = it.p_node;
       
           //If node has no branches
           if(node->right == NULL && node->left == NULL)
           {
             //Update the parent node to NULL
             updateParent(node, NULL);
       
             //Delete this node
             freeNode(&node);
           }
           //If node only has right branch
           else if (node->left == NULL)
           {
             //Update the parent node of right branch
             updateParent(node, node->right);
       
             //Delete original node
             freeNode(&node);
           }
           //If node only has left branch
           else if (node->right == NULL)
           {
             //Update the parent node of left branch
             updateParent(node, node->left);
       
             //Delete original node
             freeNode(&node);
           }
           //If node has two branches
           else
           {
             //Get predecessor from node below self
             Node* pred = node->left->last();
       
             //Set node to pred's data
             node->key = pred->key;
             node->value = pred->value;
       
             //Call erase on pred
             erase(BSTmap_iterator(pred));
           }
         }

         /************************************************************
          * @brief: Finds node of passed key in this BSTmap, returns
          *         NULL if not found, sets last as the node before
          *         found node (even if not found), Last is not 
          *         required and can be left NULL
          *       
          * @param key: Key to search for
          * @param last: Will be set to the node beforefound node 
          *              (even if not found), NULL by defualt
          * @return tyBST::Node*: Pointer to Node with passed key,
          *                       NULL if not found
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         tyBST::Node* BST::findNode(KEY_TYPE const& key, Node** last) const
         {
           //Search for the key in the binary search tree
           Node* current = pRoot;
           Node* lastNode = NULL;
           while (current != NULL && current->key != key) 
           {
             //Set lastNode
             lastNode = current;
       
             //Go left or right
             if (key > current->key) 
             {
               current = current->right;
             } 
             else 
             {
               current = current->left;
             }
           }
       
           //Set last if needed
           if(last != NULL)
           {
             *last = lastNode;
           }
       
           return current;
         }
       
         /************************************************************
          * @brief: Adds a node to this BSTmap after nodeToAddFrom in
          *         the proper direction
          * 
          * @param nodeToAddFrom: Node to add new node after
          * @param nodeToAdd: Node to add
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         void BST::addNode(Node* nodeToAddFrom, Node* nodeToAdd)
         {
           //Increment size
           ++size_;
       
           //If root is not set
           if(pRoot == NULL)
           {
             //Set as root and update end iterator
             pRoot = nodeToAdd;
           }
           else
           {
             //If larger than node adding from 
             if (nodeToAdd->key > nodeToAddFrom->key) 
             {
               //Add to the right and update end iterator
               nodeToAddFrom->right = nodeToAdd;
             } 
             else 
             {
               //Add to the left
               nodeToAddFrom->left = nodeToAdd;
             }
           }
         }
       
         /************************************************************
          * @brief: Frees memory of passed node
          * 
          * @param node: Node to free
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         void BST::freeNode(Node** node)
         {
           //Delete node and set to null
           delete *node;
           *node = NULL;
           --size_;
         }
       
         /************************************************************
          * @brief: Updates parent data of oldChild to have instead
          *         have newChild, also handles if parent is root,  
          *         and updating parent pointers of children
          *     
          * @param oldChild: Child of parent to update
          * @param newChild: Child to replace oldChild
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         void BST::updateParent(Node* oldChild, Node* newChild)
         {
           //If parent exists
           if(oldChild->parent != NULL)
           {
             //Set parent pointing to self to new node
             if (oldChild->parent->left == oldChild)
             {
               oldChild->parent->left = newChild;
             }
             else
             {
               oldChild->parent->right = newChild;
             }
           }
           //No parent, so oldChild is root
           else
           {
             //Set root to newChild
             pRoot = newChild;
           }
       
           //If new child exists
           if(newChild != NULL)
           {
             //Update its parent
             newChild->parent = oldChild->parent;
           }
       
           //Delete old child's parent
           oldChild->parent = NULL;
         }
       
         /************************************************************
          * @brief: Frees all nodes in this BSTmap after passed node
          *          
          * @param node: Starting point of freeALL
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         void BST::freeAll(Node** node)
         {
           //If this node exists
           if(*node != NULL)
           {
             //Free children
             freeAll(&((*node)->left));
             freeAll(&((*node)->right));
       
             //Free self
             freeNode(node);
           }
         }
       
         /************************************************************
          * @brief: Copies and adds all nodes starting at passed node, 
          *         lastNode is used for recursion
          *     
          * @param node: Starting point for copying and adding
          * @param lastNode: Parent of currently adding node, leave 
          *                  as defualt (NULL) when calling this
          *                  function
         *************************************************************/
         template&lttypename KEY_TYPE, typename VALUE_TYPE>
         void BST::addAll(Node* node, Node* lastNode)
         {
           //If this node exists
           if(node != NULL)
           {
             //Add self
             Node* newNode = new Node( node->key, node->value, lastNode, -1, -1, NULL, NULL);
             addNode(lastNode, newNode);
       
             //Add children
             addAll(node->left, newNode);
             addAll(node->right, newNode);
           }
         }
       }