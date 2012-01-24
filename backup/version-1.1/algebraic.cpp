// Symbolic mathematics parser / manipulator

// assumes one character symbols/variables

#include <iostream>
#include "expression.hpp"
#include "operation.hpp"

using namespace std;

const Variable XVAR('x');
const Variable YVAR('y');

int main()
{
	
	
	ExpressionNode root(&ADDITION);
	ExpressionNode addnode(&ADDITION);
	ExpressionNode mulnode(&MULTIPLICATION);
	ExpressionNode xleaf(&XVAR);
	ExpressionNode twoleaf(2);
	ExpressionNode yleaf(&YVAR);
	
	//ExpressionNode root(&ADDITION);
	//ExpressionNode leaf(5);
	//ExpressionNode leaf2(7);
	
	root.appendChild(addnode);
	root.appendChild(xleaf);
	root.getFirstChild()->appendChild(addnode);
	root.getFirstChild()->appendChild(twoleaf);
	root.getFirstChild()->getFirstChild()->appendChild(mulnode);
	root.getFirstChild()->getFirstChild()->appendChild(yleaf);
	root.getFirstChild()->getFirstChild()->getFirstChild()->appendChild(xleaf);
	root.getFirstChild()->getFirstChild()->getFirstChild()->appendChild(twoleaf);
	
	cout << "before - root: " << root << endl;
	
	root.deepSimplify();
	
	cout << "deep simplify - root: " << root << endl;
	
	return 0;
}
