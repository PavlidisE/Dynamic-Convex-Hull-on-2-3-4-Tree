#pragma once
#include <vector>
#include <intrin.h>

template<typename T>
class Node
{
	/**
	 * @author Evripidis Pavlidis
	 * @since 2020-5-12
	 */

protected:
	// values 
	T* l = nullptr;
	T* m = nullptr;
	T* r = nullptr;

	// predecessor nodes

	Node* l_max = nullptr;
	Node* m_max = nullptr;
	Node* r_max = nullptr;

	// children nodes
	Node* l_ch = nullptr;
	Node* m1_ch = nullptr;
	Node* m2_ch = nullptr;
	Node* r_ch = nullptr;

	// father node 
	Node* fatherNode = nullptr;
	Node* chainLeft = nullptr;
	Node* chainRight = nullptr;
	bool isLeaf = false;

public:
	// constructors 
	Node() {};

	Node(T* single_value);

	Node(T* first_value, T* second_value);

	Node(T* first_value, T* second_value, T* third_value);

	Node(Node* l_predecessor_node, Node* leftChild, Node* rightChild);

	Node(Node* l_max, Node* mmax, Node* rmax, Node* lch, Node* m1ch, Node* m2ch, Node* rch);

	virtual ~Node() {
		isLeaf = false;
	};

	// setters + getters

	T* getLVal();
	T* getMVal();
	T* getRVal();

	Node* getLMax();
	Node* getMMax();
	Node* getRMax();

	T* getMaxVal();
	int getHeight();

	int getNumVal();
	int getNumCh();
	std::vector<T> getKeyVals();
	std::vector<T> getAllVals();
	std::vector<T*> getLeafValues();
	std::vector<Node*> getAllCh();

	Node* getLCh();
	Node* getM1Ch();
	Node* getM2Ch();
	Node* getRCh();

	Node* getLSibling();
	Node* getRSibling();

	Node* getFather();
	Node* getChainLeft();
	Node* getChainRight();
	bool getIsLeaf();

	void setLVal(T*);
	void setMVal(T*);
	void setRVal(T*);

	void setLMax(Node*);
	void setMMax(Node*);
	void setRMax(Node*);

	void ValueCopy(const Node& node2);

	void setLCh(Node*);
	void setM1Ch(Node*);
	void setM2Ch(Node*);
	void setRCh(Node*);

	void setFather(Node<T>*);
	void setChainLeft(Node<T>*);
	void setChainRight(Node<T>*);

	void clear();
	void removeVal(T*);
	void removeChild(Node<T>*);
	static void postOrderSubTreeDelete(Node<T>*);

	bool isValidNode();

};

template <typename T>
inline Node<T>::Node(T* single_value) {
	// leaf node
	this->l = single_value;
	this->l_max = this;
	this->isLeaf = true;
}

template<typename T>
inline Node<T>::Node(T* first_value, T* second_value)
{
	// leaf node with 2 values
	this->l = first_value;
	this->r = second_value;
	this->l_max = this;
	this->isLeaf = true;
}

template<typename T>
inline Node<T>::Node(T* first_value, T* second_value, T* third_value)
{
	// leaf node with 2 values
	this->l = first_value;
	this->m = second_value;
	this->r = third_value;
	this->l_max = this;
	this->isLeaf = true;
}

template<typename T>
inline Node<T>::Node(Node* l_predecessor_node, Node* leftChild, Node* rightChild) {
	// internal node
	this->l_max = l_predecessor_node;
	this->setLCh(leftChild);
	this->setRCh(rightChild);
	this->isLeaf = false;
}

template<typename T>
inline Node<T>::Node(Node* lmax, Node* mmax, Node* rmax, Node* lch, Node* m1ch, Node* m2ch, Node* rch)
{
	this->l_max = lmax;
	this->m_max = mmax;
	this->r_max = rmax;
	this->setLCh(lch);
	this->setM1Ch(m1ch);
	this->setM2Ch(m2ch);
	this->setRCh(rch);
	this->isLeaf = false;
}

// setters + getters

template<typename T>
inline T* Node<T>::getLVal() {
	return this->l;
}

template<typename T>
inline T* Node<T>::getMVal() {
	return this->m;
}

template<typename T>
inline T* Node<T>::getRVal() {
	return this->r;
}

template<typename T>
inline Node<T>* Node<T>::getLMax() {
	return this->l_max;
}

template<typename T>
inline Node<T>* Node<T>::getMMax() {
	return this->m_max;
}

template<typename T>
inline Node<T>* Node<T>::getRMax() {
	return this->r_max;
}

template<typename T>
inline T* Node<T>::getMaxVal()
{
	return (this->r) ? this->r : this->l;
}

template<typename T>
inline int Node<T>::getHeight()
{
	Node<T>* itr = this;
	if (!itr) return -1;

	int height = -1;
	while (itr) {
		++height;
		itr = itr->getLCh();
	}
	return height;
}

template<typename T>
inline int Node<T>::getNumVal() {

	if (this->m || this->m_max)
		return 3;
	if (this->r || this->r_max)
		return 2;
	return 1;
}

template<typename T>
inline int Node<T>::getNumCh()
{
	int count = 0;
	if (l_ch)
		++count;
	if (m1_ch)
		++count;
	if (m2_ch)
		++count;
	if (r_ch)
		++count;
	return count;
}

template<typename T>
inline std::vector<T> Node<T>::getKeyVals()
{
	std::vector<T> out;
	out.push_back(*this->getLMax()->getMaxVal());
	if (this->getMMax()) {
		out.push_back(*this->getMMax()->getMaxVal());
	}
	if (this->getRMax()) {
		out.push_back(*this->getRMax()->getMaxVal());
	};
	return out;
}

template<typename T>
inline std::vector<T> Node<T>::getAllVals()
{
	std::vector<T> out;
	if (this->l) out.push_back(*l);
	if (this->m) out.push_back(*m);
	if (this->r) out.push_back(*r);
	return out;
}

template<typename T>
inline std::vector<T*> Node<T>::getLeafValues()
{
	std::vector<T*> out;
	out.push_back(this->l);
	if (this->m) out.push_back(this->m);
	if (this->r) out.push_back(this->r);
	return out;
}

template<typename T>
inline std::vector<Node<T>*> Node<T>::getAllCh() {
	std::vector<Node<T>*> out;
	if (this->l_ch) out.push_back(l_ch);
	if (this->m1_ch) out.push_back(m1_ch);
	if (this->m2_ch) out.push_back(m2_ch);
	if (this->r_ch) out.push_back(r_ch);
	return out;
}

template<typename T>
inline Node<T>* Node<T>::getLCh() {
	return this->l_ch;
}

template<typename T>
inline Node<T>* Node<T>::getM1Ch() {
	return this->m1_ch;
}

template<typename T>
inline Node<T>* Node<T>::getM2Ch() {
	return this->m2_ch;
}

template<typename T>
inline Node<T>* Node<T>::getRCh() {
	return this->r_ch;
}

template<typename T>
inline Node<T>* Node<T>::getLSibling() {

	Node<T>* fatherNode = this->getFather();
	if (!fatherNode) return nullptr;
	Node<T>* fathersLeftCh = fatherNode->getLCh();
	Node<T>* fathersM1Ch = fatherNode->getM1Ch();
	Node<T>* fathersM2Ch = fatherNode->getM2Ch();
	Node<T>* fathersRightCh = fatherNode->getRCh();

	if (this == fathersLeftCh) return nullptr;
	if (this == fathersM1Ch) return fathersLeftCh;
	if (this == fathersM2Ch) return fathersM1Ch;
	if (fathersM2Ch != nullptr) return fathersM2Ch;
	else if (fathersM1Ch != nullptr) return fathersM1Ch;
	else return fathersLeftCh;
}

template<typename T>
inline Node<T>* Node<T>::getRSibling() {
	Node<T>* fatherNode = this->getFather();
	if (!fatherNode) return nullptr;
	Node<T>* fathersLeftCh = fatherNode->getLCh();
	Node<T>* fathersM1Ch = fatherNode->getM1Ch();
	Node<T>* fathersM2Ch = fatherNode->getM2Ch();
	Node<T>* fathersRightCh = fatherNode->getRCh();

	if (this == fathersRightCh) return nullptr;
	if (this == fathersM2Ch) return fathersRightCh;
	if (this == fathersM1Ch) {
		if (fathersM2Ch != nullptr) return fathersM2Ch;
		else return fathersRightCh;
	}
	if (fathersM1Ch != nullptr) return fathersM1Ch;
	else return fathersRightCh;
}

template<typename T>
inline Node<T>* Node<T>::getFather() {
	return this->fatherNode;
}

template<typename T>
inline Node<T>* Node<T>::getChainLeft()
{
	return this->chainLeft;
}

template<typename T>
inline Node<T>* Node<T>::getChainRight()
{
	return this->chainRight;
}

template<typename T>
inline bool Node<T>::getIsLeaf()
{
	return this->isLeaf;
}

template<typename T>
inline void Node<T>::setLVal(T* p) {
	this->l = p;
}

template<typename T>
inline void Node<T>::setMVal(T* p) {
	this->m = p;
}

template<typename T>
inline void Node<T>::setRVal(T* p) {
	this->r = p;
}

template<typename T>
inline void Node<T>::setLMax(Node* n)
{
	this->l_max = n;
}

template<typename T>
inline void Node<T>::setMMax(Node* n)
{
	this->m_max = n;
}

template<typename T>
inline void Node<T>::setRMax(Node* n)
{
	this->r_max = n;
}

template<typename T>
inline void Node<T>::ValueCopy(const Node& other)
{
	this->l = other.l;
	this->m = other.m;
	this->r = other.r;

	this->l_max = other.l_max;
	this->m_max = other.m_max;
	this->r_max = other.r_max;

	this->isLeaf = other.isLeaf;
}

template<typename T>
inline void Node<T>::setLCh(Node* n) {
	this->l_ch = n;
	if (n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setM1Ch(Node* n) {
	this->m1_ch = n;
	if (n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setM2Ch(Node* n) {
	this->m2_ch = n;
	if (n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setRCh(Node* n) {
	this->r_ch = n;
	if (n) n->setFather(this);
}

template<typename T>
inline void Node<T>::setFather(Node<T>* n) {
	this->fatherNode = n;
}

template<typename T>
inline void Node<T>::setChainLeft(Node<T>* l)
{
	this->chainLeft = l;
	if (l) l->setChainRight(this);
}

template<typename T>
inline void Node<T>::setChainRight(Node<T>* r)
{
	this->chainRight = r;
}

template<typename T>
void Node<T>::clear() {
	this->l = nullptr;
	this->m = nullptr;
	this->r = nullptr;
	this->l_ch = nullptr;
	this->m1_ch = nullptr;
	this->m2_ch = nullptr;
	this->r_ch = nullptr;
}

template<typename T>
inline void Node<T>::removeVal(T* p)
{
	if (l != nullptr) if (*p == *l) l = nullptr;
	if (m != nullptr) if (*p == *m) m = nullptr;
	if (r != nullptr) if (*p == *r) r = nullptr;
}

template<typename T>
inline void Node<T>::removeChild(Node<T>* n)
{
	if (n == this->l_ch) this->l_ch = nullptr;
	if (n == this->r_ch) this->r_ch = nullptr;
	if (n == this->m1_ch) this->m1_ch = nullptr;
	if (n == this->m2_ch) this->m2_ch = nullptr;
}

template<typename T>
inline void Node<T>::postOrderSubTreeDelete(Node<T>* n) {
	if (!n) return;

	postOrderSubTreeDelete(n->getLCh());
	postOrderSubTreeDelete(n->getM1Ch());
	postOrderSubTreeDelete(n->getM2Ch());
	postOrderSubTreeDelete(n->getRCh());
	delete n;
	// always remember to change the appropriate pointer to nullptr.
}

template<typename T>
inline bool Node<T>::isValidNode()
{
	// this function does not check continuity between neighboring nodes or father-children continuities.

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
	}
	else {

		if (!this->getLMax()) {
			__debugbreak();
			return false;
		}

		if (this->getLMax() == this->getMMax() || this->getLMax() == this->getRMax() || (this->getMMax() && this->getMMax() == this->getRMax())) {
			__debugbreak();
			return false;
		}

		int NodeSize = this->getNumVal();
		int NumChildren = this->getNumCh();

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
		else {
			if (NumChildren != 2) {
				__debugbreak();
				return false;
			}
		}
	}
	return true;
}
