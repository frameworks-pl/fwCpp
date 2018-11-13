#include "fwCommon_pch.h"
#include "fwbinarytree.h"

namespace fw
{
    namespace core
    {
		void BinaryTree::addNode(BTreeNode* node)
		{
			if (NULL == rootNode)
			{
				rootNode = node;
			}
			else
			{
				rootNode->addNode(node);
				//BTreeNode* pParent = rootNode->findParent(node);
				//if (pParent)
				//{
				//	pParent->addNode(node);
				//}

				//rootNode->addNode(node);
			}
		}

		void BTreeNode::setLeft(BTreeNode* node)
		{
			left = node;

		}


		void BTreeNode::setRight(BTreeNode* node)
		{
			right = node;
		}

		void BTreeNode::addNode(BTreeNode* node)
		{
			if (isGreater(node))
			{
				if (right)
					right->addNode(node);
				else
					right = node;
			}
			else
			{
				if (left)
					left->addNode(node);
				else
					left = node;
			}
		}

		const BTreeNode* BTreeNode::findParent(const BTreeNode* needle) const
		{

			if (isGreater(needle))
			{
				if (right)
				{
					if (false == right->isGreater(needle))
						return this;
					else
						return right->findParent(needle);
				}
			}
			else
			{
				if (left)
				{
					if (false == left->isGreater(needle))
						return this;
					else
						return left->findParent(needle);
				}

			}

			return NULL;
		}

    }
}