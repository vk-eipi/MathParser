#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <iostream>
#include <exception>
#include <string>

class Operation; // forward declaration

enum NodeType {NUMBER, VARIABLE, OPERATION};
class Number
{
	private:
		int value;
	public:
		Number();
		Number(int);
		int getInt() const;
		Number operator+ (const Number&);
		bool operator== (const Number&) const;
};
std::ostream& operator<<(std::ostream&, const Number&);

class Variable
{
	private:
		char symbol;
		Number value;
	public:
		Variable();
		Variable(char);
		void setSymbol(char);
		char getSymbol() const;
};
std::ostream& operator<<(std::ostream&, const Variable&);

class ExpressionNode
{	
	private:
		NodeType type;
		const Operation *operation;
		int activechildren; // activechildren == operation.arity
		ExpressionNode *right;
		ExpressionNode *firstChild;
		const Variable *variable;
		Number value;

	public:
		ExpressionNode();
		ExpressionNode(int);
		ExpressionNode(Number);
		ExpressionNode(const Operation* operation);
		ExpressionNode(const Variable*);
		ExpressionNode(const ExpressionNode & other); // copy, excl right
		~ExpressionNode();
		void init(const Operation* operation, ExpressionNode* firstChild);
		NodeType getType() const;
		void setType(NodeType);
		const Operation* getOperation() const;
		void setOperation(const Operation*);
		ExpressionNode* getRight() const;
		void setRight(const ExpressionNode*); // actually clears right, copies dereferenced obj
		//void addRight(ExpressionNode);
		ExpressionNode* getFirstChild() const; //as a pointer to leftmost child node
		void setFirstChild(const ExpressionNode*); // actually copies dereferenced obj, including its right
		//void addFirstChild(ExpressionNode);
		void appendChild(const ExpressionNode&);
		void deleteChildren();
		const Variable* getVariable() const;
		void setVariable(const Variable*);
		Number getValue() const;
		void setValue(Number);
		const ExpressionNode* findParentOf(const ExpressionNode& target) const;
		ExpressionNode* findParentOf(const ExpressionNode& target);
		bool operator== (const ExpressionNode&) const;
		//ExpressionNode copy() const; // also copies right
		ExpressionNode & operator= (const ExpressionNode &); // excludes right siblings
		void replace(const ExpressionNode&); // also excludes right siblings
		void remove(ExpressionNode&); // replace with identity element and simplify parent
		void simplify();
		void deepSimplify();
		class WrongArityError : public std::exception
		{
			private:
				std::string info;
			public:
				WrongArityError(): info("") {}
				WrongArityError(const char* txt): info(txt) {}
				~WrongArityError() throw() {}
				const char* what() const throw();
		};
		class TargetNotFoundError : public std::exception
		{
			public:
				const char* what() const throw();
		};
		class EmptyTreeError : public std::exception
		{
			public:
				const char* what() const throw();
		};
		class GenericError : public std::exception
		{
			private:
				const char* message;
			public:
				GenericError(const char * mess): message(mess) {}
				const char* what() const throw() {return message;}
		};
};
std::ostream& operator<<(std::ostream&, const ExpressionNode&);

#endif
