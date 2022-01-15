# [TST (Ternary Search Tree)](https://en.wikipedia.org/wiki/Ternary_search_tree) 

A Ternary Search Tree is a tree data structure where each non-leaf node has up to three child nodes attached to it. 
Each node stores a character, an 'End of Word' bit, pointers to three children & a unique identifier. 
The pointer to the left child points to a node whose character value is less than the current node.
The pointer to the middle child points to a node whose character value is equal to the current node. 
The pointer to the right child points to a node whose character value is greater than the current node. 
Typical applications of Ternary Search Tree includes spell checking & auto-completion.

This code of TST is an excerpt from my Work Project. 
It served as a helper in implementing auto-complete & search on a list of up to 10 million objects.
