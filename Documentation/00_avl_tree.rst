==============
AVL tree guide
==============

:Date: 2017-12-18

AVL named after Georgy Maximovich Adelson-Velsky and
Evgenii Mikhailovich Landis.


Before start
============

* Read files in order.
* Take a paper and a pen and draw things :-)

An useful interactive online tool to visualize AVL tree:
<https://www.cs.usfca.edu/~galles/visualization/AVLtree.html>


General tree rules
==================

* Every node is represented by a number.
* Every node has two child nodes.
* Smaller numbers go left child.
* Bigger numbers go right child.
* A nonexistent node (number) is NULL.


What makes an AVL tree
======================

For **every** node, the high difference between left and right side
cannot be greater than 1.

e.g.:

Left side differs from right side by 1 [**Ok**]::

	__2
	1

Right side differs from left side by 2 [**Wrong**]::

	0__
	  1__
	    2

Both sides are equal [**Ok**]::

	__1__
	0   2


Drawing a tree in plain text
============================

A nice way to represent a tree is by positioning nodes
proportional to its numbers (like a graph)::

	|  ____2________
	|  1       ____4____
	|          3       5
	'--.---.---.---.---.
	   1   2   3   4   5


Rotations
=========

Rotations are necessary to balance the tree. See `Balancing`_ section.


Left rotation
-------------

e.g.::

	| A____
	|     B____
	|         C
	'-.---.---.

* A takes B's left child as its right child (NULL in this case)
* B takes A as its left child
* the parent of A becomes parent of B

after rotation::

	| ____B____
	| A       C
	|
	'-.---.---.


Right rotation
--------------

e.g.::

	|     ____C
	| ____B
	| A
	'-.---.---.

* C takes B's right child as its left child (NULL in this case)
* B takes C as its right child
* the parent of C becomes parent of B

after rotation::

	| ____B____
	| A       C
	|
	'-.---.---.


Balancing
=========

Balancing is necessary when a node has one of its children 2 levels
higher than the other. This happens only after an Insertion or
Deletion (see `Tree operations`_ section).

The four balancing cases:


The right child is right-higher
-------------------------------

Do a `Left rotation`_.


The left child is left-higher
-----------------------------

Do a `Right rotation`_.


The right child is left-higher
------------------------------

e.g.::

	| A________
	|     ____C
	|     B
	'-.---.---.

1. Do a `Right rotation`_ on the right node (C this case).
2. Do a `Left rotation`_.


The left child is right-higher
------------------------------

e.g.::

	| ________C
	| A____
	|     B
	'-.---.---.

1. Do a `Left rotation`_ on the left node (A this case).
2. Do a `Right rotation`_.


Tree operations
===============

+---------+-------------------------------+
|Search   | No balance is done            |
+---------+-------------------------------+
|Insertion| At most one balance is done   |
+---------+-------------------------------+
|Deletion | Multiple balances may be done |
+---------+-------------------------------+


Once you've read this
=====================

Look at examples in ``01_avl_simpler_examples.rst`` file.


References
==========

* HARGROVE, John; The AVL Tree Rotations Tutorial; 2007
  <https://www.cise.ufl.edu/~nemo/cop3530/AVL-Tree-Rotations.pdf>
