=========================
AVL tree simpler examples
=========================

:Date: 2017-12-18

AVL named after Georgy Maximovich Adelson-Velsky and
Evgenii Mikhailovich Landis.


left-right
==========

B is inserted::

	| ________C
	| A____
	|     B
	'-.---.---.

after *Left rotation* on A::

	|     ____C
	| ____B
	| A
	'-.---.---.

after *Right rotation* on C::

	| ____B____
	| A       C
	|
	'-.---.---.


right-left
==========

B is inserted::

	| A________
	|     ____C
	|     B
	'-.---.---.

after *Right rotation* on C::

	| A____
	|     B____
	|         C
	'-.---.---.

after *Left rotation* on A::

	| ____B____
	| A       C
	|
	'-.---.---.
