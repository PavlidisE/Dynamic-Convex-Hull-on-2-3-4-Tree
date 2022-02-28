#pragma once
#include "TTree.h"
#include <queue>
#include <fstream>

template <typename T>
class CQueue : public TTree<T>
{
	/**
	 * @author Evripidis Pavlidis
	 * @since 2020-5-12
	 */

public:
	CQueue() {};
	CQueue(T* p);
	CQueue(Node<T>* root, int height, Node<T>* minNode, Node<T>* maxNode);
	~CQueue() {};

	void ShallowCopy(CQueue<T>* CQ2);

	CQueue<T>* Split(T valueToSplit, bool returnFlag, bool inclusive);
	void SplitAtLeaf(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger);
	void SplitAtInternal(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger);
	void GlueTrees(CQueue<T>* leftTree, CQueue<T>* rightTree, CQueue<T>* outTree);
	static CQueue<T>* Concatenate(CQueue<T>* leftTree, CQueue<T>* rightTree);
};

template<typename T>
inline CQueue<T>::CQueue(T* p) {
	this->Insert(p);
}

template<typename T>
inline CQueue<T>::CQueue(Node<T>* root, int height, Node<T>* minNode, Node<T>* maxNode)
{
	this->root = root;
	this->height = height;
	this->minNode = minNode;
	this->maxNode = maxNode;
}

template<typename T>
inline void CQueue<T>::ShallowCopy(CQueue<T>* CQ2)
{
	if (!CQ2) return;
	this->root = CQ2->GetRoot();
	this->height = CQ2->GetHeight();
	this->minNode = CQ2->GetMinNode();
	this->maxNode = CQ2->GetMaxNode();
}

template <typename T>
inline CQueue<T>* CQueue<T>::Split(T p, bool returnFlag, bool inclusive) {
	// returnFlag true = Left SubTree (Smaller/Equal to p)
	// returnFlag false = Right SubTree (Bigger than p)

	CQueue<T>* q_lesseq = new CQueue();
	CQueue<T>* q_bigger = new CQueue();

	if (this->root == nullptr) {
		delete q_bigger;
		delete q_lesseq;
		return nullptr;
	}
	else if (p < *(this->minNode->getLVal()) || p == *(this->minNode->getLVal()) && !inclusive) {
		if (returnFlag) {
			delete q_bigger;
			return q_lesseq;
		}
		else {
			q_bigger->ShallowCopy(this);
			this->Clear();
			delete q_lesseq;
			return q_bigger;
		}
	}
	else if (p > *(this->maxNode->getMaxVal()) || p == *(this->maxNode->getMaxVal()) && inclusive) {
		if (returnFlag) {
			q_lesseq->ShallowCopy(this);
			this->Clear();
			delete q_bigger;
			return q_lesseq;
		}
		else {
			delete q_lesseq;
			return q_bigger;
		}
	}

	Node<T>* itr = this->root;
	while (!itr->getIsLeaf()) {
		Node<T>* LMax = itr->getLMax();
		Node<T>* MMax = itr->getMMax();
		Node<T>* RMax = itr->getRMax();

		if (RMax) {
			if (*RMax->getMaxVal() < p) {
				itr = itr->getRCh();
				continue;
			}
		}
		if (MMax) {
			if (*MMax->getMaxVal() < p) {
				itr = itr->getM2Ch();
				continue;
			}
		}

		if (*LMax->getMaxVal() < p) {
			if (itr->getNumVal() == 1)
				itr = itr->getRCh();
			else
				itr = itr->getM1Ch();
			continue;
		}
		else itr = itr->getLCh();
	}

	if (itr && itr->getIsLeaf()) {
		T* LVal = itr->getLVal();
		T* MVal = itr->getMVal();
		T* RVal = itr->getRVal();

		if (p < *LVal) {
			Node<T>* left = itr->getChainLeft();
			SplitAtLeaf(left, *left->getMaxVal(), q_lesseq, q_bigger);
		}
		else if (p == *LVal) {
			if (inclusive)
				SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
			else {
				Node<T>* left = itr->getChainLeft();
				SplitAtLeaf(left, *left->getMaxVal(), q_lesseq, q_bigger);
			}
		}
		else if (p == *RVal) {
			if (inclusive)
				SplitAtLeaf(itr, *RVal, q_lesseq, q_bigger);
			else {
				if (itr->getNumVal() == 2) {
					SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
				}
				else {
					SplitAtLeaf(itr, *MVal, q_lesseq, q_bigger);
				}
			}
		}
		else {
			if (itr->getNumVal() == 2) {
				if (p < *RVal)
					SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
			}
			else {
				if (p == *MVal) {
					if (inclusive)
						SplitAtLeaf(itr, *MVal, q_lesseq, q_bigger);
					else {
						SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
					}
				}
				else if (p < *MVal) {
					SplitAtLeaf(itr, *LVal, q_lesseq, q_bigger);
				}
				else {
					SplitAtLeaf(itr, *MVal, q_lesseq, q_bigger);
				}
			}
		}
	}

	if (returnFlag) {
		this->ShallowCopy(q_bigger);

		q_bigger->SetRoot(nullptr);
		delete q_bigger;

		return q_lesseq;
	}
	else {
		this->ShallowCopy(q_lesseq);

		q_lesseq->SetRoot(nullptr);
		delete q_lesseq;

		return q_bigger;
	}
}

template<typename T>
inline void CQueue<T>::SplitAtLeaf(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger)
{
	Node<T>* father = splittingNode->getFather();
	int ValNum = splittingNode->getNumVal();

	if (valueToSplit == *splittingNode->getLVal()) {
		if (ValNum == 1) {
			q_lesseq->SetRoot(splittingNode);
			q_lesseq->SetMinNode(splittingNode);
			q_lesseq->SetMaxNode(splittingNode);
			q_lesseq->SetHeight(0);

			Node<T>* chainR = splittingNode->getChainRight();
			if (chainR) {
				chainR->setChainLeft(nullptr);
				splittingNode->setChainRight(nullptr);
			}

			if (father) {
				father->removeChild(splittingNode);
				splittingNode->setFather(nullptr);
			}

		}
		else {
			Node<T>* newLeft = new Node<T>(splittingNode->getLVal());
			if (splittingNode->getMVal()){
				splittingNode->setLVal(splittingNode->getMVal());
				splittingNode->setMVal(nullptr);

			}
			else {
				splittingNode->setLVal(splittingNode->getRVal());
				splittingNode->setRVal(nullptr);

			}

			q_lesseq->SetRoot(newLeft);
			q_lesseq->SetMinNode(newLeft);
			q_lesseq->SetMaxNode(newLeft);
			q_lesseq->SetHeight(0);

			q_bigger->SetRoot(splittingNode);
			q_bigger->SetMinNode(splittingNode);
			q_bigger->SetMaxNode(splittingNode);
			q_bigger->SetHeight(0);

			Node<T>* chainL = splittingNode->getChainLeft();
			if (chainL) {
				newLeft->setChainLeft(chainL);
				splittingNode->setChainLeft(nullptr);
			}

			if (father) {
				father->removeChild(splittingNode);
				splittingNode->setFather(nullptr);
			}

		}

	}
	else if (valueToSplit == *splittingNode->getRVal()) {
		q_lesseq->SetRoot(splittingNode);
		q_lesseq->SetMinNode(splittingNode);
		q_lesseq->SetMaxNode(splittingNode);
		q_lesseq->SetHeight(0);

		Node<T>* chainR = splittingNode->getChainRight();
		if (chainR) {
			chainR->setChainLeft(nullptr);
			splittingNode->setChainRight(nullptr);
		}

		if (father) {
			father->removeChild(splittingNode);
			splittingNode->setFather(nullptr);
		}

	}
	else {
		Node<T>* newLeft = new Node<T>(splittingNode->getLVal(), splittingNode->getMVal());

		q_lesseq->SetRoot(newLeft);
		q_lesseq->SetMinNode(newLeft);
		q_lesseq->SetMaxNode(newLeft);
		q_lesseq->SetHeight(0);

		splittingNode->setLVal(splittingNode->getRVal());
		splittingNode->setMVal(nullptr);
		splittingNode->setRVal(nullptr);

		q_bigger->SetRoot(splittingNode);
		q_bigger->SetMinNode(splittingNode);
		q_bigger->SetMaxNode(splittingNode);
		q_bigger->SetHeight(0);

		Node<T>* chainL = splittingNode->getChainLeft();
		if (chainL) {
			newLeft->setChainLeft(chainL);
			splittingNode->setChainLeft(nullptr);
		}

		if (father) {
			father->removeChild(splittingNode);
			splittingNode->setFather(nullptr);
		}

	}
	return SplitAtInternal(father, valueToSplit, q_lesseq, q_bigger);
}

template<typename T>
inline void CQueue<T>::SplitAtInternal(Node<T>* splittingNode, T valueToSplit, CQueue<T>* q_lesseq, CQueue<T>* q_bigger)
{
	if (!splittingNode) return;

	Node<T>* father = splittingNode->getFather();
	Node<T>* leftCh = splittingNode->getLCh();
	Node<T>* m1Ch = splittingNode->getM1Ch();
	Node<T>* m2Ch = splittingNode->getM2Ch();
	Node<T>* rightCh = splittingNode->getRCh();

	if (splittingNode->getNumVal() == 1) {
		if (valueToSplit < *splittingNode->getLMax()->getMaxVal()) {
			// p < lmax

			int level_height = rightCh->getHeight();

			if (level_height == q_bigger->GetHeight()) {
				// Clear any remnants of split (left) subtree where we came from.
				Node<T>::postOrderSubTreeDelete(leftCh);

				// Add the new values and children to RightQueue.
				splittingNode->setLCh(q_bigger->GetRoot());
				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(q_bigger->GetHeight() + 1);

				Node<T>* itr = rightCh;
				Node<T>* chain_itr = itr;
				while (itr->getRCh()) {
					itr = itr->getRCh();
					chain_itr = chain_itr->getLCh();
				}
				chain_itr->setChainLeft(q_bigger->GetMaxNode());
				q_bigger->SetMaxNode(itr);

				// Sever SplittingNode from OriginalTree so its not destroyed when garbage cleaning happens.
				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {
				// Sever RCh subtree ( set RCh = nullptr )
				splittingNode->setRCh(nullptr);
				rightCh->setFather(nullptr);

				// Glue to subtree root = RCh 

				Node<T>* r_min = rightCh;
				Node<T>* r_max = rightCh;
				while (r_min->getRCh()) {
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}

				CQueue<T>* newQBigger = new CQueue<T>(rightCh, level_height, r_min, r_max);
				newQBigger->GlueTrees(q_bigger, newQBigger, nullptr);

				q_bigger->ShallowCopy(newQBigger);
				newQBigger->SetRoot(nullptr);
				delete newQBigger;

				// Delete LCh subtree.
				Node<T>::postOrderSubTreeDelete(leftCh);
				splittingNode->setLCh(nullptr);
				
				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}
				delete splittingNode;

			}

		}
		else if (valueToSplit == *splittingNode->getLMax()->getMaxVal()) {
			// p == lmax

			Node<T>* r_min = rightCh;
			Node<T>* r_max = rightCh;
			while (r_min->getLCh()) {
				r_min = r_min->getLCh();
				r_max = r_max->getRCh();
			}

			q_bigger->SetRoot(rightCh);
			q_bigger->SetMinNode(r_min);
			q_bigger->SetMaxNode(r_max);
			q_bigger->SetHeight(rightCh->getHeight());

			rightCh->setFather(nullptr);
			splittingNode->setRCh(nullptr);

			if (father) {
				father->removeChild(splittingNode);
			}
			else {
				this->SetRoot(nullptr);
			}
			delete splittingNode;

		}
		else {
			// p > lmax

			int level_height = leftCh->getHeight();

			if (level_height == q_lesseq->GetHeight()) {

				// Clear any remnants of split (right) subtree where we came from.
				splittingNode->postOrderSubTreeDelete(rightCh);

				// Add the new values and children to RightQueue.
				splittingNode->setRCh(q_lesseq->GetRoot());
				q_lesseq->SetRoot(splittingNode);
				q_lesseq->SetHeight((q_lesseq->GetHeight()) + 1);

				Node<T>* itr = leftCh;
				Node<T>* chain_itr = itr;
				while (itr->getLCh()) {
					itr = itr->getLCh();
					chain_itr = chain_itr->getRCh();
				}
				q_lesseq->GetMinNode()->setChainLeft(chain_itr);
				q_lesseq->SetMinNode(itr);

				// Sever SplittingNode from OriginalTree so its not destroyed when garbage cleaning happens.
				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {
				// Sever LCh subtree ( set LCh = nullptr )
				splittingNode->setLCh(nullptr);
				leftCh->setFather(nullptr);

				// Glue to subtree root = LCh 

				Node<T>* l_min = leftCh;
				Node<T>* l_max = leftCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					l_max = l_max->getRCh();
				}

				CQueue<T>* newQLessEq = new CQueue<T>(leftCh, level_height, l_min, l_max);
				newQLessEq->GlueTrees(newQLessEq, q_lesseq, nullptr);

				q_lesseq->ShallowCopy(newQLessEq);
				newQLessEq->SetRoot(nullptr);
				delete newQLessEq;

				// Delete RCh subtree 
				rightCh->postOrderSubTreeDelete(rightCh);
				splittingNode->setRCh(nullptr);

				if (!father) {
					delete splittingNode;
					this->SetRoot(nullptr);
				}

			}
		}
	}
	else if (splittingNode->getNumVal() == 2) {
		if (valueToSplit < *splittingNode->getLMax()->getMaxVal()) {
			// p < lmax
			int level_height = rightCh->getHeight();
			if (level_height == q_bigger->GetHeight()) {
				Node<T>::postOrderSubTreeDelete(leftCh);

				splittingNode->setLCh(q_bigger->GetRoot());
				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(q_bigger->GetHeight() + 1);

				Node<T>* itr = rightCh;
				while (itr->getRCh()) {
					itr = itr->getRCh();
				}
				q_bigger->SetMaxNode(itr);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {
				splittingNode->setLMax(splittingNode->getRMax());
				splittingNode->setRMax(nullptr);
				splittingNode->setLCh(splittingNode->getM1Ch());
				splittingNode->setM1Ch(nullptr);
				Node<T>::postOrderSubTreeDelete(leftCh);

				Node<T>* r_min = splittingNode->getLCh();
				Node<T>* r_max = rightCh;
				while (r_max->getRCh()) {
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}
				CQueue<T>* newQBigger = new CQueue<T>(splittingNode, ++level_height, r_min, r_max);
				newQBigger->GlueTrees(q_bigger, newQBigger, nullptr);

				q_bigger->ShallowCopy(newQBigger);
				newQBigger->SetRoot(nullptr);
				delete newQBigger;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
		}
		else if (valueToSplit == *splittingNode->getLMax()->getMaxVal()) {
			// p = lmax
			int level_height = rightCh->getHeight();

			splittingNode->setLMax(splittingNode->getRMax());
			splittingNode->setRMax(nullptr);
			splittingNode->setLCh(splittingNode->getM1Ch());
			splittingNode->setM1Ch(nullptr);

			Node<T>* r_min = m1Ch;
			Node<T>* r_max = rightCh;
			while (r_min->getLCh()) {
				r_min = r_min->getLCh();
				r_max = r_max->getRCh();
			}

			q_bigger->SetRoot(splittingNode);
			q_bigger->SetMinNode(r_min);
			q_bigger->SetMaxNode(r_max);
			q_bigger->SetHeight(level_height + 1);

			if (father) {
				father->removeChild(splittingNode);
				splittingNode->setFather(nullptr);
			}
			else {
				this->SetRoot(nullptr);
			}

		}
		else if (valueToSplit < *splittingNode->getRMax()->getMaxVal()) {
			// lmax < p < rmax
			int level_height = rightCh->getHeight();
			if (q_lesseq->GetHeight() == level_height && q_bigger->GetHeight() == level_height) {
				Node<T>* l_min, * r_max;
				l_min = leftCh;
				r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					r_max = r_max->getRCh();
				}

				Node<T>* newL = new Node<T>(splittingNode->getLMax(), leftCh, q_lesseq->GetRoot());
				q_lesseq->SetRoot(newL);
				q_lesseq->SetHeight(q_lesseq->GetHeight() + 1);
				q_lesseq->SetMinNode(l_min);

				splittingNode->setLCh(q_bigger->GetRoot());
				splittingNode->setM1Ch(nullptr);
				m1Ch->postOrderSubTreeDelete(m1Ch);
				splittingNode->setLMax(splittingNode->getRMax());
				splittingNode->setRMax(nullptr);

				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(q_bigger->GetHeight() + 1);
				q_bigger->SetMaxNode(r_max);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else if (q_lesseq->GetHeight() == level_height && q_bigger->GetHeight() != level_height) {

				Node<T>* l_min = leftCh, * r_min = rightCh, * r_max = rightCh;
				while (r_min->getLCh()) {
					l_min = l_min->getLCh();
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}

				splittingNode->setRCh(q_lesseq->GetRoot());
				rightCh->setFather(nullptr);

				splittingNode->setRMax(nullptr);
				splittingNode->setM1Ch(nullptr);
				m1Ch->postOrderSubTreeDelete(m1Ch);

				q_lesseq->SetRoot(splittingNode);
				q_lesseq->SetHeight(q_lesseq->GetHeight() + 1);
				q_lesseq->SetMinNode(l_min);

				CQueue<T>* newQBigger = new CQueue<T>(rightCh, level_height, r_min, r_max);
				newQBigger->GlueTrees(q_bigger, newQBigger, nullptr);
				q_bigger->ShallowCopy(newQBigger);
				newQBigger->SetRoot(nullptr);
				delete newQBigger;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else if (q_lesseq->GetHeight() != level_height && q_bigger->GetHeight() == level_height) {

				Node<T>* l_min = leftCh, * r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					r_max = r_max->getRCh();
				}

				splittingNode->setLCh(q_bigger->GetRoot());
				leftCh->setFather(nullptr);

				splittingNode->setM1Ch(nullptr);
				Node<T>::postOrderSubTreeDelete(m1Ch);

				CQueue<T>* newL = new CQueue<T>(leftCh, level_height, l_min, splittingNode->getLMax());
				newL->GlueTrees(newL, q_lesseq, nullptr);
				q_lesseq->ShallowCopy(newL);
				newL->SetRoot(nullptr);
				delete newL;

				splittingNode->setLMax(splittingNode->getRMax());
				splittingNode->setRMax(nullptr);

				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(q_bigger->GetHeight() + 1);
				q_bigger->SetMaxNode(r_max);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else if (q_lesseq->GetHeight() != level_height && q_bigger->GetHeight() != level_height) {

				splittingNode->setLCh(nullptr);
				splittingNode->setRCh(nullptr);
				splittingNode->setM1Ch(nullptr);
				Node<T>::postOrderSubTreeDelete(m1Ch);

				leftCh->setFather(nullptr);
				rightCh->setFather(nullptr);

				Node<T>* l_min = leftCh, * r_min = rightCh, *r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}

				CQueue<T>* newL = new CQueue<T>(leftCh, level_height, l_min, splittingNode->getLMax());
				CQueue<T>* newR = new CQueue<T>(rightCh, level_height, r_min, r_max);
				newL->GlueTrees(newL, q_lesseq, nullptr);
				newR->GlueTrees(q_bigger, newR, nullptr);

				q_lesseq->ShallowCopy(newL);
				newL->SetRoot(nullptr);
				delete newL;

				q_bigger->ShallowCopy(newR);
				newR->SetRoot(nullptr);
				delete newR;

				if (!father) {
					delete splittingNode;
					this->SetRoot(nullptr);
				}

			}
		}
		else if (valueToSplit == *splittingNode->getRMax()->getMaxVal()) {
			// p == rmax
			int level_height = leftCh->getHeight();

			Node<T>* l_min = leftCh, * r_min = rightCh, * r_max = rightCh;
			while (l_min->getLCh()) {
				l_min = l_min->getLCh();
				r_min = r_min->getLCh();
				r_max = r_max->getRCh();
			}

			rightCh->setFather(nullptr);
			q_bigger->SetRoot(rightCh);
			q_bigger->SetHeight(level_height);
			q_bigger->SetMinNode(r_min);
			q_bigger->SetMaxNode(r_max);

			Node<T>::postOrderSubTreeDelete(m1Ch);
			splittingNode->setRCh(q_lesseq->GetRoot());

			q_lesseq->SetRoot(splittingNode);
			q_lesseq->SetHeight(level_height + 1);
			q_lesseq->SetMinNode(l_min);
			q_lesseq->SetMaxNode(splittingNode->getRMax());

			splittingNode->setRMax(nullptr);
			splittingNode->setM1Ch(nullptr);

			if (father) {
				father->removeChild(splittingNode);
				splittingNode->setFather(nullptr);
			}
			else {
				this->SetRoot(nullptr);
			}

		}
		else {
			// p > rmax
			int level_height = leftCh->getHeight();
			if (level_height == q_lesseq->GetHeight()) {
				Node<T>* l_min = leftCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
				}

				Node<T>::postOrderSubTreeDelete(rightCh);
				splittingNode->setRCh(q_lesseq->GetRoot());

				q_lesseq->SetRoot(splittingNode);
				q_lesseq->SetHeight(q_lesseq->GetHeight() + 1);
				q_lesseq->SetMinNode(l_min);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {
				Node<T>::postOrderSubTreeDelete(rightCh);

				Node<T>* l_min = leftCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
				}

				CQueue<T>* newL = new CQueue<T>(splittingNode, ++level_height, l_min, splittingNode->getRMax());
				splittingNode->setRMax(nullptr);
				splittingNode->setM1Ch(nullptr);
				splittingNode->setRCh(m1Ch);

				newL->GlueTrees(newL, q_lesseq, nullptr);
				q_lesseq->ShallowCopy(newL);
				newL->SetRoot(nullptr);
				delete newL;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
		}
	}
	else {
		if (valueToSplit < *splittingNode->getLMax()->getMaxVal()) {
			int level_height = rightCh->getHeight();
			if (level_height == q_bigger->GetHeight()) {
				Node<T>::postOrderSubTreeDelete(leftCh);

				splittingNode->setLCh(q_bigger->GetRoot());

				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(q_bigger->GetHeight() + 1);

				Node<T>* itr = rightCh;
				while (itr->getRCh()) {
					itr = itr->getRCh();
				}
				q_bigger->SetMaxNode(itr);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {
				splittingNode->setLMax(splittingNode->getMMax());
				splittingNode->setMMax(nullptr);

				splittingNode->setLCh(m1Ch);
				splittingNode->setM1Ch(m2Ch);
				splittingNode->setM2Ch(nullptr);

				Node<T>::postOrderSubTreeDelete(leftCh);

				Node<T>* itr_min = m1Ch;
				Node<T>* itr_max = rightCh;
				while (itr_max->getRCh()) {
					itr_min = itr_min->getLCh();
					itr_max = itr_max->getRCh();
				}

				CQueue<T>* newQBigger = new CQueue<T>(splittingNode, level_height + 1, itr_min, itr_max);
				
				newQBigger->GlueTrees(q_bigger, newQBigger, nullptr);
				q_bigger->ShallowCopy(newQBigger);

				newQBigger->SetRoot(nullptr);
				delete newQBigger;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
		}
		else if (valueToSplit == *splittingNode->getLMax()->getMaxVal()) {
			int level_height = rightCh->getHeight();

			splittingNode->setLMax(splittingNode->getMMax());
			splittingNode->setMMax(nullptr);

			splittingNode->setLCh(m1Ch);
			splittingNode->setM1Ch(m2Ch);
			splittingNode->setM2Ch(nullptr);

			Node<T>* right_min = m1Ch;
			Node<T>* right_max = rightCh;
			while (right_min->getLCh()) {
				right_min = right_min->getLCh();
				right_max = right_max->getRCh();
			}

			q_bigger->SetRoot(splittingNode);
			q_bigger->SetMinNode(right_min);
			q_bigger->SetMaxNode(right_max);
			q_bigger->SetHeight(level_height + 1);

			if (father) {
				father->removeChild(splittingNode);
				splittingNode->setFather(nullptr);
			}
			else {
				this->SetRoot(nullptr);
			}

		}
		else if (valueToSplit < *splittingNode->getMMax()->getMaxVal()) {
			int level_height = leftCh->getHeight();
			if (q_lesseq->GetHeight() == level_height && q_bigger->GetHeight() == level_height) {
				Node<T>* newL = new Node<T>(splittingNode->getLMax(), leftCh, q_lesseq->GetRoot());
				Node<T>* l_min, * r_max;
				l_min = leftCh;
				r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					r_max = r_max->getRCh();
				}

				q_lesseq->SetRoot(newL);
				q_lesseq->SetHeight(level_height + 1);
				q_lesseq->SetMinNode(l_min);

				splittingNode->setLMax(splittingNode->getMMax());
				splittingNode->setMMax(nullptr);

				splittingNode->setLCh(q_bigger->GetRoot());
				splittingNode->setM1Ch(m2Ch);
				splittingNode->setM2Ch(nullptr);

				Node<T>::postOrderSubTreeDelete(m1Ch);

				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(level_height + 1);
				q_bigger->SetMaxNode(r_max);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else if (q_lesseq->GetHeight() == level_height && q_bigger->GetHeight() != level_height) {
				Node<T>* newR = new Node<T>(splittingNode->getRMax(), m2Ch, rightCh);

				Node<T>* l_min = leftCh, * r_min = m2Ch, * r_max = rightCh;
				while (r_min->getLCh()) {
					l_min = l_min->getLCh();
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}

				CQueue<T>* newQBigger = new CQueue<T>(newR, level_height + 1, r_min, r_max);
				newQBigger->GlueTrees(q_bigger, newQBigger, nullptr);
				q_bigger->ShallowCopy(newQBigger);

				newQBigger->SetRoot(nullptr);
				delete newQBigger;

				splittingNode->setMMax(nullptr);
				splittingNode->setRMax(nullptr);

				splittingNode->setM1Ch(nullptr);
				splittingNode->setM2Ch(nullptr);
				splittingNode->setRCh(q_lesseq->GetRoot());

				Node<T>::postOrderSubTreeDelete(m1Ch);

				q_lesseq->SetRoot(splittingNode);
				q_lesseq->SetHeight(level_height + 1);
				q_lesseq->SetMinNode(l_min);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else if (q_lesseq->GetHeight() != level_height && q_bigger->GetHeight() == level_height) {
				Node<T>* l_min = leftCh, * l_max = leftCh, * r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					l_max = l_max->getRCh();
					r_max = r_max->getRCh();
				}

				leftCh->setFather(nullptr);
				
				CQueue<T>* newQLess = new CQueue<T>(leftCh, level_height, l_min, l_max);
				newQLess->GlueTrees(newQLess, q_lesseq, nullptr);
				q_lesseq->ShallowCopy(newQLess);

				newQLess->SetRoot(nullptr);
				delete newQLess;

				splittingNode->setLMax(splittingNode->getMMax());
				splittingNode->setMMax(nullptr);

				splittingNode->setLCh(q_bigger->GetRoot());
				splittingNode->setM1Ch(m2Ch);
				splittingNode->setM2Ch(nullptr);

				Node<T>::postOrderSubTreeDelete(m1Ch);

				q_bigger->SetRoot(splittingNode);
				q_bigger->SetHeight(level_height + 1);
				q_bigger->SetMaxNode(r_max);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {
				Node<T>* l_min = leftCh, * l_max = leftCh, * r_min = m2Ch, * r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					l_max = l_max->getRCh();
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}

				leftCh->setFather(nullptr);
				
				splittingNode->setLMax(splittingNode->getRMax());
				splittingNode->setMMax(nullptr);
				splittingNode->setRMax(nullptr);

				splittingNode->setLCh(m2Ch);
				splittingNode->setM1Ch(nullptr);
				splittingNode->setM2Ch(nullptr);

				CQueue<T>* newQLess = new CQueue<T>(leftCh, level_height, l_min, l_max);
				CQueue<T>* newQBigger = new CQueue<T>(splittingNode, level_height + 1, r_min, r_max);

				newQLess->GlueTrees(newQLess, q_lesseq, nullptr);
				q_lesseq->ShallowCopy(newQLess);

				newQLess->SetRoot(nullptr);
				delete newQLess;

				newQBigger->GlueTrees(q_bigger, newQBigger, nullptr);
				q_bigger->ShallowCopy(newQBigger);

				newQBigger->SetRoot(nullptr);
				delete newQBigger;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
		}
		else if (valueToSplit == *splittingNode->getMMax()->getMaxVal()) {
			int level_height = leftCh->getHeight();
			Node<T>* l_min = leftCh, * r_min = m2Ch, * r_max = rightCh;
			while (l_min->getLCh()) {
				l_min = l_min->getLCh();
				r_min = r_min->getLCh();
				r_max = r_max->getRCh();
			}

			Node<T>* newLRoot = new Node<T>(splittingNode->getLMax(), leftCh, q_lesseq->GetRoot());

			q_lesseq->SetRoot(newLRoot);
			q_lesseq->SetHeight(level_height + 1);
			q_lesseq->SetMinNode(l_min);
			q_lesseq->SetMaxNode(splittingNode->getMMax());

			splittingNode->setLCh(m2Ch);
			splittingNode->setLMax(splittingNode->getRMax());
			splittingNode->setMMax(nullptr);
			splittingNode->setRMax(nullptr);

			splittingNode->setM1Ch(nullptr);
			splittingNode->setM2Ch(nullptr);

			q_bigger->SetRoot(splittingNode);
			q_bigger->SetHeight(level_height + 1);
			q_bigger->SetMinNode(r_min);
			q_bigger->SetMaxNode(r_max);

			if (father) {
				father->removeChild(splittingNode);
				splittingNode->setFather(nullptr);
			}
			else {
				this->SetRoot(nullptr);
			}

		}
		else if (valueToSplit < *splittingNode->getRMax()->getMaxVal()) {
			int level_height = leftCh->getHeight();
			if (q_lesseq->GetHeight() == level_height && q_bigger->GetHeight() == level_height) {

				Node<T>* newR = new Node<T>(splittingNode->getRMax(), q_bigger->GetRoot(), rightCh);
				Node<T>* l_min = leftCh, * r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					r_max = r_max->getRCh();
				}

				splittingNode->setRMax(splittingNode->getMMax());
				splittingNode->setMMax(nullptr);

				Node<T>::postOrderSubTreeDelete(m2Ch);
				splittingNode->setM2Ch(nullptr);
				splittingNode->setRCh(q_lesseq->GetRoot());

				q_lesseq->SetRoot(splittingNode);
				q_lesseq->SetHeight(level_height + 1);
				q_lesseq->SetMinNode(l_min);

				q_bigger->SetRoot(newR);
				q_bigger->SetHeight(level_height + 1);
				q_bigger->SetMaxNode(r_max);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else if (q_lesseq->GetHeight() == level_height && q_bigger->GetHeight() != level_height) {

				Node<T>* l_min = leftCh, * r_min = rightCh, * r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}

				splittingNode->setRMax(splittingNode->getMMax());
				splittingNode->setMMax(nullptr);

				Node<T>::postOrderSubTreeDelete(m2Ch);
				splittingNode->setM2Ch(nullptr);
				splittingNode->setRCh(q_lesseq->GetRoot());

				q_lesseq->SetRoot(splittingNode);
				q_lesseq->SetHeight(level_height + 1);
				q_lesseq->SetMinNode(l_min);

				rightCh->setFather(nullptr);
				CQueue<T>* newR = new CQueue<T>(rightCh, level_height, r_min, r_max);
				newR->GlueTrees(q_bigger, newR, nullptr);
				q_bigger->ShallowCopy(newR);

				newR->SetRoot(nullptr);
				delete newR;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else if (q_lesseq->GetHeight() != level_height && q_bigger->GetHeight() == level_height) {

				Node<T>* newR = new Node<T>(splittingNode->getRMax(), q_bigger->GetRoot(), rightCh);
				Node<T>* l_min = leftCh, * l_max = m1Ch, * r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					l_max = l_max->getRCh();
					r_max = r_max->getRCh();
				}

				splittingNode->setRMax(nullptr);
				splittingNode->setMMax(nullptr);

				Node<T>::postOrderSubTreeDelete(m2Ch);
				splittingNode->setM2Ch(nullptr);
				splittingNode->setRCh(m1Ch);
				splittingNode->setM1Ch(nullptr);

				CQueue<T>* newL = new CQueue<T>(splittingNode, level_height + 1, l_min, l_max);
				newL->GlueTrees(newL, q_lesseq, nullptr);
				q_lesseq->ShallowCopy(newL);

				newL->SetRoot(nullptr);
				delete newL;

				q_bigger->SetRoot(newR);
				q_bigger->SetHeight(level_height + 1);
				q_bigger->SetMaxNode(r_max);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {

				Node<T>* l_min = leftCh, * l_max = m1Ch, * r_min = rightCh, * r_max = rightCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
					l_max = l_max->getRCh();
					r_min = r_min->getLCh();
					r_max = r_max->getRCh();
				}

				splittingNode->setRMax(nullptr);
				splittingNode->setMMax(nullptr);

				Node<T>::postOrderSubTreeDelete(m2Ch);
				splittingNode->setM2Ch(nullptr);
				splittingNode->setRCh(m1Ch);
				rightCh->setFather(nullptr);
				splittingNode->setM1Ch(nullptr);

				CQueue<T>* newL = new CQueue<T>(splittingNode, level_height + 1, l_min, l_max);
				CQueue<T>* newR = new CQueue<T>(rightCh, level_height, r_min, r_max);

				newL->GlueTrees(newL, q_lesseq, nullptr);
				q_lesseq->ShallowCopy(newL);

				newL->SetRoot(nullptr);
				delete newL;

				newR->GlueTrees(q_bigger, newR, nullptr);
				q_bigger->ShallowCopy(newR);

				newR->SetRoot(nullptr);
				delete newR;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
		}
		else if (valueToSplit == *splittingNode->getRMax()->getMaxVal()) {
			int level_height = rightCh->getHeight();

			Node<T>* l_min = leftCh, * r_min = rightCh, * r_max = rightCh;
			while (r_min->getLCh()) {
				l_min = l_min->getLCh();
				r_min = r_min->getLCh();
				r_max = r_max->getRCh();
			}

			splittingNode->setRCh(q_lesseq->GetRoot());
			rightCh->setFather(nullptr);

			q_bigger->SetRoot(rightCh);
			q_bigger->SetHeight(level_height);
			q_bigger->SetMinNode(r_min);
			q_bigger->SetMaxNode(r_max);

			q_lesseq->SetRoot(splittingNode);
			q_lesseq->SetHeight(level_height + 1);
			q_lesseq->SetMinNode(l_min);
			q_lesseq->SetMaxNode(splittingNode->getRMax());

			splittingNode->setRMax(splittingNode->getMMax());
			splittingNode->setMMax(nullptr);
			splittingNode->setM2Ch(nullptr);

			if (father) {
				father->removeChild(splittingNode);
				splittingNode->setFather(nullptr);
			}
			else {
				this->SetRoot(nullptr);
			}

		}
		else {
			int level_height = leftCh->getHeight();
			if (q_lesseq->GetHeight() == level_height) {
				Node<T>::postOrderSubTreeDelete(rightCh);
				splittingNode->setRCh(q_lesseq->GetRoot());

				Node<T>* l_min = leftCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
				}

				q_lesseq->SetRoot(splittingNode);
				q_lesseq->SetHeight(level_height + 1);
				q_lesseq->SetMinNode(l_min);

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
			else {

				Node<T>* l_min = leftCh;
				while (l_min->getLCh()) {
					l_min = l_min->getLCh();
				}

				CQueue<T>* newL = new CQueue<T>(splittingNode, level_height + 1, l_min, splittingNode->getRMax());
				splittingNode->setRMax(splittingNode->getMMax());
				splittingNode->setMMax(nullptr);

				splittingNode->setM2Ch(nullptr);
				Node<T>::postOrderSubTreeDelete(rightCh);
				splittingNode->setRCh(m2Ch);

				newL->GlueTrees(newL, q_lesseq, nullptr);
				q_lesseq->ShallowCopy(newL);

				newL->SetRoot(nullptr);
				delete newL;

				if (father) {
					father->removeChild(splittingNode);
					splittingNode->setFather(nullptr);
				}
				else {
					this->SetRoot(nullptr);
				}

			}
		}
	}

	return SplitAtInternal(father, valueToSplit, q_lesseq, q_bigger);
}

template<typename T>
inline void CQueue<T>::GlueTrees(CQueue<T>* leftTree, CQueue<T>* rightTree, CQueue<T>* outTree) {

	Node<T>* l_root = leftTree->GetRoot();
	Node<T>* r_root = rightTree->GetRoot();

	int left_height = leftTree->GetHeight();
	int right_height = rightTree->GetHeight();

	if (left_height > right_height) {

		if (l_root->getNumVal() == 3) {
			l_root = leftTree->NodeSplit(l_root);
			left_height = leftTree->GetHeight();
		}
		int lev_desc = left_height - (right_height + 1);
		Node<T>* itr = l_root;

		while (lev_desc) {
			itr = itr->getRCh();
			if (itr->getNumVal() == 3) leftTree->NodeSplit(itr);
			--lev_desc;
		}

		if (itr->getNumVal() == 1) {
			itr->setM1Ch(itr->getRCh());
		}
		else {
			itr->setMMax(itr->getRMax());
			itr->setM2Ch(itr->getRCh());
		}

		itr->setRMax(leftTree->GetMaxNode());
		itr->setRCh(r_root);
		leftTree->SetMaxNode(rightTree->GetMaxNode());
	}
	else if (left_height < right_height) {

		if (r_root->getNumVal() == 3) {
			r_root = rightTree->NodeSplit(r_root);
			right_height = rightTree->GetHeight();
		}
		int lev_desc = right_height - (left_height + 1);
		Node<T>* itr = r_root;

		while (lev_desc) {
			itr = itr->getLCh();
			if (itr->getNumVal() == 3) rightTree->NodeSplit(itr);
			--lev_desc;
		}

		if (itr->getNumVal() == 1) {
			itr->setRMax(itr->getLMax());
		}
		else {
			itr->setMMax(itr->getLMax());
			itr->setM2Ch(itr->getM1Ch());
		}

		itr->setM1Ch(itr->getLCh());
		itr->setLMax(leftTree->GetMaxNode());
		itr->setLCh(l_root);

		rightTree->SetMinNode(leftTree->GetMinNode());
	}
	else {
		Node<T>* newRoot = new Node<T>(leftTree->GetMaxNode(), l_root, r_root);
		outTree->SetRoot(newRoot);
		outTree->SetHeight(left_height + 1);
		outTree->SetMinNode(leftTree->GetMinNode());
		outTree->SetMaxNode(rightTree->GetMaxNode());
	}
}

template<typename T>
inline CQueue<T>* CQueue<T>::Concatenate(CQueue<T>* leftTree, CQueue<T>* rightTree) {
	CQueue<T>* OutTree = new CQueue<T>();

	if (!leftTree) {
		OutTree->ShallowCopy(rightTree);
		rightTree->Clear();
		delete rightTree;
		return OutTree;
	}

	if (!rightTree) {
		OutTree->ShallowCopy(leftTree);
		leftTree->Clear();
		delete leftTree;
		return OutTree;
	}

	Node<T>* l_root = leftTree->GetRoot();
	Node<T>* r_root = rightTree->GetRoot();

	int left_height = leftTree->GetHeight();
	int right_height = rightTree->GetHeight();

	if (l_root == nullptr || left_height == -1) {
		OutTree->ShallowCopy(rightTree);
		rightTree->Clear();
		delete rightTree;
		return OutTree;
	}
	else if (r_root == nullptr || right_height == -1) {
		OutTree->ShallowCopy(leftTree);
		leftTree->Clear();
		delete leftTree;
		return OutTree;
	}

	rightTree->GetMinNode()->setChainLeft(leftTree->GetMaxNode());

	if (left_height > right_height) {
		if (l_root->getNumVal() == 3) {
			l_root = leftTree->NodeSplit(l_root);
			left_height = leftTree->GetHeight();
		}

		Node<T>* itr = l_root;
		int lev_desc = left_height - (right_height + 1);

		while (lev_desc) {
			itr = itr->getRCh();
			if (itr->getNumVal() == 3) leftTree->NodeSplit(itr);
			--lev_desc;
		}

		if (itr->getNumVal() == 1) {
			itr->setM1Ch(itr->getRCh());
		}
		else {
			itr->setMMax(itr->getRMax());
			itr->setM2Ch(itr->getRCh());
		}

		itr->setRMax(leftTree->GetMaxNode());
		itr->setRCh(r_root);
		leftTree->SetMaxNode(rightTree->GetMaxNode());

		OutTree->ShallowCopy(leftTree);
	}
	else if (left_height < right_height) {

		if (r_root->getNumVal() == 3) {
			r_root = rightTree->NodeSplit(r_root);
			right_height = rightTree->GetHeight();
		}

		Node<T>* itr = r_root;
		int lev_desc = right_height - (left_height + 1);

		while (lev_desc) {
			itr = itr->getLCh();
			if (itr->getNumVal() == 3) rightTree->NodeSplit(itr);
			--lev_desc;
		}

		if (itr->getNumVal() == 1) {
			itr->setRMax(itr->getLMax());
		}
		else {
			itr->setMMax(itr->getLMax());
			itr->setM2Ch(itr->getM1Ch());
		}

		itr->setM1Ch(itr->getLCh());
		itr->setLMax(leftTree->GetMaxNode());
		itr->setLCh(l_root);

		rightTree->SetMinNode(leftTree->GetMinNode());

		OutTree->ShallowCopy(rightTree);
	}
	else {
		Node<T>* newRoot = new Node<T>(leftTree->GetMaxNode(), l_root, r_root);
		OutTree->SetRoot(newRoot);
		OutTree->SetHeight(left_height + 1);
		OutTree->SetMinNode(leftTree->GetMinNode());
		OutTree->SetMaxNode(rightTree->GetMaxNode());
	}

	leftTree->SetRoot(nullptr);
	rightTree->SetRoot(nullptr);
	delete leftTree;
	delete rightTree;

	return OutTree;
}