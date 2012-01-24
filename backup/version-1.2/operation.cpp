#include <iostream>
#include <stack>
#include <vector>
#include <cassert>
#include "operation.hpp"
#include "expression.hpp"

using namespace std;

const Addition ADDITION;
const Multiplication MULTIPLICATION;

string Operation::getSymbol() const {return symbol;}
bool Operation::isFuncFormat() const {return funcFormat;}
int Operation::getArity() const {return arity;}
Number Operation::getIdentity() const {return identity;}
ostream& operator<<(ostream& out, const Operation& op)
{
	out << op.getSymbol();
	return out;
}


ExpressionNode BinaryOperation::simplify(ExpressionNode& left) const
{
	if (left.getRight() != 0)
	{
		if ((left.getRight())->getRight() != 0)
		{
			throw ExpressionNode::WrongArityError("> 2 arguments for BinaryOperation");
		}
		return simplify(left, *left.getRight());
	}
	else
	{
		cerr << left << endl;
		throw ExpressionNode::WrongArityError("only 1 argument for BinaryOperation"); 
	}
}
bool Addition::isAddable(const ExpressionNode& term1, const ExpressionNode& term2) const
{
	clog << "checkpoint isAddable(" << term1 << ", " << term2 << ")" << endl;
	
	const ExpressionNode * varpart1(0);
	const ExpressionNode * varpart2(0);
	const ExpressionNode * left;
	const ExpressionNode * right;

	if (term1.getType() == NUMBER)
	{
		varpart1 = 0;
	}
	else if (term1.getType() == OPERATION)
	{
		if (term1.getOperation() == &MULTIPLICATION)
		{
			left = term1.getFirstChild();
			right = left->getRight();
			if (left->getType() == NUMBER)
			{
				varpart1 = right;
			}
			else if (right->getType() == NUMBER)
			{
				varpart1 = left;
			}
			else
			{
				varpart1 = &term1;
			}
		}
		else
		{
			varpart1 = &term1;
		}
	}
	else // VARIABLE
	{
		assert(term1.getType() == VARIABLE);
		varpart1 = &term1;
	}
	
	if (term2.getType() == NUMBER)
	{
		varpart2 = 0;
	}
	else if (term2.getType() == OPERATION)
	{
		if (term2.getOperation() == &MULTIPLICATION)
		{
			left = term2.getFirstChild();
			right = left->getRight();
			if (left->getType() == NUMBER)
			{
				varpart2 = right;
			}
			else if (right->getType() == NUMBER)
			{
				varpart2 = left;
			}
			else
			{
				varpart2 = &term2;
			}
		}
		else
		{
			varpart2 = &term2;
		}
	}
	else // VARIABLE
	{
		assert(term2.getType() == VARIABLE);
		varpart2 = &term2;
	}
	
	if (varpart1 != 0 && varpart2 != 0)
	{
		if (*varpart1 == *varpart2)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		if (varpart1 == 0 && varpart2 == 0)
		{
			// both numbers
			return true;
		}
		else
		{
			return false;
		}
	}
}
ExpressionNode Addition::addTerms(const ExpressionNode& term1, const ExpressionNode& term2) const
{
	clog << "checkpoint addTerms" << endl;
	
	Number coefficient1;
	Number coefficient2;
	const ExpressionNode * varpart1;
	const ExpressionNode * varpart2;
	ExpressionNode * left;
	ExpressionNode * right;
	ExpressionNode newRight;
	ExpressionNode newCoef;
	ExpressionNode newNode;
	
	if (term1.getType() == NUMBER)
	{
		coefficient1 = term1.getValue();
		varpart1 = 0;
	}
	else if (term1.getType() == OPERATION)
	{
		if (term1.getOperation() == &MULTIPLICATION)
		{
			left = term1.getFirstChild();
			right = left->getRight();
			if (left->getType() == NUMBER)
			{
				coefficient1 = left->getValue();
				varpart1 = right;
			}
			else if (right->getType() == NUMBER)
			{
				coefficient1 = right->getValue();
				varpart1 = left;
			}
			else
			{
				coefficient1 = MULTIPLICATION.getIdentity();
				varpart1 = &term1;
			}
		}
		else
		{
			coefficient1 = MULTIPLICATION.getIdentity();
			varpart1 = &term1;
		}
	}
	else // VARIABLE
	{
		assert(term1.getType() == VARIABLE);
		coefficient1 = MULTIPLICATION.getIdentity();
		varpart1 = &term1;
	}
	
	if (term2.getType() == NUMBER)
	{
		coefficient2 = term2.getValue();
		varpart2 = 0;
	}
	else if (term2.getType() == OPERATION)
	{
		if (term2.getOperation() == &MULTIPLICATION)
		{
			left = term2.getFirstChild();
			right = left->getRight();
			if (left->getType() == NUMBER)
			{
				coefficient2 = left->getValue();
				varpart2 = right;
			}
			else if (right->getType() == NUMBER)
			{
				coefficient2 = right->getValue();
				varpart2 = left;
			}
			else
			{
				coefficient2 = MULTIPLICATION.getIdentity();
				varpart2 = &term2;
			}
		}
		else
		{
			coefficient2 = MULTIPLICATION.getIdentity();
			varpart2 = &term2;
		}
	}
	else // VARIABLE
	{
		assert(term2.getType() == VARIABLE);
		coefficient2 = MULTIPLICATION.getIdentity();
		varpart2 = &term2;
	}
	
	if (varpart1 != 0 && varpart2 != 0)
	{
		if (*varpart1 == *varpart2)
		{
			clog << "addTerms simplify like terms" << endl
				<< "coef1: " << coefficient1 << " var1: " << *varpart1 << endl
				<< "coef2: " << coefficient2 << " var2: " << *varpart2 << endl;
			
			newCoef = ExpressionNode(coefficient1 + coefficient2);
			newRight = *varpart1;
			newNode = ExpressionNode(&MULTIPLICATION);
			newNode.appendChild(newCoef);
			newNode.appendChild(newRight);
			clog << "sum: " << newNode << endl;
			return newNode;
		}
		else
		{
			clog << "*varpart1: " << *varpart1 << " *varpart2: " << endl;
			throw ExpressionNode::GenericError("adding unaddable terms");
		}
	}
	else
	{
		if (varpart1 == 0 && varpart2 == 0)
		{
			clog << "sum: " << coefficient1 + coefficient2 << endl;
			return ExpressionNode(coefficient1 + coefficient2);
		}
		else
		{
			throw ExpressionNode::GenericError("adding unaddable terms");
		}
	}
	throw ExpressionNode::GenericError("reached end of addTerms w/o returning.");
	//if (left.getNodeType() == NUMBER && right.getNodeType() == NUMBER)
	//{
		//ExpressionNode newNode;
		//newNode.setNodeType(NUMBER);
		//newNode.setValue(Number(left.getValue().getInt() + right.getValue().getInt()));
		//return newNode;
	//}
}

ExpressionNode Addition::simplify(ExpressionNode& left, ExpressionNode& right) const
{
	clog << "checkpoint addsimplify" << endl;
	
	//simplest case: at least one side is just 0
	if (left.getType() == NUMBER && left.getValue().getInt() == 0)
	{
		return right;
	}
	else if (right.getType() == NUMBER && right.getValue().getInt() == 0)
	{
		return left;
	}
	
	// for each left term: for each right term: try adding
	
	stack <ExpressionNode*> leftNodeStack;
	ExpressionNode * currentLeftNode = &left;
	bool leftFinished = false;
	stack <ExpressionNode*> rightNodeStack;
	vector <ExpressionNode*> rightDeleteList;
	ExpressionNode * currentRightNode = &right;
	bool rightFinished = false;
	ExpressionNode newNode(&ADDITION);
	
	while (leftFinished == false)
	{
		clog << "looping left" << endl;
		
		if (currentLeftNode->getType() == OPERATION && currentLeftNode->getOperation() == &ADDITION)
		{
			
			if (currentLeftNode->getFirstChild() == 0)
			{
				throw ExpressionNode::WrongArityError();
			}
			currentLeftNode = currentLeftNode->getFirstChild();
			leftNodeStack.push(currentLeftNode);
			continue;
		}
		
		// leaf term on left tree: traverse right tree
		rightFinished = false;
		currentRightNode = &right;
		while (rightFinished == false)
		{
			clog << "looping right" << endl;
			
			if (currentRightNode->getType() == OPERATION && currentRightNode->getOperation() == &ADDITION)
			{
				if (currentRightNode->getFirstChild() == 0)
				{
					throw ExpressionNode::WrongArityError();
				}
				currentRightNode = currentRightNode->getFirstChild();
				rightNodeStack.push(currentRightNode);
				continue;
			}
			assert(currentLeftNode != 0);
			assert(currentRightNode !=0);
			clog << "checkpoint addsimplify: before isAddable(" << *currentLeftNode << ", " << *currentRightNode << ")"<<  endl;
			// leaf terms on both sides: attempt adding
			if (isAddable(*currentLeftNode, *currentRightNode))
			{
				clog << "checkpoint addsimplify: after isAddable; " << endl;
				clog << *currentLeftNode << " " << *currentRightNode << endl;
				(*currentLeftNode) = addTerms(*currentLeftNode, *currentRightNode);
				clog << "checkpoint addsimplify: after addTerms; " << endl;
				clog << "currentLeftNode: " << *currentLeftNode << " RightNode:" << *currentRightNode << endl;
				clog << "left " << left << "right " << right << endl;
				if (currentRightNode == &right)
				{
					//entire right tree has been assimilated
					return left;
				}
				// try marking for deletion after right loop instead of removing
				rightDeleteList.push_back(currentRightNode);
			}
			while (true)
			{
				clog << "rightNodeStack: " << rightNodeStack.size() << endl;
				if (rightNodeStack.size() != 0) clog << "rightNodeStack.top(): " << *(rightNodeStack.top()) << endl;
				if (rightNodeStack.size() == 0)
				{
					rightFinished = true;
					break;
				}
				currentRightNode = rightNodeStack.top();
				rightNodeStack.pop();
				if (currentRightNode->getRight() != 0)
				{
					currentRightNode = currentRightNode->getRight();
					rightNodeStack.push(currentRightNode);
					break;
				}
			}
		}
		// deleting marked rights
		for (vector<ExpressionNode*>::iterator it = rightDeleteList.begin(); it != rightDeleteList.end(); it++)
		{
			right.remove(*it);
		}
		rightDeleteList.clear();
		clog << "here left: " << left << "right: " << right <<endl;
		
		//clog << "leftNodeStack: " << leftNodeStack.size() << endl;
		//if (leftNodeStack.size() > 0)
		//{
			//clog << "leftNodeStack: " << *(leftNodeStack.top()) << endl;
		//}
		while (true)
		{
			if (leftNodeStack.size() == 0)
			{
				leftFinished = true;
				break;
			}
			currentLeftNode = leftNodeStack.top();
			leftNodeStack.pop();
			if (currentLeftNode->getRight() != 0)
			{
				currentLeftNode = currentLeftNode->getRight();
				leftNodeStack.push(currentLeftNode);
				break;
			}
		}
	}
	//still some terms left on right
	newNode.appendChild(left);
	newNode.appendChild(right);
	return newNode;
}

ExpressionNode Multiplication::simplify(ExpressionNode& left, ExpressionNode& right) const
{
	// unimplemented
	ExpressionNode newNode(&MULTIPLICATION);
	newNode.appendChild(left);
	newNode.appendChild(right);
	return newNode;
}
