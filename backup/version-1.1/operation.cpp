#include <iostream>
#include <stack>
#include <cassert>
#include "operation.hpp"
#include "expression.hpp"

const Addition ADDITION;
const Multiplication MULTIPLICATION;

std::string Operation::getSymbol() const {return symbol;}
bool Operation::isFuncFormat() const {return funcFormat;}
int Operation::getArity() const {return arity;}
Number Operation::getIdentity() const {return identity;}
std::ostream& operator<<(std::ostream& out, const Operation& op)
{
	out << op.getSymbol();
	return out;
}


ExpressionNode BinaryOperation::simplify(ExpressionNode& left) const
{
	if (left.getRight() > 0)
	{
		if ((left.getRight())->getRight() != 0)
		{
			throw ExpressionNode::WrongArityError("> 2 arguments for BinaryOperation");
		}
		return simplify(left, *left.getRight());
	}
	else
	{
		throw ExpressionNode::WrongArityError("only 1 argument for BinaryOperation"); 
	}
}
bool Addition::isAddable(const ExpressionNode& term1, const ExpressionNode& term2) const
{
	std::clog << "checkpoint isAddable" << std::endl;
	
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
	std::clog << "checkpoint addTerms" << std::endl;
	
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
			std::clog << "addTerms simplify like terms" << std::endl
				<< "coef1: " << coefficient1 << " var1: " << *varpart1 << std::endl
				<< "coef2: " << coefficient2 << " var2: " << *varpart2 << std::endl;
			
			newCoef = ExpressionNode(coefficient1 + coefficient2);
			newRight.replace(*varpart1);
			newNode = ExpressionNode(&MULTIPLICATION);
			newNode.appendChild(newCoef);
			newNode.appendChild(newRight);
			std::clog << "sum: " << newNode << std::endl;
			return newNode;
		}
		else
		{
			std::clog << "*varpart1: " << *varpart1 << " *varpart2: " << std::endl;
			throw ExpressionNode::GenericError("adding unaddable terms");
		}
	}
	else
	{
		if (varpart1 == 0 && varpart2 == 0)
		{
			std::clog << "sum: " << coefficient1 + coefficient2 << std::endl;
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
	std::clog << "checkpoint addsimplify" << std::endl;
	
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
	
	std::stack <ExpressionNode*> leftNodeStack;
	ExpressionNode * currentLeftNode = &left;
	bool leftFinished = false;
	std::stack <ExpressionNode*> rightNodeStack;
	ExpressionNode * currentRightNode = &right;
	bool rightFinished = false;
	ExpressionNode * tempNodePtr;
	ExpressionNode newNode(&ADDITION);
	
	while (leftFinished == false)
	{
		std::clog << "looping left" << std::endl;
		
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
		while (rightFinished == false)
		{
			std::clog << "looping right" << std::endl;
			
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
			
			std::clog << "checkpoint addsimplify: before isAddable; " << std::endl;
			// leaf terms on both sides: attempt adding
			if (isAddable(*currentLeftNode, *currentRightNode))
			{
				std::clog << "checkpoint addsimplify: after isAddable; " << std::endl;
				std::clog << *currentLeftNode << " " << *currentRightNode << std::endl;
				(*currentLeftNode).replace( addTerms(*currentLeftNode, *currentRightNode) );
				std::clog << "checkpoint addsimplify: after addTerms; " << std::endl;
				if (currentRightNode == &right)
				{
					//entire right tree has been assimilated
					return left;
				}
				tempNodePtr = currentRightNode;
				currentRightNode = right.findParentOf(*currentRightNode);
				right.remove(*tempNodePtr);
			} 
			while (true)
			{
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
	left.setRight(&right);
	newNode.setFirstChild(&left);
	return newNode;
}
