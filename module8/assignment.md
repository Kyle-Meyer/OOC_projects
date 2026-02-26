Design a set of classes that can represent arithmetic expressions. That is, trees where the
interior nodes are arithmetic operations, and the leaf nodes are either variables or
constants 

Provide the following operations: (where node_ptr is a std::shared_ptr<Node>)

Hand construct an expression tree. You are not expected to build a parser! Code
the tree construction directly with something like

```
node_ptr node = new Add(new Mul(new Constant(2.3), new Variable(“Xray”) ),
new Mul( new Variable(“Yellow”),
new Sub(new Variable(“Zebra”), new Variable(“Xray”))
)
);
```

Output an expression tree (extra parenthesis are OK).

```
std::cout << node; // Output: (((2.3)*(Xray))+((Yellow)*((Zebra)–(Xray))))
```

Evaluate an expression tree (where Xray = 2.0; Yellow = 3.0; and Zebra = 5.0 )

```
double d = node->evaluate(); // d = 13.6
```

Store the current value of variables in a symbol table. Do not store a copy of the value
in the Variable node. You will have to construct a table that allows you to store and
retrieve the value associated with the name of each variable. You can use a map:
```
std::map<std::string, double>
```

Create the derivative with respect to a particular variable: for example

```
node_ptr dx = node->derivative(“Xray”);
```

For this part of the exercise, treat derivation as a new tree construction. There is no
need to remember calculus to do this problem! Just apply the following
recursive rules

| e                        | e.derivative(“Xray”)    |
| ------------------------ | ----------------------- |
| + u                      | du                      |
| - u                      | - du                    |
| u + v                    | du + dv                 |
| u - v                    | du - dv                 |
| u * v                    | u  *  dv + v  *  du<br> |
| u / v                    | (v*du - u*dv) / v*v     |
| variable Xray            | 1.0                     |
| variable other than Xray | 0.0                     |
| constant                 | 0.0                     |


where

```
du = u->derivative(“Xray”) and dv = v->derivative(“Xray”)
```

Output the derivatives expression tree and its value (maintaining the same values for Xray,
Yellow and Zebra)

```
std::cout << dx;
// Output:((2.3*1.0) + (Xray*0.0)) + ((Yellow*(0.0-1.0)) + ((Zebra-Xray)*0.0))
double d = dx->evaluate(); // Result: d = -0.7
```


Note: Because a tree and its derivative share common subtrees, you must use shared
pointers: std::shared_ptr<Node>. Just “shallow” copying raw pointers (Node*) will
result in two expression trees containing common subtrees. If you delete one tree, the
other tree will contain pointers to subtrees that have been deleted. (dangling pointers).
If you delete both trees the common subtrees will be deleted twice! Not good!
