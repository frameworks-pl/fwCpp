#ifndef FW_BINARYTREE_H_INCLUDED
#define FW_BINARYTREE_H_INCLUDED

namespace fw
{
	namespace core
	{
		class BTreeNode
		{
			public:
				BTreeNode() : left(NULL), right(NULL) { }
				virtual ~BTreeNode() { }
				virtual void addNode(BTreeNode* node);

				//returns node parent node for the provided node
				virtual bool isGreater(const BTreeNode* node) const = 0;

				//returns a node that is closest lower boundory for the new value 
				//for example if we search for value 42 and there is node 41, it should be returned as result
				//--->41<------|42|--------------80
				BTreeNode* findParent(const BTreeNode* needle);

				BTreeNode* getLeft() { return left; }
				BTreeNode* getRight() { return right; }

				void setLeft(BTreeNode* node);
				void setRight(BTreeNode* node);
			protected:
				
			private:
				BTreeNode* left;
				BTreeNode* right;
		};

		class BinaryTree
		{
			public:
				virtual ~BinaryTree() { }
				BinaryTree() : rootNode(NULL) { }
				void addNode(BTreeNode* node);
				BTreeNode* getRootNode() { return rootNode; }
				//BTreeNode* findParent(BTreeNode* node);

			protected:
				BTreeNode* rootNode;

		};
	}

};



#endif //FW_BINARYTREE_H_INCLUDED