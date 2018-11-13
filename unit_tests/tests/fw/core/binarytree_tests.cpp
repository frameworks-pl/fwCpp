#include "gtest.h"
#include <list>
#include <src/fw/core/fwbinarytree.h>




class Node : public fw::core::BTreeNode
{
    public:
        Node(int _key, double _value) : key(_key), value(_value) { }
		bool isGreater(const BTreeNode* node) const
		{
			const Node* pNode = dynamic_cast<const Node*>(node);
			if (pNode)
			{
				return pNode->getValue() > value;
			}

			false;
		}
		int getKey() const { return key; }
		double getValue() const { return value;  }
		
	protected:
		int key;
		double value;

};

TEST(BinaryTreeTests, simpleParentChild)
{

    fw::core::BinaryTree searchTree;

    searchTree.addNode(new Node(1, 10.56));
    searchTree.addNode(new Node(2, 12.65));
	searchTree.addNode(new Node(3, 10.00));

	ASSERT_EQ(10.56, ((Node*)searchTree.getRootNode())->getValue());
	ASSERT_EQ(12.65, ((Node*)searchTree.getRootNode()->getRight())->getValue());
	ASSERT_EQ(10.00, ((Node*)searchTree.getRootNode()->getLeft())->getValue());

}

TEST(BinaryTreeTests, lefrightTwoLevelsTest)
{
	fw::core::BinaryTree tree;
	tree.addNode(new Node(1, 100));
	tree.addNode(new Node(2, 50));
	tree.addNode(new Node(3, 150));
	tree.addNode(new Node(4, 90));
	tree.addNode(new Node(5, 40));
	tree.addNode(new Node(6, 120));
	tree.addNode(new Node(6, 200));

	ASSERT_EQ(100, ((Node*)tree.getRootNode())->getValue());
	ASSERT_EQ(50, ((Node*)tree.getRootNode()->getLeft())->getValue());
	ASSERT_EQ(150, ((Node*)tree.getRootNode()->getRight())->getValue());
	ASSERT_EQ(40, ((Node*)tree.getRootNode()->getLeft()->getLeft())->getValue());
	ASSERT_EQ(90, ((Node*)tree.getRootNode()->getLeft()->getRight())->getValue());
	ASSERT_EQ(120, ((Node*)tree.getRootNode()->getRight()->getLeft())->getValue());
	ASSERT_EQ(200, ((Node*)tree.getRootNode()->getRight()->getRight())->getValue());
}

TEST(BinaryTreeTests, findRaneTest)
{
	fw::core::BinaryTree tree;
	tree.addNode(new Node(1, 100));
	Node* needle = new Node(2, 50);
	tree.addNode(needle);
	tree.addNode(new Node(4, 150));
	tree.addNode(new Node(3, 90));
	tree.addNode(new Node(5, 40));
	tree.addNode(new Node(6, 120));
	tree.addNode(new Node(6, 200));

	const fw::core::BTreeNode* parent = tree.getRootNode()->findParent(new Node(2, 60));
	ASSERT_TRUE(needle == parent);



	//ASSERT_EQ(100, ((Node*)tree.getRootNode())->getValue());
	//ASSERT_EQ(50, ((Node*)tree.getRootNode()->getLeft())->getValue());
	//ASSERT_EQ(150, ((Node*)tree.getRootNode()->getRight())->getValue());
	//ASSERT_EQ(40, ((Node*)tree.getRootNode()->getLeft()->getLeft())->getValue());
	//ASSERT_EQ(90, ((Node*)tree.getRootNode()->getLeft()->getRight())->getValue());
	//ASSERT_EQ(120, ((Node*)tree.getRootNode()->getRight()->getLeft())->getValue());
	//ASSERT_EQ(200, ((Node*)tree.getRootNode()->getRight()->getRight())->getValue());
}