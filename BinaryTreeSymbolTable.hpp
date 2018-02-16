/***************************************************************
<Austin Gladfelter>
<CS_210_program3>
<Program 3>
*This is a representation of an ordered symbol table of key-value pairs.
*This code implements functions put, get, remove, contains, empty, 
*size, min, max, floor, ceiling, rank, select, deleleMin, deleteMax.
*Every function is implemented with a helper function named with the
*consisten style of "functionHelper"
*The funcitons and their helpers are grouped as the following (in order)
*{put, putHelper}, {get, getHelper}, {remove, removeHelper},
*{contains, containsHelper}, {empty}, {size, size, sizeHelper},
*{min, minHelper}, {max, maxHelper}, {floor, floorHelper}, {ceiling, ceilingHelper},
*{rank, rankHelp}, {select, selectHelper}, {deleteMin, deleteMinHelper},
*{deleteMax, deleteMaxHelper} and vectors for keys.																			
*Note empty() does not have a helper and there are two size functions.
*Other functions include keys() that is a method for iterating over all of the keys.

*Using these functions this program implements a regular (unbalenced) binary seach tree.
*The upper bound on the time complexity for this implementation in linear due to the
*tree being unbalanced (there are no rotations or recoloring, all link are the same).
*uses Structer BinaryTreeNode
*put, get, and contains are linear O(n) operations
*the rest are O(1).

**************************************************************/

#ifndef TREE_SYMBOL_TABLE_H
#define TREE_SYMBOL_TABLE_H

#include <algorithm>

#include "SymbolTable.hpp"

template <typename Key, typename Value>
class BinaryTreeSymbolTable : public SymbolTable<Key, Value>
{
protected:

	struct BinaryTreeNode
	{
		Key _key;
		Value _value;
		BinaryTreeNode* _left;
		BinaryTreeNode* _right;
		unsigned _size;

		BinaryTreeNode(const Key& key = Key{},
			const Value& value = Value{},
			unsigned size = 0,
			BinaryTreeNode* ell = nullptr,
			BinaryTreeNode* r = nullptr)
			: _key{ key }, _value{ value }, _size{ size }, _left{ ell }, _right{ r } {}

		BinaryTreeNode(const BinaryTreeNode& that)
			: _key{ that._key }, _value{ that._value }, _size{ that._size }, _left{ that._left }, _right{ that._right } {}

		~BinaryTreeNode()
		{
			if (_left != nullptr)
			{
				delete _left;
				_left = nullptr;
			}
			if (_right != nullptr)
			{
				delete _right;
				_right = nullptr;
			}
			_size = 0;
		}
	};

	// Key value comparison (less than)
	bool keyLessThan(const Key& lhs, const Key& rhs) const { return lhs < rhs; }

	// Equality of key values
	bool keyEquals(const Key& lhs, const Key& rhs) const { return lhs == rhs; }

	// Equality of key values
	bool keyLessThanOrEquals(const Key& lhs, const Key& rhs) const
	{
		return keyEquals(lhs, rhs) || keyLessThan(lhs, rhs);
	}

	// The container of the <key, value> pairs
	BinaryTreeNode* _root;

public:

	BinaryTreeSymbolTable() : _root{ nullptr } {}

	virtual ~BinaryTreeSymbolTable() { delete _root; }

	/////////////////////////////////////////////
	//			FUNCTIONS AND HELPERS		   //
	/////////////////////////////////////////////


	// Puts key-value pair into the table
	//calls on out helper using key can val from root
	virtual void put(const Key& key, const Value& val = Value{})
	{
		_root = putHelper(_root, key, val);
	}
private:
	//Helper function for put()
	//Uses new BinaryTreeNode and pairs keys with values as nodes
	BinaryTreeNode* putHelper(BinaryTreeNode* node, const Key& key, const Value& value)
	{
		//If there is no node, create new BinaryTreeNode and it will be the root of the tree
		if (node == nullptr) return new BinaryTreeNode(key, value, 1);

		//If the key is less than the node, put it to the left
		else if (keyLessThan(key, node->_key)) node->_left = putHelper(node->_left, key, value);

		//If it is greater than the node, put it in the right
		else if (keyLessThan(node->_key, key)) node->_right = putHelper(node->_right, key, value);

		// Duplicate: overwrite the value
		else node->_value = value; 

		//increase size accordingly
		node->_size = 1 + size(node->_left) + size(node->_right);

		return node;
	}

public:
	// acquire the value paired with key
	virtual bool get(const Key& key, Value& val = Value{}) const
	{
		//using new BinaryTreeNod called result that calls the getHelper
		BinaryTreeNode* result = getHelper(_root, key, val);

		//returns false if result is a null pointer
		if (result == nullptr)
		{
			return false;
		}

		//if not, what result points to is stored back into val
		else
		{
			val = result->_value;
			return true;
		}
	}
private:
	//helper for get()
	BinaryTreeNode* getHelper(BinaryTreeNode* node, const Key& key, const Value& value) const
	{
		//return the BinaryTreeNode if node is a null pointer
		if (node == nullptr)
		{
			return node;
		}

		//put key to the left
		else if (keyLessThan(key, node->_key))
		{
			getHelper(node->_left, key, value);
		}

		//put key to the right
		else if (keyLessThan(node->_key, key))
		{
			getHelper(node->_right, key, value);
		}

		else
		{
			return node;
		}
	}


public:
	// remove key (and its value) from table
	// callas the remove helper function
	virtual void remove(const Key& key)
	{
		_root = removeHelper(_root, key);
	}
private:
	virtual BinaryTreeNode* removeHelper(BinaryTreeNode* node, const Key& key)
	{

		// if the node is a null pointer, return the node
		if (node == nullptr)
		{
			return node;
		}
		//if the key we trying to remove is smaller than the node we are at, move right
		else if (keyLessThan(node->_key, key))
		{
			node->_right = removeHelper(node->_right, key);
		}
		//if the key we trying to remove is greater than the node we are at, move left
		else if (keyLessThan(key, node->_key))
		{
			node->_left = removeHelper(node->_left, key);
		}
		
		else
		{
			//if the node has no child
			if (node->_right == nullptr)
			{
				BinaryTreeNode* temp = node->_left;
				free(node);
				return temp;
			}

			//if the node has only one child
			if (node->_left == nullptr)
			{
				BinaryTreeNode* temp = node->_right;
				free(node);
				return temp;
			}
			

			else
			{
				// If the node has two children, then get the next node
				BinaryTreeNode* temp = minHelper(node->_right);

				// Replace the content of current node with next node
				node->_key = temp->_key;
				// delete the inorder successor
				node->_right = removeHelper(node->_right, temp->_key);
			}
		}
        
        //update the sizes of the nodes at it iterates back up the table
		node->_size = size(node->_right)+ size(node->_left) + 1 ;
		return node;

	}


public:
	virtual bool contains(const Key& key) const //= 0;
	{
		return containsHelper(_root, key);
	}
private:
	//helper for contains
	virtual bool containsHelper(BinaryTreeNode* node, const Key& key) const
	{
		if (node == nullptr)
		{
			return false;
		}
        //moving right
        else if (keyLessThan(node->_key, key))
        {
            containsHelper(node->_right, key);
        }
        
        //moving left
        else if (keyLessThan(key, node->_key))
		{
			containsHelper(node->_left, key);
		}

		else 
		{ 
			return true; 
		}
	}


	//is the table empty?
public:
	virtual bool empty() const
	{
        //recurses throught the root and finds if a node exists
		if (_root == nullptr) return true;
        
        //if there is no node at the root, the table is empty
		else
		{
			return false;
		}
	}


	// Number of key-value pairs.
	virtual int size() const 
	{
        //exit statement
		if (_root == nullptr)
		{
			return 0;
		}
		else
		{
            //calls the utility size function below
			return _root->_size;
		}
	}

	//utility function for size
private:
	int size(const BinaryTreeNode* const node) const { return node == nullptr ? 0 : node->_size; }


	//smallest key
public:
	virtual bool min(Key& key = Key{}) const
	{
        //exit statement
		BinaryTreeNode* temp;
		if (_root == nullptr)
		{
			return false;
		}
        
        //calls the minHelper function
		else
		{
			temp = minHelper(_root);
			key = temp->_key;
			return true;
		}
	}
    
	//helper function for min
	//takes in a node and finds the furthest left node from it
private:
	BinaryTreeNode* minHelper(BinaryTreeNode* node) const
	{
		if (node->_left != nullptr)
		{
            //traversing through the table to the left
			node = minHelper(node->_left);
		}
		return node;
	}

public:
	// Largest key
    //does the same thing as min but to the right
	virtual bool max(Key& key = Key{}) const
	{
        //exit statement
		BinaryTreeNode*  temp;
		if (_root == nullptr)
		{
			return false;
		}

		else
		{
            //calls maxHelper function
			temp = maxHelper(_root);
			key = temp->_key;
			return true;
		}
	}
private:
	//helper function for max()
	BinaryTreeNode* maxHelper(BinaryTreeNode* node) const
	{
        //exit statement
		if (node->_right != nullptr)
		{
            //traverses to the right to find the largest node
			node = maxHelper(node->_right);
		}

		return node;
	}


public:
	//floor and ceiling
	//functions and helpers are seperate for others but for these two I put them next to each other for readabilty for this specific issue.
	// Largest key less than or equal to key
	virtual bool floor(const Key& key, Key& floorKey) const
	{
		const BinaryTreeNode* keyNode;
		keyNode = floorHelper(_root, key);
        //exit, null statement
        if (keyNode == nullptr) return false;
        
        //uses floorKey to store new key as floor
		else
		{
			floorKey = keyNode->_key;
			return true;
		}
	}

private:
    //helper function for floor
	// Largest key less than or equal to key
	virtual BinaryTreeNode* floorHelper(BinaryTreeNode* node, const Key& key) const
	{
        //exit statement
        if (node == nullptr) return node;
        
        //recurses through the table to find floor
        else if (keyLessThan(key, node->_key)) return floorHelper(node->_left, key);

        
        else if (keyEquals(node->_key, key)) return node;


		else
		{
            //uses another new BinaryTreeNode pointer and stores correct value in
			BinaryTreeNode* floor = floorHelper(node->_right, key);

            if (floor == nullptr) return node;
			
            if (keyLessThanOrEquals(floor->_key, key)) return floor;

            else return node;

		}

	}


public:
	// Smallest key greater than or equal to key
	virtual bool ceiling(const Key& key, Key& ceilingKey) const
	{
        //uses new pointer from BinaryTreeNode called keyNode
		const BinaryTreeNode* keyNode;
        //calls ceiling helper
		keyNode = ceilingHelper(_root, key);
		if (keyNode == nullptr)
		{
			return false;
		}
		else
		{
			ceilingKey = keyNode->_key;
			return true;
		}
	}
	// Smallest key greater than key
    //iteratates up and down the tree and finds the smallest node great than the key
private:
	virtual BinaryTreeNode* ceilingHelper(BinaryTreeNode* node, const Key& key) const
	{
        //exit statement
		if (node == nullptr)
		{
			return node;
		}
        //node found
		else if (keyEquals(node->_key, key))
		{
			return node;
		}
        //right
		else if (keyLessThan(node->_key, key))
		{
			return ceilingHelper(node->_right, key);
		}
		else
		{
            //left
			BinaryTreeNode* ceil = ceilingHelper(node->_left, key);
			if (ceil == nullptr)
			{
				return node;
			}
			
            //returns the pointer
			else
			{
				return ceil;
			}

		}

	}


	// Number of keys less than key
public:
	virtual int rank(const Key& key) const
	{
		return rankHelp(_root, key, 0);
	}
	//helper for rank
private:
	virtual int rankHelp(BinaryTreeNode* node, const Key& key, int rank) const
	{
        //exit statement
		if (node == nullptr)
		{
			return rank;
		}
        
        //if the node is not a null pointer
		else if (node != nullptr)
		{
            //new node pointer rank
			rank = rankHelp(node->_left, key, rank);

			if (keyLessThan(node->_key, key))
			{
                //increment rank
				rank++;
			}
            
            //store new rank
			rank = rankHelp(node->_right, key, rank);
		}

		return rank;
	}


public:
	// key of rank k
	virtual bool select(int k = 0, Key& key = Key{}) const
	{
		BinaryTreeNode* selectNode = selectHelper(_root, k);
		
            //exit statement
			if (selectNode == nullptr)
			{
				return false;
			}

			else
			{
                //new key for pointer
				key = selectNode->_key;
				return true;
			}
		
	}
    
        //helper function for help
private:
	BinaryTreeNode* selectHelper(BinaryTreeNode* node, int k) const
	{
        //exit statement
		if (node == nullptr) return nullptr;

        //size of left subtree
		int leftSize = size(node->_left);

        //leftsize greater than k, store into node
		if (leftSize > k)
		{
			return selectHelper(node->_left, k);
		}

        //smaller than k, store into node
		else if (leftSize < k)
		{
			return selectHelper(node->_right, k - leftSize - 1);
		}

		else
		{
			return node;
		}
	}



public:
	// Delete the smallest key
	virtual bool deleteMin()
	{
        //exit statement
		if (_root == nullptr)
		{
			return false;
		}

        //calls helper function for delete
		else
		{
			_root = delMinHelper(_root);
			return true;
		}
	}
    //helper function for min
private:
	BinaryTreeNode* delMinHelper(BinaryTreeNode* node) const
	{
        //exit statement, just return the node
		if (node == nullptr)
		{
			return node;
		}
        
        //left node not null pointer
		else if (node->_left != nullptr)
		{
			node->_left = delMinHelper(node->_left);
		}

		else
		{
            //if no left child
			if (node->_left == nullptr)
			{
                //create new node using pointer
				BinaryTreeNode* temp = node->_right;
                //free is the only way I could get to delete the desired node properly
				free(node);
				return temp;
			}
		}
        
        //iterating back up the table
		node->_size = 1 + size(node->_left) + size(node->_right);
		return node;
	}


public:
	// Delete the largest key
	virtual bool deleteMax()
	{
        //exit statement, null pointer
		if (_root == nullptr)
		{
			return false;
		}
        
        //calls helper function for max
		else
		{
			_root = delMaxHelper(_root);
			return true;
		}
	}
    
    //helper for deleteMax
private:
	BinaryTreeNode* delMaxHelper(BinaryTreeNode* node) const
	{
        //exit statement
		if (node == nullptr)
		{
			return node;
		}
        
        //checks if it is empty to the right
		else if (node->_right != nullptr)
		{
			node->_right = delMaxHelper(node->_right);
		}
		else
		{
			if (node->_right == nullptr)
			{
				BinaryTreeNode* temp = node->_left;
                //again, free was the only method found to delete node properly
				free(node);
				return temp;
			}
		}

        //increment sizes iterating back up the tree
		node->_size = 1 + size(node->_left) + size(node->_right);
		return node;
	}

public:
	// number of keys in [low, high] (including low, high)
	virtual int size(const Key& low, const Key& high) const
	{
        //calls the size helper (different from size utility function
		return sizeHelper(_root, low, high, 0);
	}
    //helper for size
private:
	virtual int sizeHelper(BinaryTreeNode* node, const Key& low, const Key& high, int size) const
	{
		if (node != nullptr)
		{
            //left side
			size = sizeHelper(node->_left, low, high, size);
            
            //increment size if there are children to the parent nodes
			if (keyLessThanOrEquals(low, node->_key) && keyLessThanOrEquals(node->_key, high))
			{
				size++;
			}

            //right side
			size = sizeHelper(node->_right, low, high, size);
		}
		return size;
	}



public:
	// keys in [low, high] (including low, high), in sorted order
	virtual std::vector<Key> keys(const Key& low, const Key& high) const
	{
        //calls keyHelper
		std::vector<Key> keys;
		return keyHelper(_root, low, high, keys);
	}

	// all keys in the table, in sorted order
	virtual std::vector<Key> keys() const
	{
        //could not come up with a proper way to use min and max in this vector
        //runs normally with the large integers placed below
        //calls keyHelper
		std::vector<Key> keys;
		return keyHelper(_root, -100000000, 1000000000, keys);
	}
private:
	//helper for keys
	virtual std::vector<Key> keyHelper(BinaryTreeNode* node, const Key& low, const Key& high, std::vector<Key> keys) const
	{
		if (node != nullptr)
		{
            //pushes node back to vector if the key is less than or equal to the node
			keys = keyHelper(node->_left, low, high, keys);
			if (keyLessThanOrEquals(low, node->_key) && keyLessThanOrEquals(node->_key, high))
			{
				keys.push_back(node->_key);
			}

            //new keys
			keys = keyHelper(node->_right, low, high, keys);
		}
		return keys;
	}
	
//public:
//	// all keys in the table, in sorted order
//	virtual std::vector<Key> keys() const
//	{
//		Key minimum, maximum;
//		min(minimum);
//		max(maximum);
//		return keys(minimum, maximum);
//	}

public:
	// Removes all elements from the table
	virtual void clear()
	{
		if (_root != nullptr) delete _root;
		_root = nullptr;
	}

private:
	// Returns the height of the BST (for debugging).
	// @return the height of the BST (a 1-node tree has height 0)
	int height() const { return height(root); }
	int height(BinaryTreeNode* node) const
	{
		if (node == nullptr) return -1;

		return 1 + std::max(height(node->_left), height(node->_right));
	}

	// Non-recursive node deletion.
	BinaryTreeNode* Delete(BinaryTreeNode* node) const
	{
		node->_left = nullptr;
		node->_right = nullptr;
		delete node;
		node = nullptr;
		return node;
	}

	//
	///////////////////////////////////////////////////////////////////////////////
	// Check integrity of BST data structure.
	///////////////////////////////////////////////////////////////////////////////
	//
	bool check() const
	{
		if (!isBST())            std::cout << "Not in symmetric order" << std::endl;
		if (!isSizeConsistent()) std::cout << "Subtree counts not consistent" << std::endl;
		if (!isRankConsistent()) std::cout << "Ranks not consistent" << std::endl;

		return isBST() && isSizeConsistent() && isRankConsistent();
	}

	//
	// does this binary tree satisfy symmetric order?
	// Note: this test also ensures that data structure is a binary tree since order is strict
	//
	bool isBST() const { return isBST(_root, nullptr, nullptr); }
	// is the tree rooted at x a BST with all keys strictly between min and max
	// (if min or max is null, treat as empty constraint)
	bool isBST(BinaryTreeNode* node, Key* min, Key* max) const
	{
		if (node == nullptr) return true;

		if (min != nullptr && keyLessThanOrEquals(node->_key, *min)) return false;

		if (max != nullptr && keyLessThanOrEquals(*max, node->_key)) return false;

		return isBST(node->_left, min, &(node->_key)) && isBST(node->_right, &(node->_key), max);
	}

	// are the size fields correct?
	bool isSizeConsistent() const { return isSizeConsistent(_root); }
	bool isSizeConsistent(BinaryTreeNode* node) const
	{
		if (node == nullptr) return true;

		// The size of this need must equate to the sum of its children (plus itself)
		if (node->_size != size(node->_left) + size(node->_right) + 1) return false;

		return isSizeConsistent(node->_left) && isSizeConsistent(node->_right);
	}

	// check that ranks are consistent
	bool isRankConsistent() const
	{
		// The i th node should be rank i
		for (int i = 0; i < size(); i++)
		{
			Key key;
			select(i, key);
			if (i != rank(key)) return false;
		}

		// All keys must equate to the key acquired at its rank 
		for (Key key : keys())
		{
			Key acquired;
			select(rank(key), acquired);

			if (!keyEquals(key, acquired)) return false;
		}

		return true;
	}
};

#endif