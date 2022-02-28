#pragma once
#include "SubHull.h"

template <typename Point_2D>
class CNode : public Node<Point_2D>
{
	/**
	* @author Evripidis Pavlidis
	* @since 2020-5-12
	*/
protected:

	SubHull<Point_2D>* LHull = nullptr;
	SubHull<Point_2D>* LLeftIntermediateHull = nullptr;
	SubHull<Point_2D>* LRightIntermediateHull = nullptr;

	SubHull<Point_2D>* RHull = nullptr;
	SubHull<Point_2D>* RLeftIntermediateHull = nullptr;
	SubHull<Point_2D>* RRightIntermediateHull = nullptr;

	Point_2D* LBridge = nullptr;
	Point_2D* LLeftBridge = nullptr;
	Point_2D* LRightBridge = nullptr;

	Point_2D* RBridge = nullptr;
	Point_2D* RLeftBridge = nullptr;
	Point_2D* RRightBridge = nullptr;

	CNode* LMax = nullptr;
	CNode* MMax = nullptr;
	CNode* RMax = nullptr;

	CNode* LCh = nullptr;
	CNode* M1Ch = nullptr;
	CNode* M2Ch = nullptr;
	CNode* RCh = nullptr;

	CNode* Father = nullptr;
	CNode* chainLeft = nullptr;
	CNode* chainRight = nullptr;

public:
	CNode(Point_2D*);
	CNode(Point_2D*, Point_2D*);
	CNode(Point_2D*, Point_2D*, Point_2D*);
	CNode(CNode*, CNode*, CNode*);
	~CNode();

	SubHull<Point_2D>* GetLHull();
	SubHull<Point_2D>* GetLLeftIntermediateHull();
	SubHull<Point_2D>* GetLRightIntermediateHull();

	SubHull<Point_2D>* GetRHull();
	SubHull<Point_2D>* GetRLeftIntermediateHull();
	SubHull<Point_2D>* GetRRightIntermediateHull();

	Point_2D* GetLBridge();
	Point_2D* GetLLeftBridge();
	Point_2D* GetLRightBridge();

	Point_2D* GetRBridge();
	Point_2D* GetRLeftBridge();
	Point_2D* GetRRightBridge();

	CNode* getLMax();
	CNode* getMMax();
	CNode* getRMax();

	CNode* getLCh();
	CNode* getM1Ch();
	CNode* getM2Ch();
	CNode* getRCh();

	CNode* getLSibling();
	CNode* getRSibling();
	CNode* getFather();
	CNode* getChainLeft();
	CNode* getChainRight();

	int GetNodeSize();
	int GetNumCh();

	std::vector<Point_2D> getKeyVals();
	std::vector<Point_2D> getAllVals();
	std::vector<CNode<Point_2D>*> getAllCh();

	void SetLHull(SubHull<Point_2D>*);
	void SetLLeftIntermediateHull(SubHull<Point_2D>*);
	void SetLRightIntermediateHull(SubHull<Point_2D>*);

	void SetRHull(SubHull<Point_2D>*);
	void SetRLeftIntermediateHull(SubHull<Point_2D>*);
	void SetRRightIntermediateHull(SubHull<Point_2D>*);

	void SetLBridge(Point_2D*);
	void SetLLeftBridge(Point_2D*);
	void SetLRightBridge(Point_2D*);

	void SetRBridge(Point_2D*);
	void SetRLeftBridge(Point_2D*);
	void SetRRightBridge(Point_2D*);

	void DownReset();

	void setLMax(CNode<Point_2D>*);
	void setMMax(CNode<Point_2D>*);
	void setRMax(CNode<Point_2D>*);

	void setLCh(CNode<Point_2D>*);
	void setM1Ch(CNode<Point_2D>*);
	void setM2Ch(CNode<Point_2D>*);
	void setRCh(CNode<Point_2D>*);

	void setFather(CNode<Point_2D>*);
	void setChainLeft(CNode<Point_2D>*);
	void setChainRight(CNode<Point_2D>*);

	void CalculateLeafLSubHull();
	void CalculateLeafRSubHull();

	void clear();
	void deletePoints();
	bool isValidNode();

};

template<typename Point_2D>
inline CNode<Point_2D>::CNode(Point_2D* p)
{
	this->l = p;
	this->LMax = this;
	this->isLeaf = true;

	SetLHull(new SubHull<Point_2D>(p));
	SetRHull(new SubHull<Point_2D>(p));
}

template<typename Point_2D>
inline CNode<Point_2D>::CNode(Point_2D* l, Point_2D* r)
{
	this->l = l;
	this->r = r;
	this->LMax = this;
	this->isLeaf = true;

	SetLHull(new SubHull<Point_2D>(l));
	LHull->TTree<Point_2D>::Insert(r);

	SetRHull(new SubHull<Point_2D>(l));
	RHull->TTree<Point_2D>::Insert(r);
}

template<typename Point_2D>
inline CNode<Point_2D>::CNode(Point_2D* l, Point_2D* m, Point_2D* r)
{
	this->l = l;
	this->m = m;
	this->r = r;
	this->LMax = this;
	this->isLeaf = true;

	SetLHull(this->CalculateLeafLSubHull());
	SetRHull(this->CalculateLeafRSubHull());
}

template<typename Point_2D>
inline CNode<Point_2D>::CNode(CNode* lmax, CNode* lch, CNode* rch)
{
	this->LMax = lmax;
	this->setLCh(lch);
	this->setRCh(rch);
	this->isLeaf = false;

	SetLHull(GetLHull()->Bridge(lch->GetLHull(), rch->GetLHull(), &LBridge, 0));

	if (!lch->GetLHull()->GetRoot()) {
		delete lch->GetLHull();
		lch->SetLHull(nullptr);
	}

	if (!rch->GetLHull()->GetRoot()) {
		delete rch->GetLHull();
		rch->SetLHull(nullptr);
	}
	
	SetRHull(GetRHull()->Bridge(lch->GetRHull(), rch->GetRHull(), &RBridge, 1));

	if (!lch->GetRHull()->GetRoot()) {
		delete lch->GetRHull();
		lch->SetRHull(nullptr);
	}

	if (!rch->GetRHull()->GetRoot()) {
		delete rch->GetRHull();
		rch->SetRHull(nullptr);
	}
}

template<typename Point_2D>
inline CNode<Point_2D>::~CNode()
{
	delete LHull;
	if (LLeftIntermediateHull) delete LLeftIntermediateHull;
	if (LRightIntermediateHull) delete LRightIntermediateHull;
	delete RHull;
	if (RLeftIntermediateHull) delete RLeftIntermediateHull;
	if (RRightIntermediateHull) delete RRightIntermediateHull;
}

template<typename Point_2D>
inline SubHull<Point_2D>* CNode<Point_2D>::GetLHull()
{
	return this->LHull;
}

template<typename Point_2D>
inline SubHull<Point_2D>* CNode<Point_2D>::GetLLeftIntermediateHull()
{
	return this->LLeftIntermediateHull;
}

template<typename Point_2D>
inline SubHull<Point_2D>* CNode<Point_2D>::GetLRightIntermediateHull()
{
	return this->LRightIntermediateHull;
}

template<typename Point_2D>
inline SubHull<Point_2D>* CNode<Point_2D>::GetRHull()
{
	return this->RHull;
}

template<typename Point_2D>
inline SubHull<Point_2D>* CNode<Point_2D>::GetRLeftIntermediateHull()
{
	return this->RLeftIntermediateHull;
}

template<typename Point_2D>
inline SubHull<Point_2D>* CNode<Point_2D>::GetRRightIntermediateHull()
{
	return this->RRightIntermediateHull;
}

template<typename Point_2D>
inline Point_2D* CNode<Point_2D>::GetLBridge()
{
	return this->LBridge;
}

template<typename Point_2D>
inline Point_2D* CNode<Point_2D>::GetLLeftBridge()
{
	return this->LLeftBridge;
}

template<typename Point_2D>
inline Point_2D* CNode<Point_2D>::GetLRightBridge()
{
	return this->LRightBridge;
}

template<typename Point_2D>
inline Point_2D* CNode<Point_2D>::GetRBridge()
{
	return this->RBridge;
}

template<typename Point_2D>
inline Point_2D* CNode<Point_2D>::GetRLeftBridge()
{
	return this->RLeftBridge;
}

template<typename Point_2D>
inline Point_2D* CNode<Point_2D>::GetRRightBridge()
{
	return this->RRightBridge;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getLMax()
{
	return this->LMax;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getMMax()
{
	return this->MMax;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getRMax()
{
	return this->RMax;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getLCh()
{
	return this->LCh;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getM1Ch()
{
	return this->M1Ch;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getM2Ch()
{
	return this->M2Ch;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getRCh()
{
	return this->RCh;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getLSibling()
{
	CNode<Point_2D>* fatherNode = this->getFather();
	if (!fatherNode) return nullptr;
	CNode<Point_2D>* fathersLeftCh = fatherNode->getLCh();
	CNode<Point_2D>* fathersM1Ch = fatherNode->getM1Ch();
	CNode<Point_2D>* fathersM2Ch = fatherNode->getM2Ch();
	CNode<Point_2D>* fathersRightCh = fatherNode->getRCh();

	if (this == fathersLeftCh) return nullptr;
	if (this == fathersM1Ch) return fathersLeftCh;
	if (this == fathersM2Ch) return fathersM1Ch;
	if (fathersM2Ch != nullptr) return fathersM2Ch;
	else if (fathersM1Ch != nullptr) return fathersM1Ch;
	else return fathersLeftCh;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getRSibling()
{
	CNode<Point_2D>* fatherNode = this->getFather();
	if (!fatherNode) return nullptr;
	CNode<Point_2D>* fathersLeftCh = fatherNode->getLCh();
	CNode<Point_2D>* fathersM1Ch = fatherNode->getM1Ch();
	CNode<Point_2D>* fathersM2Ch = fatherNode->getM2Ch();
	CNode<Point_2D>* fathersRightCh = fatherNode->getRCh();

	if (this == fathersRightCh) return nullptr;
	if (this == fathersM2Ch) return fathersRightCh;
	if (this == fathersM1Ch) {
		if (fathersM2Ch != nullptr) return fathersM2Ch;
		else return fathersRightCh;
	}
	if (fathersM1Ch != nullptr) return fathersM1Ch;
	else return fathersRightCh;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getFather()
{
	return this->Father;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getChainLeft()
{
	return this->chainLeft;
}

template<typename Point_2D>
inline CNode<Point_2D>* CNode<Point_2D>::getChainRight()
{
	return this->chainRight;
}

template<typename Point_2D>
inline int CNode<Point_2D>::GetNodeSize()
{
	if (this->m || this->MMax)
		return 3;
	if (this->r || this->RMax)
		return 2;
	else
		return 1;
}

template<typename Point_2D>
inline int CNode<Point_2D>::GetNumCh()
{
	int count = 0;
	if (this->getLCh()) {
		++count;
	}
	if (this->getM1Ch()) {
		++count;
	}
	if (this->getM2Ch()) {
		++count;
	}
	if (this->getRCh()) {
		++count;
	}
	return count;
}

template<typename Point_2D>
inline std::vector<Point_2D> CNode<Point_2D>::getKeyVals()
{
	std::vector<Point_2D> out;
	out.push_back(*this->getLMax()->getMaxVal());
	if (this->getMMax()) {
		out.push_back(*this->getMMax()->getMaxVal());
	}
	if (this->getRMax()) {
		out.push_back(*this->getRMax()->getMaxVal());
	};
	return out;
}

template<typename Point_2D>
inline std::vector<Point_2D> CNode<Point_2D>::getAllVals()
{
	std::vector<Point_2D> out;
	out.push_back(*this->getLVal());
	if (this->getMVal()) out.push_back(*this->getMVal());
	if (this->getRVal()) out.push_back(*this->getRVal());
	return out;
}

template<typename Point_2D>
inline std::vector<CNode<Point_2D>*> CNode<Point_2D>::getAllCh()
{
	std::vector<CNode<Point_2D>*> out;
	if (this->getLCh()) out.push_back(this->getLCh());
	if (this->getM1Ch()) out.push_back(this->getM1Ch());
	if (this->getM2Ch()) out.push_back(this->getM2Ch());
	if (this->getRCh()) out.push_back(this->getRCh());
	return out;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetLHull(SubHull<Point_2D>* hull)
{
	this->LHull = hull;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetLLeftIntermediateHull(SubHull<Point_2D>* hull)
{
	this->LLeftIntermediateHull = hull;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetLRightIntermediateHull(SubHull<Point_2D>* hull)
{
	this->LRightIntermediateHull = hull;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetRHull(SubHull<Point_2D>* hull)
{
	this->RHull = hull;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetRLeftIntermediateHull(SubHull<Point_2D>* hull)
{
	this->RLeftIntermediateHull = hull;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetRRightIntermediateHull(SubHull<Point_2D>* hull)
{
	this->RRightIntermediateHull = hull;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetLBridge(Point_2D* b)
{
	this->LBridge = b;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetLLeftBridge(Point_2D* b)
{
	this->LLeftBridge = b;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetLRightBridge(Point_2D* b)
{
	this->LRightBridge = b;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetRBridge(Point_2D* b)
{
	this->RBridge = b;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetRLeftBridge(Point_2D* b)
{
	this->RLeftBridge = b;
}

template<typename Point_2D>
inline void CNode<Point_2D>::SetRRightBridge(Point_2D* b)
{
	this->RRightBridge = b;
}

template<typename Point_2D>
inline void CNode<Point_2D>::DownReset()
{
	this->SetLHull(nullptr);
	this->SetLLeftIntermediateHull(nullptr);
	this->SetLRightIntermediateHull(nullptr);

	this->SetLBridge(nullptr);
	this->SetLLeftBridge(nullptr);
	this->SetLRightBridge(nullptr);
	
	this->SetRHull(nullptr);
	this->SetRLeftIntermediateHull(nullptr);
	this->SetRRightIntermediateHull(nullptr);

	this->SetRBridge(nullptr);
	this->SetRLeftBridge(nullptr);
	this->SetRRightBridge(nullptr);
}

template<typename Point_2D>
inline void CNode<Point_2D>::setLMax(CNode* cn)
{
	this->LMax = cn;
}

template<typename Point_2D>
inline void CNode<Point_2D>::setMMax(CNode* cn)
{
	this->MMax = cn;
}

template<typename Point_2D>
inline void CNode<Point_2D>::setRMax(CNode* cn)
{
	this->RMax = cn;
}

template<typename Point_2D>
inline void CNode<Point_2D>::setLCh(CNode* lch)
{
	this->LCh = lch;
	if (lch) lch->setFather(this);
}

template<typename Point_2D>
inline void CNode<Point_2D>::setM1Ch(CNode* m1)
{
	this->M1Ch = m1;
	if (m1) m1->setFather(this);
}

template<typename Point_2D>
inline void CNode<Point_2D>::setM2Ch(CNode* m2)
{
	this->M2Ch = m2;
	if (m2) m2->setFather(this);
}

template<typename Point_2D>
inline void CNode<Point_2D>::setRCh(CNode* r)
{
	this->RCh = r;
	if (r) r->setFather(this);
}

template<typename Point_2D>
inline void CNode<Point_2D>::setFather(CNode* f)
{
	this->Father = f;
}

template<typename Point_2D>
inline void CNode<Point_2D>::setChainLeft(CNode<Point_2D>* l)
{
	this->chainLeft = l;
	if (l) l->setChainRight(this);
}

template<typename Point_2D>
inline void CNode<Point_2D>::setChainRight(CNode<Point_2D>* r)
{
	this->chainRight = r;
}

template<typename Point_2D>
inline void CNode<Point_2D>::CalculateLeafLSubHull()
{
	if (this->GetLHull()) delete this->GetLHull();
	Point_2D* l = this->getLVal();
	Point_2D* m = this->getMVal();
	Point_2D* r = this->getRVal();
	SubHull<Point_2D>* Output = new SubHull<Point_2D>(l);

	if (this->GetNodeSize() == 2) {
		if (r && r->GetY() != l->GetY())
			Output->Insert(r);
	}
	else if (this->GetNodeSize() == 3) {
		Output->Insert(r);

		// Misleading Name, ==> Orientation of 3 ordered points is better.
		double cross = (m->GetX() - l->GetX()) * (m->GetY() - r->GetY()) - (m->GetY() - l->GetY()) * (m->GetX() - r->GetX());
		
		if (cross > 0) {
			Output->TTree<Point_2D>::Insert(m);
		}
		if (r->GetY() == m->GetY() || r->GetY() == l->GetY())
			Output->Delete(*r);
	}
	SetLHull(Output);
}

template<typename Point_2D>
inline void CNode<Point_2D>::CalculateLeafRSubHull()
{
	if (this->GetRHull()) 
		delete this->GetRHull();
	
	Point_2D* l = this->getLVal();
	Point_2D* m = this->getMVal();
	Point_2D* r = this->getRVal();
	
	SubHull<Point_2D>* Output = new SubHull<Point_2D>(l);

	if (this->GetNodeSize() == 2) {
		if (r && r->GetY() != l->GetY())
			Output->Insert(r);
	}
	else if (this->GetNodeSize() == 3) {
		Output->Insert(r);

		// Misleading Name, ==> Orientation of 3 ordered points is better.
		double cross = (m->GetX() - l->GetX()) * (m->GetY() - r->GetY()) - 
			(m->GetY() - l->GetY()) * (m->GetX() - r->GetX());

		if (cross < 0) {
			Output->TTree<Point_2D>::Insert(m);
		}

		if (r->GetY() == m->GetY() || r->GetY() == l->GetY())
			Output->Delete(*r);
	}
	SetRHull(Output);
}

template<typename Point_2D>
inline void CNode<Point_2D>::clear()
{
	this->LMax = nullptr;
	this->MMax = nullptr;
	this->RMax = nullptr;

	this->LCh = nullptr;
	this->M1Ch = nullptr;
	this->M2Ch = nullptr;
	this->RCh = nullptr;

	this->LBridge = nullptr;
	this->LLeftBridge = nullptr;
	this->LRightBridge = nullptr;

	this->RBridge = nullptr;
	this->RLeftBridge = nullptr;
	this->RRightBridge = nullptr;
}

template<typename Point_2D>
inline void CNode<Point_2D>::deletePoints()
{
	delete this->l;

	if (this->r)
		delete this->r;
	if (this->m)
		delete this->m;

}

template<typename Point_2D>
inline bool CNode<Point_2D>::isValidNode() {

	// this function does not check continuity between neighboring nodes or father-children continuities.

	SubHull<Point_2D>* LH = this->GetLHull();
	SubHull<Point_2D>* LLH = this->GetLLeftIntermediateHull();
	SubHull<Point_2D>* LRH = this->GetLRightIntermediateHull();
	
	SubHull<Point_2D>* RH = this->GetRHull();
	SubHull<Point_2D>* RLH = this->GetRLeftIntermediateHull();
	SubHull<Point_2D>* RRH = this->GetRRightIntermediateHull();

	CNode<Point_2D>* father = this->getFather();
	CNode<Point_2D>* lSib = this->getLSibling();
	CNode<Point_2D>* rSib = this->getRSibling();

	if (LH)
		LH->isValidTree();
	if (LLH) 
		LLH->isValidTree();
	if (LRH) 
		LRH->isValidTree();

	if (RH)
		RH->isValidTree();
	if (RLH) 
		RLH->isValidTree();
	if (RRH) 
		RRH->isValidTree();

	if (this->getIsLeaf()) {
		if (!this->getLVal()) {
			__debugbreak();
			return false;
		}

		if (this->getLVal() == this->getMVal() || this->getLVal() == this->getRVal()) {
			__debugbreak();
			return false;
		}
		if (this->getMVal() && this->getMVal() == this->getRVal()) {
			__debugbreak();
			return false;
		}

		if (this->getMVal() && !this->getRVal()) {
			__debugbreak();
			return false;
		}

	}
	else {
		if (!this->getLMax()) {
			__debugbreak();
			return false;
		}

		if (this->getLMax() == this->getMMax() || this->getLMax() == this->getRMax() ||
			(this->getMMax() && this->getMMax() == this->getRMax())) {
			__debugbreak();
			return false;
		}

		if (this->getMMax() && !this->getRMax()) {
			__debugbreak();
			return false;
		}

		int NodeSize = this->GetNodeSize();
		int NumChildren = this->GetNumCh();

		if (NodeSize == 2) {
			if (*this->getLMax()->getMaxVal() > *this->getRMax()->getMaxVal()) {
				__debugbreak();
				return false;
			}
			if (NumChildren != 3) {
				__debugbreak();
				return false;
			}
		}
		else if (NodeSize == 3) {
			if (*this->getLMax()->getMaxVal() > *this->getMMax()->getMaxVal() ||
				*this->getLMax()->getMaxVal() > *this->getRMax()->getMaxVal() ||
				*this->getMMax()->getMaxVal() > *this->getRMax()->getMaxVal()) {
				__debugbreak();
				return false;
			}

			if (NumChildren != 4) {
				__debugbreak();
				return false;
			}
		}
	}
	return true;
}