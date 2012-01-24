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
Number Number::operator* (const Number& other)
{
	return Number(getInt() * other.getInt());
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
	firstChild = 0;
	
	std::clog << "dcounter: " << dcounter << " ncounter: " << ncounter << std::endl;
	
	if (hasChild)
	{
		std::clog << "...end-destruct node" << std::endl;
	}
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
void ExpressionNode::insertRight(const ExpressionNode& newRight)
{
	ExpressionNode * temp;
	
	if (right == 0)
	{
		right = new ExpressionNode(newRight);
		ncounter += 1;
		std::clog << "... used new for right" << std::endl;
	}
	else
	{
		temp = right;
		*right = newRight;
		right->insertRight(*temp);
	}
}
ExpressionNode* ExpressionNode::getFirstChild() const {return firstChild;}
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
		curNode->insertRight(newChild);
	}
}
void ExpressionNode::replaceFirstChild(const ExpressionNode & newChild)
{
	if (firstChild == 0)
	{
		throw GenericError("no first child to replace");
	}
	else
	{
		*firstChild = newChild;
	}
}
void ExpressionNode::deleteFirstChild()
{
	ExpressionNode * temp;
	
	if (firstChild == 0)
	{
		throw GenericError("no first child to delete");
	}
	else
	{
		temp = firstChild->getRight();
		delete firstChild;
		dcounter += 1;
		firstChild = temp;
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
ExpressionNode* ExpressionNode::findParentOf(const ExpressionNode* target)
{
	ExpressionNode * currentNode = this;
	std::stack <ExpressionNode*> nodeStack;
	
	std::clog << *this << ".findParentOf(" << *target << ")" << std::endl;
	
	while (true)
	{
		if (currentNode != 0)
		{
			if (currentNode == target)
			{
				if (nodeStack.size() == 0)
				{
					return 0;
				}
				else
				{
					return nodeStack.top();
				}
			}
			nodeStack.push(currentNode);
			currentNode = currentNode->getFirstChild();
			continue;
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
ExpressionNode & ExpressionNode::operator= (const ExpressionNode &newNode)
{
	std::clog << "<assigning> " << *this << " with " << newNode << std::endl;
	
	ExpressionNode *curNew = newNode.getFirstChild();
	
	if (this == &newNode)
	{
		return *this;
	}
	setType(newNode.getType());
	setOperation(newNode.getOperation());
	//setRight(newNode.getRight()); //automatically sets other siblings
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
	
	std::clog << "</assigned>" << std::endl;
}
void ExpressionNode::remove(ExpressionNode * target)
{
	
	ExpressionNode * parent = findParentOf(target);
	std::clog << *parent << std::endl;
	if (this == target)
	{
		throw EmptyTreeError();
	}
	else
	{
		assert(parent!=0);
		if (parent->getOperation()->getArity() == 1)
		{
			remove(parent);
		}
		else
		{
			*target = ExpressionNode(parent->getOperation()->getIdentity()); //same as target.replace()
			parent->simplify();
		}
	}
}

void ExpressionNode::simplify()
{
	//only simplify OPERATION nodes with an operation
	if (type == OPERATION && operation != 0)
	{
		std::clog << "!!begin simplify: " << *this << std::endl;
		
		if (firstChild == 0)
		{
			appendChild(ExpressionNode(operation->getIdentity()));
		}
		ExpressionNode newNode = operation->simplify(*this);
		// what happens when no simplification occurs?
		if (this == &newNode)
		{
			std::cout << "this == &newNode in ExpressionNode::simplify" << std::endl;
			return;
		}
		std::clog << "!!simplifying " << *this << " to " << newNode << std::endl;
		*this = newNode;
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
			else
			{
				out << *(node.getOperation());
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
