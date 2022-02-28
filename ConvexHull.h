#pragma once
#include "CNode.h"

template <typename Point_2D>
class ConvexHull : public TTree<Point_2D>
{
	/**
	* @author Evripidis Pavlidis
	* @since 2020-5-12
	*/
protected:
	CNode<Point_2D>* root = nullptr;
	CNode<Point_2D>* minNode = nullptr;
	CNode<Point_2D>* maxNode = nullptr;
public:
	ConvexHull() {};
	~ConvexHull();

	CNode<Point_2D>* GetRoot();
	CNode<Point_2D>* GetMinNode();
	CNode<Point_2D>* GetMaxNode();

	void SetRoot(CNode<Point_2D>*);
	void SetMinNode(CNode<Point_2D>*);
	void SetMaxNode(CNode<Point_2D>*);

	void Insert(Point_2D*);
	void InsertAt(Point_2D*);
	CNode<Point_2D>* NodeSplit(CNode<Point_2D>*);
	void Delete(Point_2D);
	void DeleteAt(Point_2D p);
	void DeleteFromLeaf(CNode<Point_2D>*, Point_2D);
	CNode<Point_2D>* Transform2NodeTo3Node(CNode<Point_2D>* cn);

	void UP(CNode<Point_2D>* cn);
	void DOWN(CNode<Point_2D>* cn);

	std::vector<Point_2D> GetChainedLeaves();
	void PostorderDelete(CNode<Point_2D>* cn);

	void OutputHull(const std::string& FileName);
	void OutputPoints(const std::string& FileName);

	void DeletePoints();

	bool isValidTree();
	bool PostOrderValidNode(CNode<Point_2D>*);

};

template<typename Point_2D>
inline ConvexHull<Point_2D>::~ConvexHull()
{
	PostorderDelete(this->GetRoot());
}

template<typename Point_2D>
inline CNode<Point_2D>* ConvexHull<Point_2D>::GetRoot()
{
	return this->root;
}

template<typename Point_2D>
inline CNode<Point_2D>* ConvexHull<Point_2D>::GetMinNode()
{
	return this->minNode;
}

template<typename Point_2D>
inline CNode<Point_2D>* ConvexHull<Point_2D>::GetMaxNode()
{
	return this->maxNode;
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::SetRoot(CNode<Point_2D>* cn)
{
	this->root = cn;
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::SetMinNode(CNode<Point_2D>* cmin)
{
	this->minNode = cmin;
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::SetMaxNode(CNode<Point_2D>* cmax)
{
	this->maxNode = cmax;
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::Insert(Point_2D* p)
{
	if (!root) {
		root = new CNode<Point_2D>(p);
		++this->height;
		SetMinNode(root);
		SetMaxNode(root);
	}
	else {
		InsertAt(p);
	}
}

template <typename Point_2D>
inline void ConvexHull<Point_2D>::InsertAt(Point_2D* p) {
	CNode<Point_2D>* itr = GetRoot();
	if (itr->GetNodeSize() == 3) {
		itr = NodeSplit(itr);
	}

	while (!itr->getIsLeaf() || itr->GetNodeSize() == 3) {
		if (itr->GetNodeSize() == 3)
			itr = NodeSplit(itr);
		DOWN(itr);
		if (itr->GetNodeSize() == 1) {
			if (*p <= *itr->getLMax()->getMaxVal()) {
				itr = itr->getLCh();
			}
			else {
				itr = itr->getRCh();
			}
		}
		else if (itr->GetNodeSize() == 2) {
			if (*p <= *itr->getLMax()->getMaxVal()) {
				itr = itr->getLCh();
			}
			else if (*p <= *itr->getRMax()->getMaxVal()) {
				itr = itr->getM1Ch();
			}
			else {
				itr = itr->getRCh();
			}
		}
		else {
			if (*p <= *itr->getLMax()->getMaxVal()) {
				itr = itr->getLCh();
			}
			else if (*p <= *itr->getMMax()->getMaxVal()) {
				itr = itr->getM1Ch();
			}
			else if (*p <= *itr->getRMax()->getMaxVal()) {
				itr = itr->getM2Ch();
			}
			else {
				itr = itr->getRCh();
			}
		}
	}


	Point_2D* lVal = itr->getLVal();
	Point_2D* rVal = itr->getRVal();

	if (*p == *lVal || rVal && *p == *rVal) { return; }
	if (rVal) {
		if (*p > *rVal) {
			itr->setMVal(rVal);
			itr->setRVal(p);
		}
		else if (*p > *lVal) {
			itr->setMVal(p);
		}
		else {
			itr->setMVal(lVal);
			itr->setLVal(p);
		}
	}
	else {
		if (*p > *lVal) {
			itr->setRVal(p);
		}
		else {
			itr->setRVal(lVal);
			itr->setLVal(p);
		}
	}

	itr->CalculateLeafLSubHull();
	itr->CalculateLeafRSubHull();
	itr = itr->getFather();

	while (itr) {
		UP(itr);
		itr = itr->getFather();
	}

}

template<typename Point_2D>
inline CNode<Point_2D>* ConvexHull<Point_2D>::NodeSplit(CNode<Point_2D>* cn)
{
	if (cn == GetRoot()) {
		if (cn->getIsLeaf()) {
			CNode<Point_2D>* newLeft = new CNode<Point_2D>(cn->getLVal(), cn->getMVal());
			CNode<Point_2D>* newRight = new CNode<Point_2D>(cn->getRVal());
			CNode<Point_2D>* newRoot = new CNode<Point_2D>(newLeft, newLeft, newRight);
			CNode<Point_2D>* mem_clear = root;

			SetRoot(newRoot);
			++this->height;
			SetMinNode(newLeft);
			SetMaxNode(newRight);

			delete mem_clear;
		}
		else {
			DOWN(cn);
			CNode<Point_2D>* newLeft = new CNode<Point_2D>(cn->getLMax(), cn->getLCh(), cn->getM1Ch());
			CNode<Point_2D>* newRight = new CNode<Point_2D>(cn->getRMax(), cn->getM2Ch(), cn->getRCh());

			CNode<Point_2D>* newRoot = new CNode<Point_2D>(cn->getMMax(), newLeft, newRight);
			CNode<Point_2D>* mem_clear = root;

			SetRoot(newRoot);
			++this->height;
			delete mem_clear;
		}
		return root;
	}

	CNode<Point_2D>* FNode = cn->getFather();

	if (cn->getIsLeaf()) {
		Point_2D* mVal = cn->getMVal();
		if (FNode->GetNodeSize() == 1) {
			if (cn == FNode->getLCh()) {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getRVal());

				FNode->setRMax(newMiddle);
				FNode->setM1Ch(newMiddle);

				cn->setRVal(mVal);
				cn->setMVal(nullptr);

				newMiddle->setChainLeft(cn);
				FNode->getRCh()->setChainLeft(newMiddle);

			}
			else {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getLVal(), mVal);

				FNode->setRMax(newMiddle);
				FNode->setM1Ch(newMiddle);

				cn->setLVal(cn->getRVal());
				cn->setMVal(nullptr);
				cn->setRVal(nullptr);

				newMiddle->setChainLeft(FNode->getLCh());
				cn->setChainLeft(newMiddle);

			}
		}
		else {
			if (cn == FNode->getLCh()) {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getRVal());

				FNode->setM2Ch(FNode->getM1Ch());
				FNode->setM1Ch(newMiddle);
				FNode->setMMax(newMiddle);

				cn->setRVal(mVal);
				cn->setMVal(nullptr);

				newMiddle->setChainLeft(cn);
				FNode->getM2Ch()->setChainLeft(newMiddle);

			}
			else if (cn == FNode->getM1Ch()) {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getRVal());

				FNode->setM2Ch(newMiddle);
				FNode->setMMax(cn);
				FNode->setRMax(newMiddle);

				cn->setMVal(nullptr);
				cn->setRVal(mVal);

				newMiddle->setChainLeft(cn);
				FNode->getRCh()->setChainLeft(newMiddle);

			}
			else {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getLVal(), mVal);

				FNode->setM2Ch(newMiddle);
				FNode->setMMax(FNode->getRMax());
				FNode->setRMax(newMiddle);

				cn->setLVal(cn->getRVal());
				cn->setMVal(nullptr);
				cn->setRVal(nullptr);

				newMiddle->setChainLeft(FNode->getM1Ch());
				cn->setChainLeft(newMiddle);

			}
		}
		cn->CalculateLeafLSubHull();
		cn->CalculateLeafRSubHull();
	}
	else {
		CNode<Point_2D>* mMax = cn->getMMax();
		DOWN(cn);
		if (FNode->GetNodeSize() == 1) {
			if (cn == FNode->getLCh()) {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getRMax(), cn->getM2Ch(), cn->getRCh());
				FNode->setRMax(FNode->getLMax());
				FNode->setLMax(mMax);
				FNode->setM1Ch(newMiddle);

				cn->setMMax(nullptr);
				cn->setRMax(nullptr);
				cn->setRCh(cn->getM1Ch());
				cn->setM1Ch(nullptr);
				cn->setM2Ch(nullptr);

			}
			else {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getLMax(), cn->getLCh(), cn->getM1Ch());
				FNode->setRMax(mMax);
				FNode->setM1Ch(newMiddle);

				cn->setLMax(cn->getRMax());
				cn->setMMax(nullptr);
				cn->setRMax(nullptr);
				cn->setLCh(cn->getM2Ch());
				cn->setM1Ch(nullptr);
				cn->setM2Ch(nullptr);

			}
		}
		else {
			if (cn == FNode->getLCh()) {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getRMax(), cn->getM2Ch(), cn->getRCh());
				FNode->setMMax(FNode->getLMax());
				FNode->setLMax(mMax);
				FNode->setM2Ch(FNode->getM1Ch());
				FNode->setM1Ch(newMiddle);

				cn->setMMax(nullptr);
				cn->setRMax(nullptr);
				cn->setRCh(cn->getM1Ch());
				cn->setM2Ch(nullptr);
				cn->setM1Ch(nullptr);

			}
			else if (cn == FNode->getM1Ch()) {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getRMax(), cn->getM2Ch(), cn->getRCh());
				FNode->setMMax(mMax);
				FNode->setM2Ch(newMiddle);

				cn->setMMax(nullptr);
				cn->setRMax(nullptr);
				cn->setRCh(cn->getM1Ch());
				cn->setM1Ch(nullptr);
				cn->setM2Ch(nullptr);

			}
			else {
				CNode<Point_2D>* newMiddle = new CNode<Point_2D>(cn->getLMax(), cn->getLCh(), cn->getM1Ch());
				FNode->setMMax(FNode->getRMax());
				FNode->setRMax(mMax);
				FNode->setM2Ch(newMiddle);

				cn->setLMax(cn->getRMax());
				cn->setMMax(nullptr);
				cn->setRMax(nullptr);
				cn->setLCh(cn->getM2Ch());
				cn->setM1Ch(nullptr);
				cn->setM2Ch(nullptr);

			}
		}
		UP(cn);
	}
	UP(FNode);
	return FNode;
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::Delete(Point_2D p)
{
	if (!root)
		return;
	else if (root->getIsLeaf()) {
		DeleteFromLeaf(root, p);
	}
	else {
		DeleteAt(root, p);
	}
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::DeleteAt(Point_2D p)
{
	CNode<Point_2D>* cn = GetRoot();

	while (!cn->getIsLeaf())
	{
		if (cn->GetNodeSize() == 1 && cn != GetRoot()) {
			cn = Transform2NodeTo3Node(cn);
		}

		DOWN(cn);

		if (cn->getRMax()) {
			if (*cn->getRMax()->getMaxVal() < p) {
				cn = cn->getRCh();
			}
			else {
				if (cn->getMMax()) {
					if (*cn->getMMax()->getMaxVal() < p) {
						cn = cn->getM2Ch();
					}
					else {
						if (*cn->getLMax()->getMaxVal() < p) {
							cn = cn->getM1Ch();
						}
						else {
							cn = cn->getLCh();
						}
					}
				}
				else {
					if (*cn->getLMax()->getMaxVal() < p) {
						cn = cn->getM1Ch();
					}
					else {
						cn = cn->getLCh();
					}
				}
			}
		}
		else {
			if (*cn->getLMax()->getMaxVal() < p) {
				cn = cn->getRCh();
			}
			else {
				cn = cn->getLCh();
			}
		}
	}

	if (cn->GetNodeSize() == 1) {
		cn = Transform2NodeTo3Node(cn);
	}

	DeleteFromLeaf(cn, p);

	while (cn) {
		if (!cn->GetLHull()) {
			UP(cn);
		}
		cn = cn->getFather();
	}
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::DeleteFromLeaf(CNode<Point_2D>* cn, Point_2D p)
{
	int NodeSize = cn->GetNodeSize();

	if (cn == GetRoot() || NodeSize >= 2) {
		if (p == *cn->getLVal()) {
			if (NodeSize == 1) {

				delete cn;

				SetRoot(nullptr);
				SetMinNode(nullptr);
				SetMaxNode(nullptr);
				TTree<Point_2D>::SetHeight(-1);

				return;
			}
			else if (NodeSize == 2) {
				cn->setLVal(cn->getRVal());
				cn->setRVal(nullptr);
			}
			else {
				cn->setLVal(cn->getMVal());
				cn->setMVal(nullptr);
			}
		}
		else if (cn->getRVal() && p == *cn->getRVal()) {
			if (NodeSize == 2) {
				cn->setRVal(nullptr);
			}
			else {
				cn->setRVal(cn->getMVal());
				cn->setMVal(nullptr);
			}
		}
		else if (cn->getMVal() && p == *cn->getMVal()) {
			cn->setMVal(nullptr);
		}
	}

	cn->CalculateLeafLSubHull();
	cn->CalculateLeafRSubHull();
}

template<typename Point_2D>
inline CNode<Point_2D>* ConvexHull<Point_2D>::Transform2NodeTo3Node(CNode<Point_2D>* cn)
{
	CNode<Point_2D>* LSibling = cn->getLSibling();
	CNode<Point_2D>* RSibling = cn->getRSibling();
	CNode<Point_2D>* FNode = cn->getFather();


	if (FNode->GetNodeSize() == 1 && FNode->getLCh()->GetNodeSize() == 1 && FNode->getRCh()->GetNodeSize() == 1) {
		if (cn->getIsLeaf()) {
			CNode<Point_2D>* newRoot = new CNode<Point_2D>(FNode->getLCh()->getLVal(), FNode->getRCh()->getLVal());
			CNode<Point_2D>* mem_clear = GetRoot();

			SetMinNode(newRoot);
			SetMaxNode(newRoot);
			--this->height;

			SetRoot(newRoot);

			delete mem_clear->getLCh();
			delete mem_clear->getRCh();
			delete mem_clear;

			return newRoot;
		}
		else {
			Point_2D* LBridge;
			Point_2D* RBridge;

			CNode<Point_2D>* lCh = FNode->getLCh();
			CNode<Point_2D>* rCh = FNode->getRCh();

			// Node info
			FNode->setMMax(FNode->getLMax());
			FNode->setLMax(lCh->getLMax());
			FNode->setRMax(rCh->getLMax());

			// Left Child Left Hull info migration
			FNode->SetLLeftIntermediateHull(lCh->GetLHull());
			FNode->SetLLeftBridge(lCh->GetLBridge());

			// Left Child Right Hull info migration
			FNode->SetRLeftIntermediateHull(lCh->GetRHull());
			FNode->SetRLeftBridge(lCh->GetRBridge());

			// Right Child Left Hull info migration
			FNode->SetLRightIntermediateHull(rCh->GetLHull());
			FNode->SetLRightBridge(rCh->GetLBridge());

			// Right Child Right Hull info migration
			FNode->SetRRightIntermediateHull(rCh->GetRHull());
			FNode->SetRRightBridge(rCh->GetRBridge());

			// Node Left and Right Hulls
			FNode->SetLHull(SubHull<Point_2D>::Bridge(FNode->GetLLeftIntermediateHull(), FNode->GetLRightIntermediateHull(), &LBridge, 0));
			FNode->SetLBridge(LBridge);
			FNode->SetRHull(SubHull<Point_2D>::Bridge(FNode->GetRLeftIntermediateHull(), FNode->GetRRightIntermediateHull(), &RBridge, 0));
			FNode->SetRBridge(RBridge);

			// Node Children info
			FNode->setLCh(lCh->getLCh());
			FNode->setM1Ch(lCh->getRCh());
			FNode->setM2Ch(rCh->getLCh());
			FNode->setRCh(rCh->getRCh());

			--this->height;

			// cleanup

			lCh->SetLHull(nullptr);
			rCh->SetLHull(nullptr);
			lCh->SetRHull(nullptr);
			rCh->SetRHull(nullptr);

			delete lCh;
			delete rCh;

		}
		return FNode;
	}

	if (LSibling) {

		if (LSibling->GetNodeSize() >= 2) {

			if (cn->getIsLeaf()) {

				cn->setRVal(cn->getLVal());
				cn->setLVal(LSibling->getRVal());

				cn->CalculateLeafLSubHull();
				cn->CalculateLeafRSubHull();

				LSibling->setRVal(LSibling->getMVal());
				LSibling->setMVal(nullptr);

				LSibling->CalculateLeafLSubHull();
				LSibling->CalculateLeafRSubHull();

			}
			else {
				DOWN(cn);
				DOWN(LSibling);

				cn->setRMax(cn->getLMax());
				cn->setM1Ch(cn->getLCh());
				cn->setLCh(LSibling->getRCh());

				if (cn == FNode->getM1Ch()) {
					cn->setLMax(FNode->getLMax());
					FNode->setLMax(LSibling->getRMax());

				}
				else if (cn == FNode->getM2Ch()) {
					cn->setLMax(FNode->getMMax());
					FNode->setMMax(LSibling->getRMax());

				}
				else {
					if (FNode->GetNodeSize() > 1) {
						cn->setLMax(FNode->getRMax());
						FNode->setRMax(LSibling->getRMax());

					}
					else {
						cn->setLMax(FNode->getLMax());
						FNode->setLMax(LSibling->getRMax());

					}
				}

				if (LSibling->GetNodeSize() == 2) {
					LSibling->setRMax(nullptr);
					LSibling->setRCh(LSibling->getM1Ch());
					LSibling->setM1Ch(nullptr);

				}
				else {
					LSibling->setRMax(LSibling->getMMax());
					LSibling->setMMax(nullptr);
					LSibling->setRCh(LSibling->getM2Ch());
					LSibling->setM2Ch(nullptr);

				}

				UP(cn);
				UP(LSibling);
			}
			return cn;
		}
		else {

			if (!RSibling || RSibling->GetNodeSize() == 1) {

				if (cn->getIsLeaf()) {

					if (cn == FNode->getRCh()) {

						cn->setRVal(cn->getLVal());
						cn->setLVal(LSibling->getLVal());

						if (FNode->GetNodeSize() == 2) {
							FNode->setRMax(nullptr);
							FNode->setM1Ch(nullptr);
						}
						else {
							FNode->setRMax(FNode->getMMax());
							FNode->setMMax(nullptr);
							FNode->setM2Ch(nullptr);

						}

						cn->CalculateLeafLSubHull();
						cn->CalculateLeafRSubHull();

						delete LSibling;

						return cn;

					}
					else {
						LSibling->setRVal(cn->getLVal());

						if (FNode->GetNodeSize() == 2) {
							FNode->setRMax(nullptr);
							FNode->setM1Ch(nullptr);
						}
						else {
							if (cn == FNode->getM1Ch()) {
								FNode->setM1Ch(FNode->getM2Ch());
							}
							else {
								FNode->setRMax(FNode->getMMax());
							}

							FNode->setMMax(nullptr);
							FNode->setM2Ch(nullptr);

						}

						delete cn;
						LSibling->CalculateLeafLSubHull();
						LSibling->CalculateLeafRSubHull();

						return LSibling;
					}
				}
				else {

					DOWN(cn);
					DOWN(LSibling);

					if (cn == FNode->getRCh()) {

						cn->setRMax(cn->getLMax());
						cn->setMMax(FNode->getRMax());
						cn->setLMax(LSibling->getLMax());

						cn->setM2Ch(cn->getLCh());
						cn->setM1Ch(LSibling->getRCh());
						cn->setLCh(LSibling->getLCh());

						if (FNode->GetNodeSize() == 2) {
							FNode->setRMax(nullptr);
							FNode->setM1Ch(nullptr);

						}
						else {
							FNode->setRMax(FNode->getMMax());
							FNode->setMMax(nullptr);
							FNode->setM2Ch(nullptr);

						}

						delete LSibling;
						UP(cn);

						return cn;
					}
					else {

						LSibling->setM1Ch(LSibling->getRCh());
						LSibling->setM2Ch(cn->getLCh());
						LSibling->setRCh(cn->getRCh());

						LSibling->setRMax(cn->getLMax());

						if (cn == FNode->getM2Ch()) {
							LSibling->setMMax(FNode->getMMax());
							FNode->setMMax(nullptr);
							FNode->setM2Ch(nullptr);

						}
						else {
							LSibling->setMMax(FNode->getLMax());
							if (FNode->GetNodeSize() == 2) {
								FNode->setLMax(FNode->getRMax());
								FNode->setRMax(nullptr);
								FNode->setM1Ch(nullptr);

							}
							else {
								FNode->setLMax(FNode->getMMax());
								FNode->setMMax(nullptr);
								FNode->setM1Ch(FNode->getM2Ch());
								FNode->setM2Ch(nullptr);

							}
						}

						delete cn;
						UP(LSibling);

						return LSibling;
					}
				}
			}
		}
	}

	if (RSibling) {

		if (RSibling->GetNodeSize() >= 2) {

			if (cn->getIsLeaf()) {

				cn->setRVal(RSibling->getLVal());

				cn->CalculateLeafLSubHull();
				cn->CalculateLeafRSubHull();

				if (RSibling->GetNodeSize() == 2) {

					RSibling->setLVal(RSibling->getRVal());
					RSibling->setRVal(nullptr);

				}
				else {

					RSibling->setLVal(RSibling->getMVal());
					RSibling->setMVal(nullptr);

				}

				RSibling->CalculateLeafLSubHull();
				RSibling->CalculateLeafRSubHull();

			}
			else {

				DOWN(cn);
				DOWN(RSibling);

				cn->setM1Ch(cn->getRCh());
				cn->setRCh(RSibling->getLCh());

				RSibling->setLCh(RSibling->getM1Ch());

				if (cn == FNode->getLCh()) {
					cn->setRMax(FNode->getLMax());
					FNode->setLMax(RSibling->getLMax());

				}
				else if (cn == FNode->getM2Ch()) {
					cn->setRMax(FNode->getRMax());
					FNode->setRMax(RSibling->getLMax());

					//
					//cn->isValidNode();
					//FNode->isValidNode();
					//
				}
				else {
					if (FNode->GetNodeSize() == 2) {
						cn->setRMax(FNode->getRMax());
						FNode->setRMax(RSibling->getLMax());

					}
					else {
						cn->setRMax(FNode->getMMax());
						FNode->setMMax(RSibling->getLMax());

					}
				}

				if (RSibling->GetNodeSize() == 2) {
					RSibling->setLMax(RSibling->getRMax());
					RSibling->setRMax(nullptr);
					RSibling->setM1Ch(nullptr);

				}
				else {
					RSibling->setLMax(RSibling->getMMax());
					RSibling->setMMax(nullptr);
					RSibling->setM1Ch(RSibling->getM2Ch());
					RSibling->setM2Ch(nullptr);

				}

				UP(cn);
				UP(RSibling);
			}
			return cn;
		}
		else {

			if (cn->getIsLeaf()) {

				if (cn == FNode->getLCh()) {

					cn->setRVal(RSibling->getLVal());

					if (FNode->GetNodeSize() == 3) {
						FNode->setMMax(nullptr);
						FNode->setM1Ch(FNode->getM2Ch());
						FNode->setM2Ch(nullptr);

					}
					else {
						FNode->setRMax(nullptr);
						FNode->setM1Ch(nullptr);

					}

					delete RSibling;
					cn->CalculateLeafLSubHull();
					cn->CalculateLeafRSubHull();

					return cn;
				}
				else if (cn == FNode->getM1Ch()) {
					if (FNode->GetNodeSize() == 2) {

						FNode->setRMax(nullptr);
						FNode->setM1Ch(nullptr);

						RSibling->setRVal(RSibling->getLVal());
						RSibling->setLVal(cn->getLVal());

						RSibling->CalculateLeafLSubHull();
						RSibling->CalculateLeafRSubHull();

						delete cn;

						return RSibling;
					}
					else {
						cn->setRVal(RSibling->getLVal());

						FNode->setRMax(FNode->getMMax());
						FNode->setMMax(nullptr);
						FNode->setM2Ch(nullptr);

						delete RSibling;
						cn->CalculateLeafLSubHull();

						return cn;
					}
				}
				else {
					FNode->setRMax(FNode->getMMax());
					FNode->setMMax(nullptr);
					FNode->setM2Ch(nullptr);

					RSibling->setRVal(RSibling->getLVal());
					RSibling->setLVal(cn->getLVal());

					RSibling->CalculateLeafLSubHull();
					RSibling->CalculateLeafRSubHull();

					delete cn;

					return RSibling;
				}
			}
			else {
				DOWN(cn);
				DOWN(RSibling);

				if (cn == FNode->getLCh()) {

					cn->setMMax(FNode->getLMax());
					cn->setRMax(RSibling->getLMax());

					cn->setM1Ch(cn->getRCh());
					cn->setM2Ch(RSibling->getLCh());
					cn->setRCh(RSibling->getRCh());

					if (FNode->GetNodeSize() == 3) {
						FNode->setLMax(FNode->getMMax());
						FNode->setMMax(nullptr);

						FNode->setM1Ch(FNode->getM2Ch());
						FNode->setM2Ch(nullptr);

					}
					else {
						FNode->setLMax(FNode->getRMax());
						FNode->setRMax(nullptr);

						FNode->setM1Ch(nullptr);

					}

					delete RSibling;

					UP(cn);
					return cn;
				}
				else if (cn == FNode->getM1Ch()) {

					if (FNode->GetNodeSize() == 2) {

						RSibling->setRMax(RSibling->getLMax());
						RSibling->setMMax(FNode->getRMax());
						RSibling->setLMax(cn->getLMax());

						RSibling->setM2Ch(RSibling->getLCh());
						RSibling->setM1Ch(cn->getRCh());
						RSibling->setLCh(cn->getLCh());

						FNode->setM1Ch(nullptr);

						FNode->setRMax(nullptr);

						delete cn;

						UP(RSibling);
						return RSibling;
					}
					else {

						cn->setMMax(FNode->getMMax());
						cn->setRMax(RSibling->getLMax());

						cn->setM1Ch(cn->getRCh());
						cn->setM2Ch(RSibling->getLCh());
						cn->setRCh(RSibling->getRCh());

						FNode->setMMax(nullptr);

						FNode->setM2Ch(nullptr);

						delete RSibling;

						UP(cn);
						return cn;
					}
				}
				else {
					RSibling->setRMax(RSibling->getLMax());
					RSibling->setMMax(FNode->getRMax());
					RSibling->setLMax(cn->getLMax());

					RSibling->setM2Ch(RSibling->getLCh());
					RSibling->setM1Ch(cn->getRCh());
					RSibling->setLCh(cn->getLCh());

					FNode->setRMax(FNode->getMMax());

					FNode->setMMax(nullptr);
					FNode->setM2Ch(nullptr);

					delete cn;

					UP(RSibling);
					return RSibling;
				}
			}
		}
	}
	std::cerr << "Undefined Behavior in node transformation." << std::endl;
	return nullptr;
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::UP(CNode<Point_2D>* cn)
{
	CNode<Point_2D>* LCh = cn->getLCh();
	CNode<Point_2D>* M1Ch = cn->getM1Ch();
	CNode<Point_2D>* M2Ch = cn->getM2Ch();
	CNode<Point_2D>* RCh = cn->getRCh();

	Point_2D* LBridge = cn->GetLBridge();
	Point_2D* LLeftBridge = cn->GetLLeftBridge();
	Point_2D* LRightBridge = cn->GetLRightBridge();

	Point_2D* RBridge = cn->GetRBridge();
	Point_2D* RLeftBridge = cn->GetRLeftBridge();
	Point_2D* RRightBridge = cn->GetRRightBridge();

	if (cn->GetNodeSize() == 1) {
		SubHull<Point_2D>* LeftLHull = LCh->GetLHull();
		SubHull<Point_2D>* RightLHull = RCh->GetLHull();
		
		cn->SetLHull(SubHull<Point_2D>::Bridge(LeftLHull, RightLHull, &LBridge, 0));
		cn->SetLBridge(LBridge);

		if (LeftLHull && LeftLHull->GetHeight() == -1) {
			delete LeftLHull;
			LCh->SetLHull(nullptr);
		}

		if (RightLHull && RightLHull->GetHeight() == -1) {
			delete RightLHull;
			RCh->SetLHull(nullptr);
		}

		SubHull<Point_2D>* LeftRHull = LCh->GetRHull();
		SubHull<Point_2D>* RightRHull = RCh->GetRHull();

		cn->SetRHull(SubHull<Point_2D>::Bridge(LeftRHull, RightRHull, &RBridge, 1));
		cn->SetRBridge(RBridge);

		if (LeftRHull && LeftRHull->GetHeight() == -1) {
			delete LeftRHull;
			LCh->SetRHull(nullptr);
		}

		if (RightRHull && RightRHull->GetHeight() == -1) {
			delete RightRHull;
			RCh->SetRHull(nullptr);
		}
 
	}
	else if (cn->GetNodeSize() == 2) {
		SubHull<Point_2D>* LLIHull = cn->GetLLeftIntermediateHull();
		SubHull<Point_2D>* LeftLHull = LCh->GetLHull();
		SubHull<Point_2D>* M1LHull = M1Ch->GetLHull();
		SubHull<Point_2D>* RightLHull = RCh->GetLHull();

		cn->SetLLeftIntermediateHull(SubHull<Point_2D>::Bridge(LeftLHull, M1LHull, &LLeftBridge, 0));
		cn->SetLLeftBridge(LLeftBridge);

		LLIHull = cn->GetLLeftIntermediateHull();

		cn->SetLHull(SubHull<Point_2D>::Bridge(LLIHull, RightLHull, &LBridge, 0));
		cn->SetLBridge(LBridge);

		if (LeftLHull && LeftLHull->GetHeight() == -1) {
			delete LeftLHull;
			LCh->SetLHull(nullptr);
		}

		if (M1LHull && M1LHull->GetHeight() == -1) {
			delete M1LHull;
			M1Ch->SetLHull(nullptr);
		}

		if (RightLHull && RightLHull->GetHeight() == -1) {
			delete RightLHull;
			RCh->SetLHull(nullptr);
		}

		if (LLIHull && LLIHull->GetHeight() == -1) {
			delete LLIHull;
			cn->SetLLeftIntermediateHull(nullptr);
		}


		SubHull<Point_2D>* RLIHull = cn->GetRLeftIntermediateHull();
		SubHull<Point_2D>* LeftRHull = LCh->GetRHull();
		SubHull<Point_2D>* M1RHull = M1Ch->GetRHull();
		SubHull<Point_2D>* RightRHull = RCh->GetRHull();

		cn->SetRLeftIntermediateHull(SubHull<Point_2D>::Bridge(LeftRHull, M1RHull, &RLeftBridge, 1));
		cn->SetRLeftBridge(RLeftBridge);

		RLIHull = cn->GetRLeftIntermediateHull();

		cn->SetRHull(SubHull<Point_2D>::Bridge(RLIHull, RightRHull, &RBridge, 1));
		cn->SetRBridge(RBridge);

		if (LeftRHull && LeftRHull->GetHeight() == -1) {
			delete LeftRHull;
			LCh->SetRHull(nullptr);
		}

		if (M1RHull && M1RHull->GetHeight() == -1) {
			delete M1RHull;
			M1Ch->SetRHull(nullptr);
		}

		if (RightRHull && RightRHull->GetHeight() == -1) {
			delete RightRHull;
			RCh->SetRHull(nullptr);
		}

		if (RLIHull && RLIHull->GetHeight() == -1) {
			delete RLIHull;
			cn->SetRLeftIntermediateHull(nullptr);
		}
	}
	else {
		SubHull<Point_2D>* LLIHull = cn->GetLLeftIntermediateHull();
		SubHull<Point_2D>* LRIHull = cn->GetLRightIntermediateHull();
		SubHull<Point_2D>* LeftLHull = LCh->GetLHull();
		SubHull<Point_2D>* M1LHull = M1Ch->GetLHull();
		SubHull<Point_2D>* M2LHull = M2Ch->GetLHull();
		SubHull<Point_2D>* RightLHull = RCh->GetLHull();

		cn->SetLLeftIntermediateHull(SubHull<Point_2D>::Bridge(LeftLHull, M1LHull, &LLeftBridge, 0));
		cn->SetLLeftBridge(LLeftBridge);

		LLIHull = cn->GetLLeftIntermediateHull();

		cn->SetLRightIntermediateHull(SubHull<Point_2D>::Bridge(M2LHull, RightLHull, &LRightBridge, 0));
		cn->SetLRightBridge(LRightBridge);

		LRIHull = cn->GetLRightIntermediateHull();

		cn->SetLHull(SubHull<Point_2D>::Bridge(LLIHull, LRIHull, &LBridge, 0));
		cn->SetLBridge(LBridge);

		if (LeftLHull && LeftLHull->GetHeight() == -1) {
			delete LeftLHull;
			LCh->SetLHull(nullptr);
		}

		if (M1LHull && M1LHull->GetHeight() == -1) {
			delete M1LHull;
			M1Ch->SetLHull(nullptr);
		}

		if (M2LHull && M2LHull->GetHeight() == -1) {
			delete M2LHull;
			M2Ch->SetLHull(nullptr);
		}

		if (RightLHull && RightLHull->GetHeight() == -1) {
			delete RightLHull;
			RCh->SetLHull(nullptr);
		}

		if (LLIHull && LLIHull->GetHeight() == -1) {
			delete LLIHull;
			cn->SetLLeftIntermediateHull(nullptr);
		}

		if (LRIHull && LRIHull->GetHeight() == -1) {
			delete LRIHull;
			cn->SetLRightIntermediateHull(nullptr);
		}


		SubHull<Point_2D>* RLIHull = cn->GetRLeftIntermediateHull();
		SubHull<Point_2D>* RRIHull = cn->GetRRightIntermediateHull();
		SubHull<Point_2D>* LeftRHull = LCh->GetRHull();
		SubHull<Point_2D>* M1RHull = M1Ch->GetRHull();
		SubHull<Point_2D>* M2RHull = M2Ch->GetRHull();
		SubHull<Point_2D>* RightRHull = RCh->GetRHull();

		cn->SetRLeftIntermediateHull(SubHull<Point_2D>::Bridge(LeftRHull, M1RHull, &RLeftBridge, 1));
		cn->SetRLeftBridge(RLeftBridge);

		RLIHull = cn->GetRLeftIntermediateHull();

		cn->SetRRightIntermediateHull(SubHull<Point_2D>::Bridge(M2RHull, RightRHull, &RRightBridge, 1));
		cn->SetRRightBridge(RRightBridge);

		RRIHull = cn->GetRRightIntermediateHull();

		cn->SetRHull(SubHull<Point_2D>::Bridge(RLIHull, RRIHull, &RBridge, 1));
		cn->SetRBridge(RBridge);

		if (LeftRHull && LeftRHull->GetHeight() == -1) {
			delete LeftRHull;
			LCh->SetRHull(nullptr);
		}

		if (M1RHull && M1RHull->GetHeight() == -1) {
			delete M1RHull;
			M1Ch->SetRHull(nullptr);
		}

		if (M2RHull && M2RHull->GetHeight() == -1) {
			delete M2RHull;
			M2Ch->SetRHull(nullptr);
		}

		if (RightRHull && RightRHull->GetHeight() == -1) {
			delete RightRHull;
			RCh->SetRHull(nullptr);
		}

		if (RLIHull && RLIHull->GetHeight() == -1) {
			delete RLIHull;
			cn->SetRLeftIntermediateHull(nullptr);
		}

		if (RRIHull && RRIHull->GetHeight() == -1) {
			delete RRIHull;
			cn->SetRRightIntermediateHull(nullptr);
		}
	}
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::DOWN(CNode<Point_2D>* cn)
{
	CNode<Point_2D>* LCh = cn->getLCh();
	CNode<Point_2D>* M1Ch = cn->getM1Ch();
	CNode<Point_2D>* M2Ch = cn->getM2Ch();
	CNode<Point_2D>* RCh = cn->getRCh();

	if (cn->GetNodeSize() == 1) {

		CQueue<Point_2D>* LTemp = CQueue<Point_2D>::Concatenate(cn->GetLHull()->Split(*cn->GetLBridge(), true, true), LCh->GetLHull());
		LCh->SetLHull(new SubHull<Point_2D>(LTemp));

		CQueue<Point_2D>* RTemp = CQueue<Point_2D>::Concatenate(RCh->GetLHull(), cn->GetLHull());
		RCh->SetLHull(new SubHull<Point_2D>(RTemp));

		LTemp->Clear();
		RTemp->Clear();

		delete LTemp;
		delete RTemp;

		LTemp = CQueue<Point_2D>::Concatenate(cn->GetRHull()->Split(*cn->GetRBridge(), true, true), LCh->GetRHull());
		LCh->SetRHull(new SubHull<Point_2D>(LTemp));

		RTemp = CQueue<Point_2D>::Concatenate(RCh->GetRHull(), cn->GetRHull());
		RCh->SetRHull(new SubHull<Point_2D>(RTemp));

		LTemp->Clear();
		RTemp->Clear();

		delete LTemp;
		delete RTemp;

	}
	else if (cn->GetNodeSize() == 2) {
		/* Split hull (LBridge) ==> leftIntermediate and other.
		* Split LeftIntermediate (LLeftBridge) ==> LCh and M1Ch hulls.
		* Concat to LCh and then to M1Ch the remainder of the LeftIntermediate
		* Concat to RCh the remainder of LHull.
		*/

		CQueue<Point_2D>* LInterTemp = CQueue<Point_2D>::Concatenate(cn->GetLHull()->Split(*cn->GetLBridge(), true, true), cn->GetLLeftIntermediateHull());

		CQueue<Point_2D>* LTemp = CQueue<Point_2D>::Concatenate(LInterTemp->Split(*cn->GetLLeftBridge(), true, true), LCh->GetLHull());
		LCh->SetLHull(new SubHull<Point_2D>(LTemp));

		CQueue<Point_2D>* M1Temp = CQueue<Point_2D>::Concatenate(M1Ch->GetLHull(), LInterTemp);
		M1Ch->SetLHull(new SubHull<Point_2D>(M1Temp));

		CQueue<Point_2D>* RTemp = CQueue<Point_2D>::Concatenate(RCh->GetLHull(), cn->GetLHull());
		RCh->SetLHull(new SubHull<Point_2D>(RTemp));

		LTemp->Clear();
		M1Temp->Clear();
		RTemp->Clear();

		delete LTemp;
		delete M1Temp;
		delete RTemp;

		LInterTemp = CQueue<Point_2D>::Concatenate(cn->GetRHull()->Split(*cn->GetRBridge(), true, true), cn->GetRLeftIntermediateHull());

		LTemp = CQueue<Point_2D>::Concatenate(LInterTemp->Split(*cn->GetRLeftBridge(), true, true), LCh->GetRHull());
		LCh->SetRHull(new SubHull<Point_2D>(LTemp));

		M1Temp = CQueue<Point_2D>::Concatenate(M1Ch->GetRHull(), LInterTemp);
		M1Ch->SetRHull(new SubHull<Point_2D>(M1Temp));

		RTemp = CQueue<Point_2D>::Concatenate(RCh->GetRHull(), cn->GetRHull());
		RCh->SetRHull(new SubHull<Point_2D>(RTemp));

		LTemp->Clear();
		M1Temp->Clear();
		RTemp->Clear();

		delete LTemp;
		delete M1Temp;
		delete RTemp;

	}
	else {
		/* Split hull (LBridge) ==>  concat right away from split to leftIntermediate and the remainder to rightIntermediate.
		* Split LeftIntermediate (LLeftBridge) ==> concat right away from split to LCh and the remainder to M1Ch hull.
		* Split RightIntermediate (LRightBridge) ==> concat right away from split to M2Ch and the remainder to RCh hull.
		*/

		CQueue<Point_2D>* LInterTemp = CQueue<Point_2D>::Concatenate(cn->GetLHull()->Split(*cn->GetLBridge(), true, true), cn->GetLLeftIntermediateHull());

		CQueue<Point_2D>* RInterTemp = CQueue<Point_2D>::Concatenate(cn->GetLRightIntermediateHull(), cn->GetLHull());

		CQueue<Point_2D>* LTemp = CQueue<Point_2D>::Concatenate(LInterTemp->Split(*cn->GetLLeftBridge(), true, true), LCh->GetLHull());
		LCh->SetLHull(new SubHull<Point_2D>(LTemp));

		CQueue<Point_2D>* M1Temp = CQueue<Point_2D>::Concatenate(M1Ch->GetLHull(), LInterTemp);
		M1Ch->SetLHull(new SubHull<Point_2D>(M1Temp));

		CQueue<Point_2D>* M2Temp = CQueue<Point_2D>::Concatenate(RInterTemp->Split(*cn->GetLRightBridge(), true, true), M2Ch->GetLHull());
		M2Ch->SetLHull(new SubHull<Point_2D>(M2Temp));

		CQueue<Point_2D>* RTemp = CQueue<Point_2D>::Concatenate(RCh->GetLHull(), RInterTemp);
		RCh->SetLHull(new SubHull<Point_2D>(RTemp));

		LTemp->Clear();
		M1Temp->Clear();
		M2Temp->Clear();
		RTemp->Clear();

		delete LTemp;
		delete M1Temp;
		delete M2Temp;
		delete RTemp;

		LInterTemp = CQueue<Point_2D>::Concatenate(cn->GetRHull()->Split(*cn->GetRBridge(), true, true), cn->GetRLeftIntermediateHull());

		RInterTemp = CQueue<Point_2D>::Concatenate(cn->GetRRightIntermediateHull(), cn->GetRHull());

		LTemp = CQueue<Point_2D>::Concatenate(LInterTemp->Split(*cn->GetRLeftBridge(), true, true), LCh->GetRHull());
		LCh->SetRHull(new SubHull<Point_2D>(LTemp));

		M1Temp = CQueue<Point_2D>::Concatenate(M1Ch->GetRHull(), LInterTemp);
		M1Ch->SetRHull(new SubHull<Point_2D>(M1Temp));

		M2Temp = CQueue<Point_2D>::Concatenate(RInterTemp->Split(*cn->GetRRightBridge(), true, true), M2Ch->GetRHull());
		M2Ch->SetRHull(new SubHull<Point_2D>(M2Temp));

		RTemp = CQueue<Point_2D>::Concatenate(RCh->GetRHull(), RInterTemp);
		RCh->SetRHull(new SubHull<Point_2D>(RTemp));

		LTemp->Clear();
		M1Temp->Clear();
		M2Temp->Clear();
		RTemp->Clear();

		delete LTemp;
		delete M1Temp;
		delete M2Temp;
		delete RTemp;
	}
	cn->DownReset();
}

template<typename Point_2D>
inline std::vector<Point_2D> ConvexHull<Point_2D>::GetChainedLeaves() {
	CNode<Point_2D>* root = GetRoot();
	if (!root)
		return std::vector<Point_2D>();
	else {
		std::vector<Point_2D> Output;
		CNode<Point_2D>* itr = GetMinNode();
		while (itr) {
			for (const Point_2D& p : itr->getAllVals()) {
				Output.push_back(p);
			}
			itr = itr->getChainRight();
		}
		return Output;
	}
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::PostorderDelete(CNode<Point_2D>* cn) {
	if (!cn) return;

	PostorderDelete(cn->getLCh());
	PostorderDelete(cn->getM1Ch());
	PostorderDelete(cn->getM2Ch());
	PostorderDelete(cn->getRCh());
	delete cn;
}

/// <summary>
/// This function outputs the points of the hull in clockwise rotation starting from the point with min y value.
/// </summary>
/// <typeparam name="Point_2D"></typeparam>
/// <param name="FileName"></param>
template<typename Point_2D>
inline void ConvexHull<Point_2D>::OutputHull(const std::string& FileName) 
{
	PROFILE_FUNCTION();

	CQueue<Point_2D>* LeftHull = GetRoot()->GetLHull();
	CQueue<Point_2D>* RightHull = GetRoot()->GetRHull();

	std::vector<Point_2D> LeftHullPoints = LeftHull->TTree<Point_2D>::GetChainedLeaves();
	std::vector<Point_2D> RightHullPoints = RightHull->TTree<Point_2D>::GetChainedLeaves();

	std::vector<Point_2D> FullHullPoints;
	FullHullPoints.insert(FullHullPoints.begin(), LeftHullPoints.begin(), LeftHullPoints.end());
	FullHullPoints.insert(FullHullPoints.end(), ++RightHullPoints.rbegin(), --RightHullPoints.rend());

	std::ofstream outFile;
	outFile.open(FileName.c_str());

	outFile << std::fixed << std::setprecision(11);

	for (const Point_2D& p : FullHullPoints) {
		outFile << p;
		if (&p != &FullHullPoints.back()) {
			outFile << ",";
		}
		outFile << '\n';
	}
	outFile.close();
}

/// <summary>
/// This function outputs all the points present in the dynamic structure.
/// </summary>
/// <typeparam name="Point_2D"></typeparam>
/// <param name="FileName"></param>
template<typename Point_2D>
inline void ConvexHull<Point_2D>::OutputPoints(const std::string& FileName)
{
	PROFILE_FUNCTION();

	std::ofstream outFile;
	outFile.open(FileName.c_str());

	outFile << std::fixed << std::setprecision(9);

	std::vector<Point_2D> LeavesValues = GetChainedLeaves();

	for (const Point_2D& p : LeavesValues) {
		outFile << p ;
		if (&p != &LeavesValues.back()) {
			outFile << ",";
		}
		outFile << '\n';
	}
	outFile.close();
}

template<typename Point_2D>
inline void ConvexHull<Point_2D>::DeletePoints()
{
	CNode<Point_2D>* itr = this->GetMinNode();
	
	while (itr->CNode<Point_2D>::getChainRight()) {
		itr->CNode<Point_2D>::deletePoints();
		itr = itr->CNode<Point_2D>::getChainRight();
	}

	itr->CNode<Point_2D>::deletePoints();
	return;
}

template<typename Point_2D>
inline bool ConvexHull<Point_2D>::isValidTree()
{
	CNode<Point_2D>* root = this->GetRoot();
	if (!root) return true;
	return PostOrderValidNode(root);
}

template<typename Point_2D>
inline bool ConvexHull<Point_2D>::PostOrderValidNode(CNode<Point_2D>* cn)
{
	if (!cn) return true;
	if (!cn->isValidNode()) return false;

	return (PostOrderValidNode(cn->getLCh()) &&
		PostOrderValidNode(cn->getM1Ch()) &&
		PostOrderValidNode(cn->getM2Ch()) &&
		PostOrderValidNode(cn->getRCh())
		);
}