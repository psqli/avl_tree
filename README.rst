=================================
iterative AVL tree implementation
=================================

:Date: 2018-01-28

AVL named after Georgy Maximovich Adelson-Velsky and
Evgenii Mikhailovich Landis.


How to use
==========

See example.c :-)


What is an AVL tree?
====================

Read ``00_avl_tree.rst`` in Documentation/


About the implementation
========================

There are some choices when implementing AVL trees:

1. recursive or iterative
2. store balance factor or height
3. store parent or not

At the moment, here we implement an iterative (non-recursive) AVL
tree that uses balance factor and doesn't have parent pointer.

Also, it is an intrusive data structure, meaning that the node
structure must be embedded inside the object.

If you want an implementation that have parent pointer, I strongly
recommend visiting <https://github.com/ebiggers/avl_tree>. It has
an excellent performance and the developer (Eric Biggers) has
contributed to the Linux kernel [1]_.

The code in this repo was based entirely on libavl
<http://adtinfo.org>. I've also read and picked up some pieces
from the Eric's implementation.

.. [1] See
   <https://patchwork.kernel.org/project/linux-fsdevel/list/?submitter=90951>
