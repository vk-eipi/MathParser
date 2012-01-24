// Symbolic mathematics parser / manipulator

// assumes one character symbols/variables

#include <iostream>
#include "expression.hpp"
#include "operation.hpp"

using namespace std;

int main()
{
	Variable XVAR('x');
	
	ExpressionNode root(&ADDITION);
	ExpressionNode leaf(&XVAR);
	ExpressionNode leaf2(&XVAR);
	
	//ExpressionNode root(&ADDITION);
	//ExpressionNode leaf(5);
	//ExpressionNode leaf2(7);
	
	cout << "leaf: " << leaf << endl;
	cout << "leaf2: " << leaf2 << endl;
	//cout << "default rootvalue: " << root.getValue().getInt() << endl;
	cout << "root: " << root << endl;
	
	root.setFirstChild(&leaf);
	root.getFirstChild()->setRight(&leaf2);
	
	cout << "root: " << root << endl;
	
	root.simplify();
	
	cout << "after - root: " << root << endl;
	
	return 0;
}
