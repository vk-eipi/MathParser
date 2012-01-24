#include <iostream>
#include <stack>
#include <cassert>

#include "expression.hpp"
#include "operation.hpp"

int ncounter = 0;
int dcounter = 0;

Number::Number(): value(1) {}
Number::Number(int n): value(n) {}
int Number::getInt() const {return value;}
Number Number::operator+ (const Number& other)
{
	return Number(getInt() + other.getInt());
}
bool Number::operator== (const Number& other) const
{
	return getInt() == other.getInt();
}
std::ostream& operator<<(std::ostream& out, const Number& num)
{
	out << num.getInt();
	return out;
}

Variable::Variable()
{
	setSymbol(0);
}
Variable::Variable(char s): symbol(s) {}

char Variable::getSymbol() const
{
	return symbol;
}

void Variable::setSymbol(char s)
{
	symbol = s;
}

std::ostream& operator<<(std::ostream& out, const Variable& variable)
{
	out << variable.getSymbol();
	return out;
}


ExpressionNode::ExpressionNode(): 
// initialize with arbritary decisions for type, value, activechildren; null pointers for rest
type(OPERATION), operation(0), activechildren(0), right(0), firstChild(0), variable(0),value(1) {}

ExpressionNode::ExpressionNode(int num):
type(NUMBER), operation(0), activechildren(0), right(0), firstChild(0), variable(0), value(num) {}

ExpressionNode::ExpressionNode(Number num):
type(NUMBER), operation(0), activechildren(0), right(0), firstChild(0), variable(0), value(num) {}

ExpressionNode::ExpressionNode(const Operation* operation):
type(OPERATION), operation(operation), activechildren(operation->getArity()), right(0), firstChild(0), variable(0), value(1) {}

ExpressionNode::ExpressionNode(const Variable* varPtr):
type(VARIABLE), operation(0), activechildren(0), right(0), firstChild(0), variable(varPtr), value(1) {}

ExpressionNode::ExpressionNode(const ExpressionNode & other): //copy constructor
type(other.getType()), operation(other.getOperation()), activechildren(0),
right(0), firstChild(0), variable(other.getVariable()), value(other.getValue())
{
	ExpressionNode* otherChild = other.getFirstChild();
	
	std::clog << "copy" << std::endl;
	if (operation != 0)
	{
		activechildren = other.getOperation()->getArity();
	}
	//setRight(other.getRight());
	
	while (otherChild != 0)
	{
		appendChild(*otherChild);
		otherChild = otherChild->getRight();
	}
}

ExpressionNode::~ExpressionNode()
{
	std::clog << "...destructing node... " << *this << std::endl;
	ExpressionNode *ptr = firstChild;
	ExpressionNode *next = 0;
	bool hasChild = (ptr != 0);
	
	while (ptr != 0)
	{
		next = ptr->getRight();
		delete ptr;
		dcounter += 1;
		ptr = next;
	}
	
	std::clog << "dcounter: " << dcounter << " ncounter: " << ncounter << std::endl;
	
	if (hasChild)
	{
		std::clog << "...end-destruct node" << std::endl;
	}
}

void ExpressionNode::init(const Operation* operation, ExpressionNode* firstChild)
{
	type = OPERATION;
	this->operation = operation;
	activechildren = operation->getArity();
	setFirstChild(firstChild);
}

//getters and setters
NodeType ExpressionNode::getType() const {return type;}
void ExpressionNode::setType(NodeType t) {type = t;}
const Operation* ExpressionNode::getOperation() const {return operation;}
void ExpressionNode::setOperation(const Operation* newOp) 
{
	operation = newOp;
	if (newOp != 0)
	{
		activechildren = newOp->getArity();
	}
}
ExpressionNode* ExpressionNode::getRight() const {return right;}
void ExpressionNode::setRight(const ExpressionNode* newRight) // also changes further rights (siblings)
{
	ExpressionNode *nextRight;
	
	if (newRight != 0)
	{ 
		if (right == 0)
		{
			right = new ExpressionNode;
			ncounter += 1;
			std::clog << "... used new for right" << std::endl;
			right->replace(*newRight);
			right->setRight(newRight->getRight()); // recursively adds siblings
		}
		else
		{
			right->replace(*newRight);
			right->setRight(newRight->getRight());
		}
	}
	else // newRight = 0
	{
		while (right != 0)
		{
			nextRight = right->getRight();
			delete right;
			dcounter += 1;
			right = nextRight;
		}
	}
}
//void ExpressionNode::addRight(ExpressionNode newRight)
//{
	//right = new ExpressionNode;
	//right->replace(newRight);
//}
ExpressionNode* ExpressionNode::getFirstChild() const {return firstChild;}
void ExpressionNode::setFirstChild(const ExpressionNode* newChild) 
{	
	if (newChild != 0)
	{ 
		if (firstChild == 0)
		{
			firstChild = new ExpressionNode;
			ncounter += 1;
			std::clog << "... used new for firstChild" << std::endl;
			firstChild->replace(*newChild);
			firstChild->setRight(newChild->getRight());
		}
		else
		{
			firstChild->replace(*newChild);
			firstChild->setRight(newChild->getRight());
		}
	}
	else if (firstChild != 0)
	{
		firstChild->setRight(0);
		delete firstChild;
		dcounter += 1;
		firstChild = 0;
	}
}
//void ExpressionNode::addFirstChild(ExpressionNode newChild)
//{
	//firstChild = new ExpressionNode;
	//firstChild->replace(newChild);
//}
void ExpressionNode::appendChild(const ExpressionNode& newChild)
{
	ExpressionNode* nxtNode = firstChild;
	ExpressionNode* curNode = 0;
	if (firstChild == 0)
	{
		firstChild = new ExpressionNode(newChild);
		ncounter += 1;
		std::clog << "... used new for firstChild" << std::endl;
	}
	else
	{
		while(nxtNode != 0)
		{
			curNode = nxtNode;
			nxtNode = nxtNode->getRight();
		}
		curNode->setRight(&newChild);
	}
}
void ExpressionNode::deleteChildren()
{
	ExpressionNode *ptr = firstChild;
	ExpressionNode *next = 0;
	
	while (ptr != 0)
	{
		next = ptr->getRight();
		delete ptr;
		dcounter += 1;
		ptr = next;
	}
	firstChild = 0;
	
	std::clog << "dcounter: " << dcounter << " ncounter: " << ncounter << std::endl;
}
const Variable* ExpressionNode::getVariable() const {return variable;}
void ExpressionNode::setVariable(const Variable* varPtr) {variable = varPtr;}
Number ExpressionNode::getValue() const {return value;};
void ExpressionNode::setValue(Number newValue) {value = newValue;}

const ExpressionNode* ExpressionNode::findParentOf(const ExpressionNode& target) const
{
	//ExpressionNode * currentNode;
	//ExpressionNode * parent;
	
	//if (this == &target)
	//{
		//return 0;
	//}
	
	//currentNode = getFirstChild();
	//while (currentNode != 0)
	//{
		//if (currentNode == &target)
		//{
			//return this;
		//}
		//parent = currentNode->findParentOf(target);
		//if (parent == -1)
		//{
			//currentNode = currentNode->getRight();
		//}
		//else
		//{
			//assert(parent!=0); // since we already checked for it
			//return parent;
		//}
	//}
	//return -1; // target not found
	
	const ExpressionNode * currentNode = this;
	std::stack <const ExpressionNode*> nodeStack;
	const ExpressionNode * parent = 0;
	
	while (true)
	{
		if (currentNode != 0)
		{
			if (currentNode == &target)
			{
				return parent;
			}
			nodeStack.push(currentNode);
			parent = currentNode;
			currentNode = currentNode->getFirstChild();
		}
		if (nodeStack.size() == 1) // only root left in stack
		{
			// target not found
			throw TargetNotFoundError();
		}
		currentNode = nodeStack.top();
		nodeStack.pop();
		currentNode = currentNode->getRight();
	}
}

ExpressionNode* ExpressionNode::findParentOf(const ExpressionNode& target)
{
	ExpressionNode * currentNode = this;
	std::stack <ExpressionNode*> nodeStack;
	ExpressionNode * parent = 0;
	
	while (true)
	{
		if (currentNode != 0)
		{
			if (currentNode == &target)
			{
				return parent;
			}
			nodeStack.push(currentNode);
			parent = currentNode;
			currentNode = currentNode->getFirstChild();
		}
		if (nodeStack.size() == 1) // only root left in stack
		{
			// target not found
			throw TargetNotFoundError();
		}
		currentNode = nodeStack.top();
		nodeStack.pop();
		currentNode = currentNode->getRight();
	}
	
}

bool ExpressionNode::operator== (const ExpressionNode& other) const
{
	ExpressionNode *selfptr;
	ExpressionNode *otherptr;
	
	if (getType() == other.getType() && 
		getOperation() == other.getOperation() && 
		getVariable() == other.getVariable() &&
		getValue() == other.getValue() )
	{
		selfptr = firstChild;
		otherptr = other.getFirstChild();
		while (selfptr != 0 && otherptr != 0)
		{
			if (*selfptr == *otherptr)
			{
				selfptr = selfptr->getRight();
				otherptr = otherptr->getRight();
			}
			else
			{
				return false;
			}
		}
		if (selfptr == 0 && otherptr == 0)
		{
			return true;
		}
		else
		{
			assert(selfptr != otherptr);
			return false;
		}
	}
	else
	{	
		return false;
	}
}

//ExpressionNode ExpressionNode::copy() const
//{
	//std::clog << "...<copying> "  << *this << std::endl;
	//ExpressionNode newNode;
	//newNode.replace(*this);
	//newNode.setRight(getRight()); //necessary?
	//std::clog << "...</copying>" << std::endl;
	//return newNode;
//}

ExpressionNode & ExpressionNode::operator= (const ExpressionNode &newNode)
{
	ExpressionNode *curNew = newNode.getFirstChild();
	
	if (this == &newNode)
	{
		return *this;
	}
	setType(newNode.getType());
	setOperation(newNode.getOperation());
	setRight(newNode.getRight()); //automatically sets other siblings
	deleteChildren();
	assert(firstChild==0);
	while(curNew != 0)
	{
		appendChild(*curNew);
		curNew = curNew->getRight();
	}
	setVariable(newNode.getVariable());
	setValue(newNode.getValue());
	return *this;
}

void ExpressionNode::replace(const ExpressionNode &newNode)
{
	std::clog << "...<replacing> "  << *this << " with " << newNode << std::endl;
	
	ExpressionNode *curNew = newNode.getFirstChild();
	
	setType(newNode.getType());
	setOperation(newNode.getOperation());
	setVariable(newNode.getVariable());
	setValue(newNode.getValue());
//	setRight(newNode.getRight());

	deleteChildren();
	assert(firstChild==0);
	while(curNew != 0)
	{
		appendChild(*curNew);
		curNew = curNew->getRight();
	}
	
	std::clog << "...</replacing>" << std::endl;
}

void ExpressionNode::remove(ExpressionNode& target)
{
	ExpressionNode * parent = findParentOf(target);
	
	if (this == &target)
	{
		throw EmptyTreeError();
	}
	else
	{
		assert(parent!=0);
		if (parent->getOperation()->getArity() == 1)
		{
			remove(*parent);
		}
		else
		{
			target.replace(ExpressionNode(parent->getOperation()->getIdentity()));
			(*parent).simplify();
		}
	}
}

void ExpressionNode::simplify()
{
	//only simplify OPERATION nodes with an operation
	if (type == OPERATION && operation != 0)
	{
		if (firstChild == 0)
		{
			appendChild(ExpressionNode(operation->getIdentity()));
		}
		ExpressionNode newNode = operation->simplify(*firstChild);
		// what happens when no simplification occurs?
		if (this == &newNode)
		{
			std::cout << "this == &newNode in ExpressionNode::simplify" << std::endl;
			return;
		}
		replace(newNode);
	}
}

void ExpressionNode::deepSimplify()
{
	ExpressionNode * curNode = firstChild;
	
	if (type == OPERATION && operation != 0)
	{
		while (curNode != 0)
		{
			curNode->deepSimplify();
			curNode = curNode->getRight();
		}
		simplify();
	}
}

const char* ExpressionNode::WrongArityError::what() const throw()
{
	std::string s;
	s += "ERROR: Arity of operation does not match with expressions. \n";
	s += info;
	return s.c_str();
}
const char* ExpressionNode::TargetNotFoundError::what() const throw()
{
	return "ERROR: target not found.";
}
const char* ExpressionNode::EmptyTreeError::what() const throw()
{
	return "ERROR: Empty trees have not been implemented.";
}

std::ostream& operator<<(std::ostream& out, const ExpressionNode& node)
{
	ExpressionNode * ptr = 0;
	
	if (node.getType() == OPERATION)
	{
		if (node.getOperation() == 0)
		{
			if (node.getFirstChild() != 0)
			{
				throw ExpressionNode::GenericError("ERROR: inserting OPERATION parent node w/o operation into ostream");
			}
			else
			{
				out << "()";
				return out;
			}		
		}
		if (node.getOperation()->isFuncFormat() == true)
		{
			out << *(node.getOperation()) << "(";
			ptr = node.getFirstChild();
			if (ptr != 0)
			{
				out << *ptr;
				ptr = ptr->getRight();
				while (ptr != 0)
				{
					out << ", " << *ptr;
					ptr = ptr->getRight();
				}
			}
			out << ")";
		}
		else
		{
			out << "(";
			ptr = node.getFirstChild();
			if (ptr != 0)
			{
				out << *ptr;
				ptr = ptr->getRight();
				while (ptr != 0)
				{
					out << *(node.getOperation()) << *ptr;
					ptr = ptr->getRight();
				}
			}
			out << ")";
		}
	}
	else if (node.getType() == VARIABLE)
	{
		if (node.getVariable() == 0)
		{
			throw ExpressionNode::GenericError("ERROR: inserting VARIABLE node w/o variable into ostream");
		}
		out << *(node.getVariable());
	}
	else
	{
		if (node.getType() != NUMBER)
		{
			int i = node.getType();
			std::clog << std::endl << i << std::endl;
		}
		assert(node.getType() == NUMBER);
		out << node.getValue();
	}
	return out;
}
