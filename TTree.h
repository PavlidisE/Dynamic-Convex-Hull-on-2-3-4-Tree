#pragma once
#include <iostream>
#include "Node.h"

template<typename T>
class TTree {

	/**
	 * @author Evripidis Pavlidis
	 * @since 2020-5-12
	 */
protected:
	Node<T>* root = nullptr;
	int height = -1;
	Node<T>* minNode = nullptr;
	Node<T>* maxNode = nullptr;
public:
	TTree() {};

	virtual ~TTree() {
		PostorderDelete(root);
	};

	Node<T>* GetRoot();
	int GetHeight();
	Node<T>* GetMinNode();
	Node<T>* GetMaxNode();

	void SetRoot(Node<T>*);
	void SetHeight(int);
	void SetMinNode(Node<T>*);
	void SetMaxNode(Node<T>*);

	void Insert(T* p);
	void InsertAt(Node<T>*, T*);
	Node<T>* NodeSplit(Node<T>*);
	Node<T>* Search(Node<T>* n, T* p);
	void Delete(T p);
	void DeleteAt(Node<T>*, T*);
	void DeleteFromLeaf(Node<T>* n, T* p);

	Node<T>* Transform2NodeTo3Node(Node<T>* n);

	void Clear();
	void GetTreeTraversal(Node<T>* n, std::vector<T>& inorderVectorOut);
	std::vector<T> GetChainedLeaves();
	void PostorderDelete(Node<T>* n);

	bool isValidTree();
	bool PostOrderValidNode(Node<T>*);
};

template<typename T>
inline Node<T>* TTree<T>::GetRoot() {
	return this->root;
}

template<typename T>
inline int TTree<T>::GetHeight() {
	return this->height;
}

template<typename T>
inline Node<T>* TTree<T>::GetMinNode()
{
	return this->minNode;
}

template<typename T>
inline Node<T>* TTree<T>::GetMaxNode()
{
	return this->maxNode;
}

template<typename T>
inline void TTree<T>::SetRoot(Node<T>* newroot)
{
	this->root = newroot;
}

template<typename T>
inline void TTree<T>::SetHeight(int h)
{
	this->height = h;
}

template<typename T>
inline void TTree<T>::SetMinNode(Node<T>* minNod)
{
	this->minNode = minNod;
}

template<typename T>
inline void TTree<T>::SetMaxNode(Node<T>* maxNod)
{
	this->maxNode = maxNod;
}

template<typename T>
inline void TTree<T>::Insert(T* p) {
	if (!root) {
		root = new Node<T>(p);
		++height;
		minNode = root;
		maxNode = root;
	}
	else
		InsertAt(root, p);
	return;
}

template<typename T>
inline void TTree<T>::InsertAt(Node<T>* n, T* p)
{
	int nodeSize = n->getNumVal();
	if (nodeSize == 3) {
		n = NodeSplit(n);
		nodeSize = n->getNumVal();
	}

	if (n->getIsLeaf()) {
		T* leftVal = n->getLVal();
		T* middleVal = n->getMVal();
		T* rightVal = n->getRVal();

		switch (nodeSize) {
		case(1):
			if (*p == *leftVal) break;

			if (*p < *leftVal) {
				n->setRVal(leftVal);
				n->setLVal(p);
				break;
			}

			n->setRVal(p);
			break;
		case(2):
			if (*p == *rightVal || *p == *leftVal) break;

			if (*p < *leftVal) {
				n->setMVal(leftVal);
				n->setLVal(p);
				break;
			}

			if (*p < *rightVal) {
				n->setMVal(p);
				break;
			}

			n->setMVal(rightVal);
			n->setRVal(p);
			break;
		default:
			std::cerr << __FUNCTION__ << " Leaf node with 3 values after split." << std::endl;
		}
	}
	else {
		switch (nodeSize)
		{
		case(1):
			if (*p <= *n->getLMax()->getMaxVal())
				InsertAt(n->getLCh(), p);
			else
				InsertAt(n->getRCh(), p);
			break;
		case(2):
			if (*p <= *n->getLMax()->getMaxVal())
				InsertAt(n->getLCh(), p);
			else if (*p <= *n->getRMax()->getMaxVal())
				InsertAt(n->getM1Ch(), p);
			else
				InsertAt(n->getRCh(), p);
			break;
		case(3):
			if (*p <= *n->getLMax()->getMaxVal())
				InsertAt(n->getLCh(), p);
			else if (*p <= *n->getMMax()->getMaxVal())
				InsertAt(n->getM1Ch(), p);
			else if (*p <= *n->getRMax()->getMaxVal())
				InsertAt(n->getM2Ch(), p);
			else
				InsertAt(n->getRCh(), p);
			break;
		default:
			std::cerr << __FUNCTION__ << " Internal node with 0 or 3 values after split." << std::endl;
			break;
		}
	}
	return;
}

template<typename T>
inline Node<T>* TTree<T>::NodeSplit(Node<T>* n)
{
	if (n == root) {
		if (n->getIsLeaf()) {
			Node<T>* newLeft = new Node<T>(n->getLVal(), n->getMVal());
			Node<T>* newRight = new Node<T>(n->getRVal());
			Node<T>* newRoot = new Node<T>(newLeft, newLeft, newRight);
			Node<T>* mem_clear = root;

			newRight->setChainLeft(newLeft);

			root = newRoot;
			++height;
			minNode = newLeft;
			maxNode = newRight;

			delete mem_clear;
			return root;
		}
		else {
			Node<T>* newLeft = new Node<T>(n->getLMax(), n->getLCh(), n->getM1Ch());
			Node<T>* newRight = new Node<T>(n->getRMax(), n->getM2Ch(), n->getRCh());
			Node<T>* newRoot = new Node<T>(n->getMMax(), newLeft, newRight);
			Node<T>* mem_clear = root;

			root = newRoot;
			++height;

			delete mem_clear;
			return root;
		}
	}

	if (n->getIsLeaf()) {
		T* middleVal = n->getMVal();
		Node<T>* father = n->getFather();
		switch (father->getNumVal()) {
		case(1):
			if (*middleVal < *father->getLMax()->getMaxVal()) {

				Node<T>* newMiddle = new Node<T>(middleVal, n->getRVal());

				father->setM1Ch(newMiddle);
				father->setRMax(newMiddle);

				n->setRVal(nullptr);
				n->setMVal(nullptr);

				newMiddle->setChainLeft(n);
				father->getRCh()->setChainLeft(newMiddle);

			}
			else {
				Node<T>* newMiddle = new Node<T>(n->getLVal(), middleVal);

				father->setM1Ch(newMiddle);
				father->setRMax(newMiddle);

				n->setLVal(n->getRVal());
				n->setMVal(nullptr);
				n->setRVal(nullptr);

				newMiddle->setChainLeft(father->getLCh());
				n->setChainLeft(newMiddle);

			}
			break;
		case(2):
			if (*middleVal < *father->getLMax()->getMaxVal()) {

				Node<T>* newMiddle = new Node<T>(middleVal, n->getRVal());

				father->setM2Ch(father->getM1Ch());
				father->setM1Ch(newMiddle);

				n->setMVal(nullptr);
				n->setRVal(nullptr);

				father->setMMax(newMiddle);

				newMiddle->setChainLeft(n);
				father->getM2Ch()->setChainLeft(newMiddle);
			}
			else if (*middleVal < *father->getRMax()->getMaxVal()) {

				Node<T>* newMiddle = new Node<T>(middleVal, n->getRVal());

				father->setM2Ch(newMiddle);

				n->setMVal(nullptr);
				n->setRVal(nullptr);

				father->setMMax(n);
				father->setRMax(newMiddle);

				newMiddle->setChainLeft(n);
				father->getRCh()->setChainLeft(newMiddle);
			}
			else {
				Node<T>* newMiddle = new Node<T>(n->getLVal(), middleVal);
				father->setM2Ch(newMiddle);

				n->setLVal(n->getRVal());
				n->setMVal(nullptr);
				n->setRVal(nullptr);

				father->setMMax(father->getRMax());
				father->setRMax(newMiddle);

				newMiddle->setChainLeft(father->getM1Ch());
				n->setChainLeft(newMiddle);
			}
			break;
		}
		return father;
	}

	// if n is internal node 

	Node<T>* middleMax = n->getMMax();
	Node<T>* father = n->getFather();

	switch (father->getNumVal()) {
	case(1):
		if (*middleMax->getMaxVal() < *father->getLMax()->getMaxVal()) {
			Node<T>* newMiddle = new Node<T>(n->getRMax(), n->getM2Ch(), n->getRCh());
			newMiddle->setFather(father);

			father->setM1Ch(newMiddle);

			father->setRMax(father->getLMax());
			father->setLMax(middleMax);

			n->setMMax(nullptr);
			n->setRMax(nullptr);

			n->setRCh(n->getM1Ch());
			n->setM1Ch(nullptr);
			n->setM2Ch(nullptr);
		}
		else {
			Node<T>* newMiddle = new Node<T>(n->getLMax(), n->getLCh(), n->getM1Ch());

			father->setM1Ch(newMiddle);

			father->setRMax(middleMax);

			n->setLMax(n->getRMax());
			n->setMMax(nullptr);
			n->setRMax(nullptr);

			n->setLCh(n->getM2Ch());
			n->setM1Ch(nullptr);
			n->setM2Ch(nullptr);
		}
		break;
	case(2):
		if (*middleMax->getMaxVal() < *father->getLMax()->getMaxVal()) {
			Node<T>* newMiddle = new Node<T>(n->getRMax(), n->getM2Ch(), n->getRCh());

			father->setM2Ch(father->getM1Ch());
			father->setM1Ch(newMiddle);

			father->setMMax(father->getLMax());
			father->setLMax(middleMax);

			n->setRCh(n->getM1Ch());
			n->setM1Ch(nullptr);
			n->setM2Ch(nullptr);

			n->setMMax(nullptr);
			n->setRMax(nullptr);

		}
		else if (*middleMax->getMaxVal() < *father->getRMax()->getMaxVal()) {
			Node<T>* newMiddle = new Node<T>(n->getRMax(), n->getM2Ch(), n->getRCh());

			father->setM2Ch(newMiddle);

			father->setMMax(middleMax);

			n->setRCh(n->getM1Ch());
			n->setM1Ch(nullptr);
			n->setM2Ch(nullptr);

			n->setMMax(nullptr);
			n->setRMax(nullptr);
		}
		else {
			Node<T>* newMiddle = new Node<T>(n->getLMax(), n->getLCh(), n->getM1Ch());

			father->setM2Ch(newMiddle);

			father->setMMax(father->getRMax());
			father->setRMax(middleMax);

			n->setLMax(n->getRMax());
			n->setMMax(nullptr);
			n->setRMax(nullptr);

			n->setLCh(n->getM2Ch());
			n->setM1Ch(nullptr);
			n->setM2Ch(nullptr);

		}
		break;
	}
	return father;
}

template<typename T>
inline Node<T>* TTree<T>::Search(Node<T>* n, T* p)
{
	if (!n) return nullptr;

	if (n->getIsLeaf()) {
		T* l = n->getLVal();
		T* m = n->getMVal();
		T* r = n->getRVal();

		if (*p == *l) return n;
		if (r) if (*p == *r) return n;
		if (m) if (*p == *m) return n;
		return nullptr;
	}

	Node<T>* LMax = n->getLMax();
	Node<T>* MMax = n->getMMax();
	Node<T>* RMax = n->getRMax();

	if (RMax) {
		if (*RMax->getMaxVal() == *p) return RMax;
		if (*RMax->getMaxVal() < *p) return Search(n->getRCh(), p);
	}
	if (MMax) {
		if (*MMax->getMaxVal() == *p) return MMax;
		if (*MMax->getMaxVal() < *p) return Search(n->getM2Ch(), p);
	}
	if (LMax) {
		if (*LMax->getMaxVal() == *p) return LMax;
		if (*LMax->getMaxVal() < *p) return Search(n->getM1Ch(), p);
		else return Search(n->getLCh(), p);
	}
}

template<typename T>
inline void TTree<T>::Delete(T p) {
	if (!root) return;
	if (root->getIsLeaf()) {
		DeleteFromLeaf(root, &p);
		return;
	}
	DeleteAt(root, &p);
}

template<typename T>
inline void TTree<T>::DeleteAt(Node<T>* n, T* p)
{
	if (!n) return;
	int nodeSize = n->getNumVal();
	if (nodeSize == 1 && n != root)
		n = Transform2NodeTo3Node(n);

	if (n->getIsLeaf()) {
		DeleteFromLeaf(n, p);
		return;
	}

	Node<T>* LMax = n->getLMax();
	Node<T>* MMax = n->getMMax();
	Node<T>* RMax = n->getRMax();

	if (RMax) {
		if (*RMax->getMaxVal() < *p)
			return DeleteAt(n->getRCh(), p);
		if (MMax) {
			if (*MMax->getMaxVal() < *p)
				return DeleteAt(n->getM2Ch(), p);
			else {
				if (*LMax->getMaxVal() < *p)
					return DeleteAt(n->getM1Ch(), p);
				else
					return DeleteAt(n->getLCh(), p);
			}
		}
		else {
			if (*LMax->getMaxVal() < *p)
				return DeleteAt(n->getM1Ch(), p);
			else
				return DeleteAt(n->getLCh(), p);
		}
	}
	else {
		if (*LMax->getMaxVal() < *p)
			return DeleteAt(n->getRCh(), p);
		else
			return DeleteAt(n->getLCh(), p);
	}
}

template<typename T>
inline void TTree<T>::DeleteFromLeaf(Node<T>* n, T* p)
{
	int nodeSize = n->getNumVal();

	if (n == GetRoot() || nodeSize >= 2) {
		if (*p == *n->getLVal()) {
			if (nodeSize == 1) {
				this->Clear();
				delete n;
				return;
			}
			else if (nodeSize == 2) {
				n->setLVal(n->getRVal());
				n->setRVal(nullptr);
			}
			else {
				n->setLVal(n->getMVal());
				n->setMVal(nullptr);
			}
		}
		else if (*p == *n->getRVal()) {
			if (nodeSize == 2) {
				n->setRVal(nullptr);
			}
			else {
				n->setRVal(n->getMVal());
				n->setMVal(nullptr);
			}
		}
		else {
			if (n->getMVal() && *p == *n->getMVal()) {
				n->setMVal(nullptr);
			}
		}
	}
}

template<typename T>
inline Node<T>* TTree<T>::Transform2NodeTo3Node(Node<T>* n)
{
	Node<T>* leftSibling = n->getLSibling();
	Node<T>* rightSibling = n->getRSibling();
	Node<T>* fatherNode = n->getFather();

	// 1. Shrink Tree 
	if (fatherNode->getNumVal() == 1 && fatherNode->getLCh()->getNumVal() == 1 && fatherNode->getRCh()->getNumVal() == 1) {
		if (n->getIsLeaf()) {
			Node<T>* newRoot = new Node<T>(fatherNode->getLCh()->getLVal(), fatherNode->getRCh()->getLVal());
			Node<T>* mem_clear = root;

			root = newRoot;
			--height;
			minNode = root;
			maxNode = root;

			delete mem_clear->getLCh();
			delete mem_clear->getRCh();
			delete mem_clear;
			return newRoot;
		}

		Node<T>* leftCh = fatherNode->getLCh();
		Node<T>* rightCh = fatherNode->getRCh();

		fatherNode->setMMax(fatherNode->getLMax());
		fatherNode->setLMax(leftCh->getLMax());
		fatherNode->setRMax(rightCh->getLMax());

		fatherNode->setLCh(leftCh->getLCh());
		fatherNode->setM1Ch(leftCh->getRCh());
		fatherNode->setM2Ch(rightCh->getLCh());
		fatherNode->setRCh(rightCh->getRCh());

		delete leftCh;
		delete rightCh;
		--height;
		return fatherNode;
	}

	// 2. Rotation

	if (leftSibling) {
		if (leftSibling->getNumVal() >= 2) {
			if (n->getIsLeaf()) {
				n->setRVal(n->getLVal());
				n->setLVal(leftSibling->getRVal());
				leftSibling->setRVal(leftSibling->getMVal());
				leftSibling->setMVal(nullptr);
				return n;
			}
			else {
				n->setRMax(n->getLMax());
				n->setM1Ch(n->getLCh());
				n->setLCh(leftSibling->getRCh());

				if (n == fatherNode->getM1Ch()) {
					n->setLMax(fatherNode->getLMax());
					fatherNode->setLMax(leftSibling->getRMax());
				}
				else if (n == fatherNode->getM2Ch()) {
					n->setLMax(fatherNode->getMMax());
					fatherNode->setMMax(leftSibling->getRMax());
				}
				else {
					if (fatherNode->getNumVal() > 1) {
						n->setLMax(fatherNode->getRMax());
						fatherNode->setRMax(leftSibling->getRMax());
					}
					else {
						n->setLMax(fatherNode->getLMax());
						fatherNode->setLMax(leftSibling->getRMax());
					}
				}

				if (leftSibling->getNumVal() == 2) {
					leftSibling->setRMax(nullptr);
					leftSibling->setRCh(leftSibling->getM1Ch());
					leftSibling->setM1Ch(nullptr);
				}
				else {
					leftSibling->setRMax(leftSibling->getMMax());
					leftSibling->setMMax(nullptr);
					leftSibling->setRCh(leftSibling->getM2Ch());
					leftSibling->setM2Ch(nullptr);
				}
				return n;
			}
		}
		// 3. Fusion
		else {
			if (!rightSibling || rightSibling->getNumVal() == 1) {
				if (n->getIsLeaf()) {
					if (n == fatherNode->getRCh()) {
						n->setRVal(n->getLVal());
						n->setLVal(leftSibling->getLVal());

						if (fatherNode->getNumVal() == 2) {
							fatherNode->setRMax(nullptr);
							fatherNode->setM1Ch(nullptr);
							n->setChainLeft(fatherNode->getLCh());
						}
						else {
							fatherNode->setRMax(fatherNode->getMMax());
							fatherNode->setMMax(nullptr);
							fatherNode->setM2Ch(nullptr);
							n->setChainLeft(fatherNode->getM1Ch());
						}

						delete leftSibling;
						return n;
					}

					leftSibling->setRVal(n->getLVal());

					if (fatherNode->getNumVal() == 2) {
						fatherNode->setRMax(nullptr);
						fatherNode->setM1Ch(nullptr);

						fatherNode->getRCh()->setChainLeft(leftSibling);

						delete n;
						return leftSibling;
					}

					if (n == fatherNode->getM1Ch()) {
						fatherNode->setM1Ch(fatherNode->getM2Ch());

						fatherNode->getM1Ch()->setChainLeft(leftSibling);
					}
					else {
						fatherNode->setRMax(fatherNode->getMMax());

						fatherNode->getRCh()->setChainLeft(leftSibling);
					}

					fatherNode->setMMax(nullptr);
					fatherNode->setM2Ch(nullptr);
					delete n;
					return leftSibling;
				}

				if (n == fatherNode->getRCh()) {
					n->setRMax(n->getLMax());
					n->setMMax(fatherNode->getRMax());
					n->setLMax(leftSibling->getLMax());

					n->setM2Ch(n->getLCh());
					n->setM1Ch(leftSibling->getRCh());
					n->setLCh(leftSibling->getLCh());

					if (fatherNode->getNumVal() == 2) {
						fatherNode->setRMax(nullptr);
						fatherNode->setM1Ch(nullptr);
					}
					else {
						fatherNode->setRMax(fatherNode->getMMax());
						fatherNode->setMMax(nullptr);
						fatherNode->setM2Ch(nullptr);
					}
					delete leftSibling;
					return n;
				}

				leftSibling->setM1Ch(leftSibling->getRCh());
				leftSibling->setM2Ch(n->getLCh());
				leftSibling->setRCh(n->getRCh());

				leftSibling->setRMax(n->getLMax());

				if (n == fatherNode->getM2Ch()) {
					leftSibling->setMMax(fatherNode->getMMax());
					fatherNode->setMMax(nullptr);
					fatherNode->setM2Ch(nullptr);
				}
				else {
					leftSibling->setMMax(fatherNode->getLMax());
					if (fatherNode->getNumVal() == 2) {
						fatherNode->setLMax(fatherNode->getRMax());
						fatherNode->setRMax(nullptr);
						fatherNode->setM1Ch(nullptr);
					}
					else {
						fatherNode->setLMax(fatherNode->getMMax());
						fatherNode->setMMax(nullptr);
						fatherNode->setM1Ch(fatherNode->getM2Ch());
						fatherNode->setM2Ch(nullptr);
					}
				}
				delete n;
				return leftSibling;
			}
		}
	}

	if (rightSibling) {
		if (rightSibling->getNumVal() >= 2) {
			if (n->getIsLeaf()) {
				n->setRVal(rightSibling->getLVal());
				if (rightSibling->getNumVal() == 2) {
					rightSibling->setLVal(rightSibling->getRVal());
					rightSibling->setRVal(nullptr);
				}
				else {
					rightSibling->setLVal(rightSibling->getMVal());
					rightSibling->setMVal(nullptr);
				}
				return n;
			}
			else {
				n->setM1Ch(n->getRCh());
				n->setRCh(rightSibling->getLCh());

				rightSibling->setLCh(rightSibling->getM1Ch());

				if (n == fatherNode->getLCh()) {
					n->setRMax(fatherNode->getLMax());
					fatherNode->setLMax(rightSibling->getLMax());
				}
				else if (n == fatherNode->getM2Ch()) {
					n->setRMax(fatherNode->getRMax());
					fatherNode->setRMax(rightSibling->getLMax());
				}
				else {
					if (fatherNode->getNumVal() == 2) {
						n->setRMax(fatherNode->getRMax());
						fatherNode->setRMax(rightSibling->getLMax());
					}
					else {
						n->setRMax(fatherNode->getMMax());
						fatherNode->setMMax(rightSibling->getLMax());
					}
				}

				if (rightSibling->getNumVal() == 2) {
					rightSibling->setLMax(rightSibling->getRMax());
					rightSibling->setRMax(nullptr);
					rightSibling->setM1Ch(nullptr);
				}
				else {
					rightSibling->setLMax(rightSibling->getMMax());
					rightSibling->setMMax(nullptr);
					rightSibling->setM1Ch(rightSibling->getM2Ch());
					rightSibling->setM2Ch(nullptr);
				}
				return n;
			}
		}
		// 3. Fusion
		else {
			if (n->getIsLeaf()) {
				if (n == fatherNode->getLCh()) {
					n->setRVal(rightSibling->getLVal());

					if (fatherNode->getNumVal() == 3) {
						fatherNode->setMMax(nullptr);
						fatherNode->setM1Ch(fatherNode->getM2Ch());
						fatherNode->setM2Ch(nullptr);

						fatherNode->getM1Ch()->setChainLeft(n);
					}
					else {
						fatherNode->setRMax(nullptr);
						fatherNode->setM1Ch(nullptr);

						fatherNode->getRCh()->setChainLeft(n);
					}
					delete rightSibling;
					return n;
				}
				if (n == fatherNode->getM1Ch()) {
					if (fatherNode->getNumVal() == 2) {
						fatherNode->setRMax(nullptr);
						fatherNode->setM1Ch(nullptr);

						rightSibling->setRVal(rightSibling->getLVal());
						rightSibling->setLVal(n->getLVal());

						rightSibling->setChainLeft(fatherNode->getLCh());

						delete n;
						return rightSibling;
					}

					n->setRVal(rightSibling->getLVal());

					fatherNode->setRMax(fatherNode->getMMax());
					fatherNode->setMMax(nullptr);
					fatherNode->setM2Ch(nullptr);

					fatherNode->getRCh()->setChainLeft(n);

					delete rightSibling;
					return n;
				}

				fatherNode->setRMax(fatherNode->getMMax());
				fatherNode->setMMax(nullptr);
				fatherNode->setM2Ch(nullptr);

				rightSibling->setRVal(rightSibling->getLVal());
				rightSibling->setLVal(n->getLVal());

				n->setChainLeft(fatherNode->getM1Ch());

				delete n;
				return rightSibling;
			}

			if (n == fatherNode->getLCh()) {
				n->setMMax(fatherNode->getLMax());
				n->setRMax(rightSibling->getLMax());

				n->setM1Ch(n->getRCh());
				n->setM2Ch(rightSibling->getLCh());
				n->setRCh(rightSibling->getRCh());

				if (fatherNode->getNumVal() == 3) {
					fatherNode->setLMax(fatherNode->getMMax());
					fatherNode->setMMax(nullptr);
					fatherNode->setM1Ch(fatherNode->getM2Ch());
					fatherNode->setM2Ch(nullptr);
				}
				else {
					fatherNode->setLMax(fatherNode->getRMax());
					fatherNode->setRMax(nullptr);
					fatherNode->setM1Ch(nullptr);

				}
				delete rightSibling;
				return n;
			}

			if (n == fatherNode->getM1Ch()) {
				if (fatherNode->getNumVal() == 2) {
					rightSibling->setRMax(rightSibling->getLMax());
					rightSibling->setMMax(fatherNode->getRMax());
					rightSibling->setLMax(n->getLMax());

					rightSibling->setM2Ch(rightSibling->getLCh());
					rightSibling->setM1Ch(n->getRCh());
					rightSibling->setLCh(n->getLCh());

					fatherNode->setM1Ch(nullptr);
					fatherNode->setRMax(nullptr);

					delete n;
					return rightSibling;
				}

				n->setMMax(fatherNode->getMMax());
				n->setRMax(rightSibling->getLMax());

				n->setM1Ch(n->getRCh());
				n->setM2Ch(rightSibling->getLCh());
				n->setRCh(rightSibling->getRCh());

				fatherNode->setMMax(nullptr);
				fatherNode->setM2Ch(nullptr);

				delete rightSibling;
				return n;
			}

			rightSibling->setRMax(rightSibling->getLMax());
			rightSibling->setMMax(fatherNode->getRMax());
			rightSibling->setLMax(n->getLMax());

			rightSibling->setM2Ch(rightSibling->getLCh());
			rightSibling->setM1Ch(n->getRCh());
			rightSibling->setLCh(n->getLCh());

			fatherNode->setRMax(fatherNode->getMMax());
			fatherNode->setMMax(nullptr);
			fatherNode->setM2Ch(nullptr);

			delete n;
			return rightSibling;
		}
	}

	std::cerr << "Undefined Behavior in node transformation." << std::endl;
	return NULL;
}

template<typename T>
inline void TTree<T>::Clear()
{
	this->SetRoot(nullptr);
	this->SetHeight(-1);
	this->SetMinNode(nullptr);
	this->SetMaxNode(nullptr);
}

template<typename T>
inline void TTree<T>::GetTreeTraversal(Node<T>* n, std::vector<T>& inorderVectorOut) {
	// from when the 2-4 tree was default.

	if (n == nullptr) return;
	GetTreeTraversal(n->getLCh(), inorderVectorOut);

	int nodeSize = n->getNumVal();

	if (n->getIsLeaf()) {
		std::vector<T> values = n->getAllVals();
		for (int i = 0; i < nodeSize; ++i) {
			inorderVectorOut.push_back(values[i]);
		}
	}
	else {
		std::vector<Node<T>*> children = n->getAllCh();
		for (int i = 0; i < nodeSize; ++i) {
			if ((int)children.size() > (i + 1))
				GetTreeTraversal(children[(i + 1)], inorderVectorOut);
		}
	}
}

template<typename T>
inline std::vector<T> TTree<T>::GetChainedLeaves()
{
	Node<T>* root = this->GetRoot();
	if (!root)
		return std::vector<T>();
	else {
		std::vector<T> Output;
		Node<T>* itr = GetMinNode();
		while (itr) {
			std::vector<T> values = itr->getAllVals();
			for (int i = 0; i < itr->getNumVal(); ++i) {
				Output.push_back(values[i]);
			}
			itr = itr->getChainRight();
		}
		return Output;
	}
}

template<typename T>
inline void TTree<T>::PostorderDelete(Node<T>* n) {
	if (!n) return;

	PostorderDelete(n->getLCh());
	PostorderDelete(n->getM1Ch());
	PostorderDelete(n->getM2Ch());
	PostorderDelete(n->getRCh());
	delete n;
}

template <typename T>
inline bool TTree<T>::isValidTree() {
	Node<T>* root = this->GetRoot();
	if (!root) return true;
	if (root->getFather()) {
		__debugbreak();
	}
	return PostOrderValidNode(root);
}

template<typename T>
inline bool TTree<T>::PostOrderValidNode(Node<T>* n)
{
	if (!n) return true;
	if (!n->isValidNode()) return false;

	return (PostOrderValidNode(n->getLCh()) &&
		PostOrderValidNode(n->getM1Ch()) &&
		PostOrderValidNode(n->getM2Ch()) &&
		PostOrderValidNode(n->getRCh())
		);
}