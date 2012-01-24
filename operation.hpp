#ifndef OPERATION_HPP
#define OPERATION_HPP

#include <iostream>
#include "expression.hpp"

//class ExpressionNode;

class Operation
{
	protected:
		std::string symbol;
		bool funcFormat; 
		int arity;
		Number identity;
	public:
		Operation(int args) : symbol(""), funcFormat(true), arity(args) {}
		Operation(int args, bool isFunc): symbol(""), funcFormat(isFunc), arity(args) {}
		std::string getSymbol() const;
		bool isFuncFormat() const;
		int getArity() const; // arity of -1 refers to arbritrary num of args 
		Number getIdentity() const;
		virtual ExpressionNode simplify(const ExpressionNode& root) const = 0;
};
std::ostream& operator<<(std::ostream&, const Operation&);

class BinaryOperation: public Operation
{
	protected:
		bool commutative;
		bool associative;
	public:
		BinaryOperation(): Operation(2,false) {}
		BinaryOperation(bool com, bool assoc): Operation(2, false), commutative(com), associative(assoc) {}
		virtual ExpressionNode simplify(ExpressionNode& left, ExpressionNode& right) const = 0;
		virtual bool isCompatible(const ExpressionNode&, const ExpressionNode&) const {return false;}
		ExpressionNode simplify(const ExpressionNode& root) const; // consider being virtual
};

class ChainOperation: public Operation
{
	const BinaryOperation * pairwiseOp; // assumed to be commutative and associative
	public:
		ChainOperation(const BinaryOperation* binOp): Operation(-1,binOp->isFuncFormat())
		{
			symbol = binOp->getSymbol();
			identity = binOp->getIdentity();
			pairwiseOp = binOp;
		}
		ExpressionNode simplify(const ExpressionNode& root) const;
};

class Addition: public BinaryOperation
{
	private:
		bool isCompatible(const ExpressionNode& term1, const ExpressionNode& term2) const;
		ExpressionNode addTerms(const ExpressionNode& term1, const ExpressionNode& term2) const; // should change to use heap memory
	public:
		Addition(): BinaryOperation(true, true) {symbol = '+'; identity = Number(0);};
		ExpressionNode simplify(ExpressionNode& left, ExpressionNode& right) const;
//		ExpressionNode simplify(const ExpressionNode& left);
} extern const ADDITION;

class Multiplication: public BinaryOperation
{
	public:
		Multiplication(): BinaryOperation(true, true) {symbol = '*'; identity = Number(1);};
		ExpressionNode simplify(ExpressionNode& left, ExpressionNode& right) const;
		bool isCompatible(const ExpressionNode&, const ExpressionNode&);
} extern const MULTIPLICATION;

extern const ChainOperation SUM;
extern const ChainOperation PRODUCT;

#endif
