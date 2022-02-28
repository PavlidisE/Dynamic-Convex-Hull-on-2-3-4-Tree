#pragma once
#include "CQueue.h"
#include "Point_2D.h"


template <typename Point_2D>
class SubHull : public CQueue<Point_2D>
{
	/**
	* @author Evripidis Pavlidis
	* @since 2020-5-12
	*/

public:
	SubHull();
	SubHull(Point_2D* p);
	SubHull(CQueue<Point_2D>* q);

	static SubHull<Point_2D>* Bridge(SubHull<Point_2D>*, SubHull<Point_2D>*, Point_2D**, bool);

	static std::pair<Node<Point_2D>*, Node<Point_2D>*> getDirections(Node<Point_2D>*, Node<Point_2D>*, int, double, bool);

	static std::pair<int, int> getDirectionsHelp(Node<Point_2D>*, Node<Point_2D>*, double, bool);
	
	static std::pair<Node<Point_2D>*, Node<Point_2D>*> getRightDirectionHelp(Node<Point_2D>*, Node<Point_2D>*, double, Node<Point_2D>*, Node<Point_2D>*, bool);
	
	static int determineCaseLowerLeft(Node<Point_2D>*, Point_2D);
	static int determineCaseUpperLeft(Node<Point_2D>*, Point_2D);

	static int determineCaseLowerRight(Node<Point_2D>*, Point_2D);
	static int determineCaseUpperRight(Node<Point_2D>*, Point_2D);

	static int getCase(int, int);
	static int determineConcaveConcave(Node<Point_2D>*, Node<Point_2D>*, double);
	
	static Point_2D* detSuppPointLowerLeft(Node<Point_2D>*, Point_2D);
	static Point_2D* detSuppPointUpperLeft(Node<Point_2D>*, Point_2D);
	
	static Point_2D* detSuppPointUpperRight(Node<Point_2D>*, Point_2D);
	static Point_2D* detSuppPointLowerRight(Node<Point_2D>*, Point_2D);


	static std::pair<Point_2D*, Point_2D*> determineBridgePointsLeft(Node<Point_2D>*, Node<Point_2D>*);
	static std::pair<Point_2D*, Point_2D*> determineBridgePointsRight(Node<Point_2D>*, Node<Point_2D>*);
	
	
	static double computeSlope(Point_2D, Point_2D);
	static double crossProduct(Point_2D, Point_2D, Point_2D);

};

template <typename Point_2D>
inline SubHull<Point_2D>::SubHull() : CQueue<Point_2D>() {}

template <typename Point_2D>
inline SubHull<Point_2D>::SubHull(Point_2D* p) : CQueue<Point_2D>(p) {}

template <typename Point_2D>
inline SubHull<Point_2D>::SubHull(CQueue<Point_2D>* q) : CQueue<Point_2D>(q->GetRoot(), q->GetHeight(), q->GetMinNode(), q->GetMaxNode()) {}

template<typename Point_2D>
inline SubHull<Point_2D>* SubHull<Point_2D>::Bridge(SubHull* l_hull, SubHull* u_hull, Point_2D** BridgeP, bool hullFlag)
{
	Node<Point_2D>* l_itr = l_hull->GetRoot();
	Node<Point_2D>* u_itr = u_hull->GetRoot();

	int iL, iU;
	double middleY = (l_hull->GetMaxNode()->getMaxVal()->GetY() + u_hull->GetMaxNode()->getMaxVal()->GetY()) / 2;
	bool done = false;

	while (!done) {
		if (l_itr->getIsLeaf() && u_itr->getIsLeaf()) break;
		if (l_itr->getIsLeaf()) {
			iL = 0;

			if (!hullFlag) {
				iU = determineCaseUpperLeft(u_itr->getLMax(), *detSuppPointLowerLeft(l_itr, *u_itr->getLMax()->getMaxVal()));
			}
			else {
				iU = determineCaseUpperRight(u_itr->getLMax(), *detSuppPointLowerRight(l_itr, *u_itr->getLMax()->getMaxVal()));
			}
		}
		else {
			if (u_itr->getIsLeaf()) {
				iU = 0;

				if (!hullFlag) {
					iL = determineCaseLowerLeft(l_itr->getLMax(), *detSuppPointUpperLeft(u_itr, *l_itr->getLMax()->getMaxVal()));
				}
				else {
					iL = determineCaseLowerRight(l_itr->getLMax(), *detSuppPointUpperRight(u_itr, *l_itr->getLMax()->getMaxVal()));
				}
			}
			else {

				if (!hullFlag) {
					iL = determineCaseLowerLeft(l_itr->getLMax(), *u_itr->getLMax()->getMaxVal());
					iU = determineCaseUpperLeft(u_itr->getLMax(), *l_itr->getLMax()->getMaxVal());
				}
				else {
					iL = determineCaseLowerRight(l_itr->getLMax(), *u_itr->getLMax()->getMaxVal());
					iU = determineCaseUpperRight(u_itr->getLMax(), *l_itr->getLMax()->getMaxVal());
				}
			}
		}

		// bool flag end == 0 left bridge
		std::pair<Node<Point_2D>*, Node<Point_2D>*> children = getDirections(l_itr, u_itr, getCase(iL, iU), middleY, hullFlag);

		if (children.first)
			l_itr = children.first;
		if (children.second)
			u_itr = children.second;
	}

	std::pair<Point_2D*, Point_2D*> bridges;

	if (!hullFlag) {
		bridges = determineBridgePointsLeft(l_itr, u_itr);
	}
	else {
		bridges = determineBridgePointsRight(l_itr, u_itr);
	}

	Point_2D* splitLeft = bridges.first;
	Point_2D* splitRight = bridges.second;

	*BridgeP = splitLeft;

	CQueue<Point_2D>* BaseOut = CQueue<Point_2D>::Concatenate(l_hull->Split(*splitLeft, true, true), u_hull->Split(*splitRight, false, false));	
	SubHull<Point_2D>* Output = new SubHull<Point_2D>(BaseOut);

	/* Tackle this tedious procedure later by implementing a constructor using the already existing BaseOut and not creating a new cqueue based on that. */
	BaseOut->Clear();
	delete BaseOut;

	return Output;

}

template<typename Point_2D>
inline std::pair<Node<Point_2D>*, Node<Point_2D>*> SubHull<Point_2D>::getDirections(Node<Point_2D>* LowerNode, Node<Point_2D>* UpperNode, int baseCase, double midY, bool hullFlag)
{
	Node<Point_2D>* lChild = nullptr, * uChild = nullptr;
	Point_2D* suppPoint = nullptr;
	int LowerNodeSize = LowerNode->getNumVal(), UpperNodeSize = UpperNode->getNumVal();
	int helpFlag;
	std::pair<Node<Point_2D>*, Node<Point_2D>*> helpChildren;

	if (baseCase == 1) {
		helpFlag = determineConcaveConcave(LowerNode->getLMax(), UpperNode->getLMax(), midY);
		if (helpFlag) {
			uChild = UpperNode->getLCh();
		}
		else {
			if (LowerNodeSize == 1) {
				lChild = LowerNode->getRCh();
			}
			else if (LowerNodeSize == 2) {
				helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getRMax(), UpperNode->getLMax(), hullFlag);
				lChild = helpChildren.first;
				uChild = helpChildren.second;
			}
			else {
				helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getMMax(), UpperNode->getLMax(), hullFlag);
				lChild = helpChildren.first;
				uChild = helpChildren.second;
			}
		}
	}
	else if (baseCase == 2) {
		if (UpperNode->getIsLeaf()) {
			if (LowerNodeSize == 1) {
				lChild = LowerNode->getRCh();
			}
			else {
				// Find Supporting Point.

				if (!hullFlag) {
					suppPoint = detSuppPointUpperLeft(UpperNode, *LowerNode->getLMax()->getMaxVal());
				}
				else {
					suppPoint = detSuppPointUpperLeft(UpperNode, *LowerNode->getLMax()->getMaxVal());
				}
				// Perform Checks with that Point.
				// Possible cases either 2 or 8 or 5!!! 

				if (LowerNodeSize == 2) {
					if (!hullFlag) {
						helpFlag = getCase(determineCaseLowerLeft(LowerNode->getRMax(), *suppPoint), 0);
					}
					else {
						helpFlag = getCase(determineCaseLowerRight(LowerNode->getRMax(), *suppPoint), 0);
					}

					if (helpFlag == 2) {
						lChild = LowerNode->getRCh();
					}
					else if (helpFlag == 8) {
						lChild = LowerNode->getM1Ch();
					}
					else {
						lChild = LowerNode->getRMax();
					}
				}
				else {
					if (!hullFlag) {
						helpFlag = getCase(determineCaseLowerLeft(LowerNode->getMMax(), *suppPoint), 0);
					}
					else {
						helpFlag = getCase(determineCaseLowerRight(LowerNode->getMMax(), *suppPoint), 0);
					}


					if (helpFlag == 2) {
						if (!hullFlag) {
							helpFlag = getCase(determineCaseLowerLeft(LowerNode->getRMax(), *detSuppPointLowerLeft(UpperNode, *LowerNode->getRMax()->getMaxVal())), 0);
						}
						else {
							helpFlag = getCase(determineCaseLowerRight(LowerNode->getRMax(), *detSuppPointLowerRight(UpperNode, *LowerNode->getRMax()->getMaxVal())), 0);
						}
						
						if (helpFlag == 2) {
							lChild = LowerNode->getRCh();
						}
						else if (helpFlag == 8) {
							lChild = LowerNode->getM2Ch();
						}
						else {
							lChild = LowerNode->getRMax();
						}
					}
					else if (helpFlag == 8) {
						lChild = LowerNode->getM1Ch();
					}
					else {
						lChild = LowerNode->getMMax();
					}
				}
			}
		}
		else {
			if (LowerNodeSize == 1) {
				lChild = LowerNode->getRCh();
			}
			else if (LowerNodeSize == 2) {
				helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getRMax(), UpperNode->getLMax(), hullFlag);
				lChild = helpChildren.first;
				uChild = helpChildren.second;
			}
			else {
				helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getMMax(), UpperNode->getLMax(), hullFlag);
				lChild = helpChildren.first;
				uChild = helpChildren.second;
			}
		}
	}
	else if (baseCase == 3) {
		if (UpperNodeSize == 1) {
			uChild = UpperNode->getRCh();
		}
		else if (UpperNodeSize == 2) {
			helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getLMax(), UpperNode->getRMax(), hullFlag);
			lChild = helpChildren.first;
			uChild = helpChildren.second;
		}
		else {
			helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getLMax(), UpperNode->getMMax(), hullFlag);
			lChild = helpChildren.first;
			uChild = helpChildren.second;
		}
	}
	else if (baseCase == 4) {
		if (!LowerNode->getIsLeaf()) {
			lChild = LowerNode->getLCh();
		}
		uChild = UpperNode->getLCh();
	}
	else if (baseCase == 5) {
		lChild = LowerNode->getLMax();
		uChild = UpperNode->getLMax();
	}
	else if (baseCase == 6) {
		if (LowerNode->getIsLeaf()) {
			if (UpperNodeSize == 1) {
				uChild = UpperNode->getRCh();
			}
			else {
				if (!hullFlag) {
					suppPoint = detSuppPointLowerLeft(LowerNode, *UpperNode->getLMax()->getMaxVal());
				}
				else {
					suppPoint = detSuppPointLowerRight(LowerNode, *UpperNode->getLMax()->getMaxVal());
				}
				
				// possible cases 4, 5, 6
				if (UpperNodeSize == 2) {
					if (!hullFlag) {
						helpFlag = getCase(0, determineCaseUpperLeft(UpperNode->getRMax(), *suppPoint));
					}
					else {
						helpFlag = getCase(0, determineCaseUpperRight(UpperNode->getRMax(), *suppPoint));
					}
					
					if (helpFlag == 4) {
						uChild = UpperNode->getM1Ch();
					}
					else if (helpFlag == 5) {
						uChild = UpperNode->getRMax();
					}
					else {
						uChild = UpperNode->getRCh();
					}
				}
				else {
					if (!hullFlag) {
						helpFlag = getCase(0, determineCaseUpperLeft(UpperNode->getMMax(), *suppPoint));
					}else{
						helpFlag = getCase(0, determineCaseUpperRight(UpperNode->getMMax(), *suppPoint));
					}
					
					if (helpFlag == 4) {
						uChild = UpperNode->getM1Ch();
					}
					else if (helpFlag == 5) {
						uChild = UpperNode->getRMax();
					}
					else {
						if (!hullFlag) {
							helpFlag = getCase(0, determineCaseUpperLeft(UpperNode->getRMax(), *detSuppPointLowerLeft(LowerNode, *UpperNode->getRMax()->getMaxVal())));
						}
						else {
							helpFlag = getCase(0, determineCaseUpperRight(UpperNode->getRMax(), *detSuppPointLowerRight(LowerNode, *UpperNode->getRMax()->getMaxVal())));
						}
						
						if (helpFlag == 4) {
							uChild = UpperNode->getM2Ch();
						}
						else if (helpFlag == 5) {
							uChild = UpperNode->getRMax();
						}
						else {
							uChild = UpperNode->getRCh();
						}
					}
				}
			}
		}
		else {
			if (UpperNodeSize == 1) {
				uChild = UpperNode->getRCh();
				lChild = LowerNode->getLCh();
			}
			else if (UpperNodeSize == 2) {
				helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getLMax(), UpperNode->getRMax(), hullFlag);
				lChild = helpChildren.first;
				uChild = helpChildren.second;
			}
			else {
				helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getLMax(), UpperNode->getMMax(), hullFlag);
				lChild = helpChildren.first;
				uChild = helpChildren.second;
			}
		}
	}
	else if (baseCase == 7) {
		lChild = LowerNode->getLCh();
	}
	else if (baseCase == 8) {
		lChild = LowerNode->getLCh();
	}
	else {
		if (UpperNodeSize == 1) {
			lChild = LowerNode->getLCh();
			uChild = UpperNode->getRCh();
		}
		else if (UpperNodeSize == 2) {
			helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getLMax(), UpperNode->getRMax(), hullFlag);
			lChild = helpChildren.first;
			uChild = helpChildren.second;
		}
		else {
			helpChildren = getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getLMax(), UpperNode->getMMax(), hullFlag);
			lChild = helpChildren.first;
			uChild = helpChildren.second;
		}
	}

	return std::make_pair(lChild, uChild);
}

template<typename Point_2D>
inline std::pair<int, int> SubHull<Point_2D>::getDirectionsHelp(Node<Point_2D>* LowerNode, Node<Point_2D>* UpperNode, double midY, bool hullFlag)
{
	// LEFT  : -1
	// UNDEF :  0
	// RIGHT : +1
	// HERE  : +2
	int UpperCase, LowerCase, Case, helpFlag, LowerDir, UpperDir;
	if (!hullFlag) {
		LowerCase = determineCaseLowerLeft(LowerNode, *UpperNode->getMaxVal());
		UpperCase = determineCaseUpperLeft(UpperNode, *LowerNode->getMaxVal());
	}
	else {
		LowerCase = determineCaseLowerRight(LowerNode, *UpperNode->getMaxVal());
		UpperCase = determineCaseUpperRight(UpperNode, *LowerNode->getMaxVal());
	}

	Case = getCase(LowerCase, UpperCase);

	if (Case == 1) {
		helpFlag = determineConcaveConcave(LowerNode, UpperNode, midY);
		if (helpFlag) {
			LowerDir = 0;
			UpperDir = -1;
		}
		else {
			LowerDir = 1;
			UpperDir = 0;
		}
	}
	else if (Case == 2) {
		LowerDir = 1;
		UpperDir = 0;
	}
	else if (Case == 3) {
		LowerDir = 0;
		UpperDir = 1;
	}
	else if (Case == 4) {
		LowerDir = -1;
		UpperDir = -1;
	}
	else if (Case == 5) {
		LowerDir = 2;
		UpperDir = 2;
	}
	else if (Case == 6) {
		LowerDir = -1;
		UpperDir = 1;
	}
	else if (Case == 7) {
		LowerDir = -1;
		UpperDir = 0;
	}
	else if (Case == 8) {
		LowerDir = -1;
		UpperDir = 0;
	}
	else {
		LowerDir = -1;
		UpperDir = 1;
	}

	return std::pair<int, int>(LowerDir, UpperDir);
}

template<typename Point_2D>
inline std::pair<Node<Point_2D>*, Node<Point_2D>*> SubHull<Point_2D>::getRightDirectionHelp(Node<Point_2D>* LowerNode, Node<Point_2D>* UpperNode, double midY, Node<Point_2D>* LowerSegFlag, Node<Point_2D>* UpperSegFlag, bool hullFlag)
{
	// only use this method when we are checking rightside search in internal node, to determine child.
	int LNodeSize = LowerNode->getNumVal();
	int UNodeSize = UpperNode->getNumVal();

	Node<Point_2D>* LowerChild = nullptr;
	Node<Point_2D>* UpperChild = nullptr;

	std::pair<int, int> Dirs = getDirectionsHelp(LowerSegFlag, UpperSegFlag, midY, hullFlag);

	if (Dirs.first == -1) {
		if (Dirs.second == -1) {
			if (LowerSegFlag == LowerNode->getLMax()) {
				LowerChild = LowerNode->getLCh();
			}
			else if (LowerSegFlag == LowerNode->getMMax()) {
				LowerChild = LowerNode->getM1Ch();
			}
			else {
				if (LNodeSize == 2) {
					LowerChild = LowerNode->getM1Ch();
				}
				else {
					LowerChild = LowerNode->getM2Ch();
				}
			}

			if (UpperSegFlag == UpperNode->getLMax()) {
				UpperChild = UpperNode->getLCh();
			}
			else if (UpperSegFlag == UpperNode->getMMax()) {
				UpperChild = UpperNode->getM1Ch();
			}
			else {
				if (UNodeSize == 2) {
					UpperChild = UpperNode->getM1Ch();
				}
				else {
					UpperChild = UpperNode->getM2Ch();
				}
			}

		}
		else if (Dirs.second == 0) {
			if (LowerSegFlag == LowerNode->getLMax()) {
				LowerChild = LowerNode->getLCh();
			}
			else if (LowerSegFlag == LowerNode->getMMax()) {
				LowerChild = LowerNode->getM1Ch();
			}
			else {
				if (LNodeSize == 2) {
					LowerChild = LowerNode->getM1Ch();
				}
				else {
					LowerChild = LowerNode->getM2Ch();
				}
			}
		}
		else {
			if (UNodeSize == 1) {
				if (LowerSegFlag == LowerNode->getLMax()) {
					LowerChild = LowerNode->getLCh();
				}
				else if (LowerSegFlag == LowerNode->getMMax()) {
					LowerChild = LowerNode->getM1Ch();
				}
				else {
					if (LNodeSize == 2) {
						LowerChild = LowerNode->getM1Ch();
					}
					else {
						LowerChild = LowerNode->getM2Ch();
					}
				}

				UpperChild = UpperNode->getRCh();
			}
			else {
				if (UpperSegFlag == UpperNode->getRMax()) {
					if (LowerSegFlag == LowerNode->getLMax()) {
						LowerChild = LowerNode->getLCh();
					}
					else if (LowerSegFlag == LowerNode->getMMax()) {
						LowerChild = LowerNode->getM1Ch();
					}
					else {
						if (LNodeSize == 2) {
							LowerChild = LowerNode->getM1Ch();
						}
						else {
							LowerChild = LowerNode->getM2Ch();
						}
					}

					UpperChild = UpperNode->getRCh();
				}
				else {
					if (UNodeSize == 2) {
						return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerSegFlag, UpperNode->getRMax(), hullFlag);
					}
					else {
						if (UpperSegFlag == UpperNode->getLMax()) {
							return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerSegFlag, UpperNode->getMMax(), hullFlag);
						}
						else {
							return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerSegFlag, UpperNode->getRMax(), hullFlag);
						}
					}
				}
			}
		}
	}
	else if (Dirs.first == 0) {
		if (Dirs.second == -1) {
			if (UpperSegFlag == UpperNode->getLMax()) {
				UpperChild = UpperNode->getLCh();
			}
			else if (UpperSegFlag == UpperNode->getMMax()) {
				UpperChild = UpperNode->getM1Ch();
			}
			else {
				if (UNodeSize == 2) {
					UpperChild = UpperNode->getM1Ch();
				}
				else {
					UpperChild = UpperNode->getM2Ch();
				}
			}
		}
		else {
			if (UNodeSize == 1 || UpperSegFlag == UpperNode->getRMax()) {
				UpperChild = UpperNode->getRCh();
			}
			else {
				if (UNodeSize == 2) {
					return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerSegFlag, UpperNode->getRMax(), hullFlag);
				}
				else {
					if (UpperSegFlag == UpperNode->getLMax()) {
						return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerSegFlag, UpperNode->getMMax(), hullFlag);
					}
					else {
						return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerSegFlag, UpperNode->getRMax(), hullFlag);
					}
				}
			}
		}
	}
	else if (Dirs.first == 1) {
		if (LNodeSize == 1 || LowerSegFlag == LowerNode->getRMax()) {
			LowerChild = LowerNode->getRCh();
		}
		else {
			if (LNodeSize == 2) {
				return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getRMax(), UpperSegFlag, hullFlag);
			}
			else {
				if (LowerSegFlag == LowerNode->getLMax()) {
					return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getMMax(), UpperSegFlag, hullFlag);
				}
				else {
					return getRightDirectionHelp(LowerNode, UpperNode, midY, LowerNode->getRMax(), UpperSegFlag, hullFlag);
				}
			}
		}
	}
	else {
		return std::pair<Node<Point_2D>*, Node<Point_2D>*>(LowerSegFlag, UpperSegFlag);
	}

	return std::pair<Node<Point_2D>*, Node<Point_2D>*>(LowerChild, UpperChild);
}

template<typename Point_2D>
inline int SubHull<Point_2D>::determineCaseLowerLeft(Node<Point_2D>* n, Point_2D referencePoint)
{
	bool above = false;
	bool below = false;

	int NodeSize = n->getNumVal();
	Node<Point_2D>* left = n->getChainLeft();
	Node<Point_2D>* right = n->getChainRight();

	if (right && crossProduct(*n->getMaxVal(), *right->getLVal(), referencePoint) > 0) {
		above = true;
	}

	if (NodeSize == 1) {
		if (left && crossProduct(*left->getMaxVal(), referencePoint, *n->getLVal()) > 0) {
			below = true;
		}
	}
	else if (NodeSize == 2) {
		if (crossProduct(*n->getLVal(), referencePoint, *n->getRVal()) > 0) {
			below = true;
		}
	}
	else {
		if (crossProduct(*n->getMVal(), referencePoint, *n->getRVal()) > 0) {
			below = true;
		}
	}

	if (above && !below) {
		// concave for l_hull, reflex for r_hull
		return -1;
	}
	else if (!above && !below) {
		return 0;
	}
	else {
		// only case left is !Above && Below
		// reflex for l_hull, concave for r_hull
		return +1;
	}
}

template<typename Point_2D>
inline int SubHull<Point_2D>::determineCaseUpperLeft(Node<Point_2D>* n, Point_2D referencePoint)
{
	bool above = false;
	bool below = false;

	int NodeSize = n->getNumVal();
	Node<Point_2D>* left = n->getChainLeft();
	Node<Point_2D>* right = n->getChainRight();

	if (right && crossProduct(referencePoint, *n->getMaxVal(), *right->getLVal()) < 0) {
		above = true;
	}

	if (NodeSize == 1) {
		if (left && crossProduct(referencePoint, *left->getMaxVal(), *n->getLVal()) > 0) {
			below = true;
		}
	}
	else if (NodeSize == 2) {
		if (crossProduct(referencePoint, *n->getLVal(), *n->getRVal()) > 0) {
			below = true;
		}
	}
	else {
		if (crossProduct(referencePoint, *n->getMVal(), *n->getRVal()) > 0) {
			below = true;
		}
	}

	if (above && !below) {
		// concave for l_hull, reflex for r_hull
		return -1;
	}
	else if (!above && !below) {
		return 0;
	}
	else {
		// only case left is !Above && Below
		// reflex for l_hull, concave for r_hull
		return +1;
	}
}

template<typename Point_2D>
inline int SubHull<Point_2D>::determineCaseLowerRight(Node<Point_2D>* n, Point_2D referencePoint)
{
	bool above = false;
	bool below = false;

	int NodeSize = n->getNumVal();
	Node<Point_2D>* left = n->getChainLeft();
	Node<Point_2D>* right = n->getChainRight();

	if (right && crossProduct(referencePoint, *right->getLVal(), *n->getMaxVal()) > 0) {
		above = true;
	}

	if (NodeSize == 1) {
		if (left && crossProduct(referencePoint, *n->getMaxVal(), *left->getMaxVal()) < 0) {
			below = true;
		}
	}
	else if (NodeSize == 2) {
		if (crossProduct(referencePoint, *n->getRVal(), *n->getLVal()) < 0) {
			below = true;
		}
	}
	else {
		if (crossProduct(referencePoint, *n->getRVal(), *n->getMVal()) < 0) {
			below = true;
		}
	}

	if (above && !below) {
		// concave for l_hull, reflex for r_hull
		return -1;
	}
	else if (!above && !below) {
		return 0;
	}
	else {
		// only case left is !Above && Below
		// reflex for l_hull, concave for r_hull
		return +1;
	}
}

template<typename Point_2D>
inline int SubHull<Point_2D>::determineCaseUpperRight(Node<Point_2D>* n, Point_2D referencePoint)
{
	bool above = false;
	bool below = false;

	int NodeSize = n->getNumVal();
	Node<Point_2D>* left = n->getChainLeft();
	Node<Point_2D>* right = n->getChainRight();

	if (right && crossProduct(*right->getLVal(), *n->getMaxVal(), referencePoint) < 0) {
		above = true;
	}

	if (NodeSize == 1) {
		if (left && crossProduct(*n->getLVal(), *left->getMaxVal(), referencePoint) > 0) {
			below = true;
		}
	}
	else if (NodeSize == 2) {
		if (crossProduct(*n->getRVal(), *n->getLVal(), referencePoint) > 0) {
			below = true;
		}
	}
	else {
		if (crossProduct(*n->getRVal(), *n->getMVal(), referencePoint) > 0) {
			below = true;
		}
	}

	if (above && !below) {
		// concave for l_hull, reflex for r_hull
		return -1;
	}
	else if (!above && !below) {
		return 0;
	}
	else {
		// only case left is !Above && Below
		// reflex for l_hull, concave for r_hull
		return +1;
	}
}

template<typename Point_2D>
inline int SubHull<Point_2D>::getCase(int lower_case, int upper_case) {
	switch (lower_case) {
	case -1:
		switch (upper_case) {
		case -1:
			return 3;
			break;
		case 0:
			return 2;
			break;
		case +1:
			return 1;
			break;
		}
		break;
	case 0:
		switch (upper_case) {
		case -1:
			return 6;
			break;
		case 0:
			return 5;
			break;
		case +1:
			return 4;
			break;
		}
		break;
	case +1:
		switch (upper_case) {
		case -1:
			return 9;
			break;
		case 0:
			return 8;
			break;
		case +1:
			return 7;
			break;
		}
		break;
	}
	return -100;
}

template<typename Point_2D>
inline int SubHull<Point_2D>::determineConcaveConcave(Node<Point_2D>* lower, Node<Point_2D>* upper, double middleY) {

	/* input ex.
	* r_itr->getLMax
	* l_itr->getRMax
	*
	* returns +1 for search in rhull
	* returns -1 for search in lhull
	*/

	int u_size = upper->getNumVal();
	int l_size = lower->getNumVal();

	Point_2D upper_p2, upper_p1, lower_p1, lower_p2;

	if (u_size == 1) {
		upper_p2 = *upper->getLVal();
		upper_p1 = *upper->getChainLeft()->getMaxVal();
	}
	else if (u_size == 2) {
		upper_p2 = *upper->getRVal();
		upper_p1 = *upper->getLVal();
	}
	else {
		upper_p2 = *upper->getRVal();
		upper_p1 = *upper->getMVal();
	}

	lower_p1 = *lower->getChainRight()->getLVal();
	if (l_size == 1) {
		lower_p2 = *lower->getLVal();
	}
	else {
		lower_p2 = *lower->getRVal();
	}

	double upperHullSlope = computeSlope(upper_p2, upper_p1);
	double lowerHullSlope = computeSlope(lower_p2, lower_p1);

	double cross_point_y = (lower_p2.GetY() * upperHullSlope - upper_p2.GetY() * lowerHullSlope + lowerHullSlope * upperHullSlope * (upper_p2.GetX() - lower_p2.GetX())) / (upperHullSlope - lowerHullSlope);

	if (cross_point_y <= middleY) {
		return -1;
	}
	else
		return +1;
}

template<typename Point_2D>
inline Point_2D* SubHull<Point_2D>::detSuppPointLowerLeft(Node<Point_2D>* n, Point_2D referencePoint)
{
	int NodeSize = n->getNumVal();
	if (NodeSize == 1) {
		return n->getLVal();
	}
	else if (NodeSize == 2) {
		if (crossProduct(*n->getLVal(), referencePoint, *n->getRVal()) > 0) {
			return n->getLVal();
		}
		else {
			return n->getRVal();
		}
	}
	else {
		if (crossProduct(*n->getMVal(), referencePoint, *n->getRVal()) > 0) {
			if (crossProduct(*n->getLVal(), referencePoint, *n->getMVal()) > 0) {
				return n->getLVal();
			}
			else {
				return n->getMVal();
			}
		}
		else {
			return n->getRVal();
		}
	}
}

template<typename Point_2D>
inline Point_2D* SubHull<Point_2D>::detSuppPointUpperLeft(Node<Point_2D>* n, Point_2D referencePoint)
{
	int NodeSize = n->getNumVal();
	if (NodeSize == 1) {
		return n->getLVal();
	}
	else if (NodeSize == 2) {
		if (crossProduct(referencePoint, *n->getLVal(), *n->getRVal()) > 0) {
			return n->getLVal();
		}
		else {
			return n->getRVal();
		}
	}
	else {
		if (crossProduct(referencePoint, *n->getMVal(), *n->getRVal()) > 0) {
			if (crossProduct(referencePoint, *n->getLVal(), *n->getMVal()) > 0) {
				return n->getLVal();
			}
			else {
				return n->getMVal();
			}
		}
		else {
			return n->getRVal();
		}
	}
}

template<typename Point_2D>
inline Point_2D* SubHull<Point_2D>::detSuppPointUpperRight(Node<Point_2D>* n, Point_2D referencePoint)
{
	int NodeSize = n->getNumVal();
	if (NodeSize == 1) {
		return n->getLVal();
	}
	else if (NodeSize == 2) {
		if (crossProduct(*n->getRVal(), *n->getLVal(), referencePoint) > 0){
			return n->getLVal();
		}
		else {
			return n->getRVal();
		}
	}
	else {
		if (crossProduct(*n->getRVal(), *n->getMVal(), referencePoint) > 0) {
			if (crossProduct(*n->getMVal(), *n->getLVal(), referencePoint) > 0) {
				return n->getLVal();
			}
			else {
				return n->getMVal();
			}
		}
		else {
			return n->getRVal();
		}
	}
}

template<typename Point_2D>
inline Point_2D* SubHull<Point_2D>::detSuppPointLowerRight(Node<Point_2D>* n, Point_2D referencePoint)
{
	int NodeSize = n->getNumVal();
	if (NodeSize == 1) {
		return n->getLVal();
	}
	else if (NodeSize == 2) {
		if (crossProduct(referencePoint, *n->getRVal(), *n->getLVal()) < 0) {
			return n->getLVal();
		}
		else {
			return n->getRVal();
		}
	}
	else {
		if (crossProduct(referencePoint, *n->getRVal(), *n->getMVal()) < 0) {
			if (crossProduct(referencePoint, *n->getMVal(), *n->getLVal()) < 0) {
				return n->getLVal();
			}
			else {
				return n->getMVal();
			}
		}
		else {
			return n->getRVal();
		}
	}
}

template<typename Point_2D>
inline std::pair<Point_2D*, Point_2D*> SubHull<Point_2D>::determineBridgePointsLeft(Node<Point_2D>* lNode, Node<Point_2D>* rNode)
{
	std::vector<Point_2D*> LValues = lNode->getLeafValues();
	std::vector<Point_2D*> RValues = rNode->getLeafValues();

	Point_2D* lBridge = *LValues.rbegin(), * rBridge = *RValues.rbegin();

	for (int i = (int)LValues.size() - 1; i >= 0; --i) {
		lBridge = LValues[i];
		rBridge = detSuppPointUpperLeft(rNode, *lBridge);
		if (detSuppPointLowerLeft(lNode, *rBridge) == lBridge) {
			break;
		}
	}

	return std::make_pair(lBridge, rBridge);
}

template<typename Point_2D>
inline std::pair<Point_2D*, Point_2D*> SubHull<Point_2D>::determineBridgePointsRight(Node<Point_2D>* lNode, Node<Point_2D>* rNode)
{
	std::vector<Point_2D*> LValues = lNode->getLeafValues();
	std::vector<Point_2D*> RValues = rNode->getLeafValues();

	Point_2D* lBridge = *LValues.rbegin(), * rBridge = *RValues.rbegin();

	for (int i = (int)LValues.size() - 1; i >= 0; --i) {
		lBridge = LValues[i];
		rBridge = detSuppPointUpperRight(rNode, *lBridge);
		if (detSuppPointLowerRight(lNode, *rBridge) == lBridge) {
			break;
		}
	}

	return std::make_pair(lBridge, rBridge);
}

template <typename Point_2D>
inline double SubHull<Point_2D>::computeSlope(Point_2D p1, Point_2D p2) {
	/* we should check undefined cases Dx = 0 */
	return (p2.GetY() - p1.GetY()) / (p2.GetX() - p1.GetX());
}

template <typename Point_2D>
inline double SubHull<Point_2D>::crossProduct(Point_2D p1, Point_2D p2, Point_2D p3)
{
	// Misleading Name, ==> Orientation of 3 ordered points is better.
	return (p2.GetY() - p1.GetY()) * (p3.GetX() - p2.GetX()) -
		(p3.GetY() - p2.GetY()) * (p2.GetX() - p1.GetX());
}