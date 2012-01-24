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
	
	//root.appendChild(addnode);
	//root.appendChild(twoleaf);
	//root.getFirstChild()->appendChild(xleaf);
	//root.getFirstChild()->appendChild(twoleaf);
	
	root.appendChild(addnode);
	root.appendChild(xleaf);
	root.getFirstChild()->appendChild(addnode);
	root.getFirstChild()->appendChild(addnode);
	root.getFirstChild()->getFirstChild()->appendChild(xleaf);
	root.getFirstChild()->getFirstChild()->appendChild(twoleaf);
	root.getFirstChild()->getFirstChild()->getRight()->appendChild(xleaf);
	root.getFirstChild()->getFirstChild()->getRight()->appendChild(twoleaf);
	
	//ExpressionNode root(&ADDITION);
	//ExpressionNode leaf(&XVAR);
	//ExpressionNode leaf2(&XVAR);
	//root.appendChild(leaf);
	//root.appendChild(leaf2);
	
	cout << "before - root: " << root << endl;
	
	root.deepSimplify();
	
	cout << "deep simplify - root: " << root << endl;
	
	return 0;
}
