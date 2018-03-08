/*
 * iterative (non-recursive) AVL tree
 * Copyright (C) 2018  Ricardo Biehl Pasquali <pasqualirb@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * 11/01/2018
 * iterative (non-recursive) AVL tree that uses balance factor and
 * doesn't have parent pointer
 *
 * read the README!
 *
 * AVL named after Georgy Maximovich Adelson-Velsky and
 * Evgenii Mikhailovich Landis.
 */

#include <stddef.h> /* NULL */

#include "avl_tree.h"

/*
 * node_history: register the nodes that might be affected
 * by the insertion/deletion. We use the history to update
 * balance factors and to rebalance the unbalanced node
 * (in insertion) or nodes (in deletion).
 */

/*
 * The node is imperfect when its balance factor is -1 or 1.
 * When calling is_imperfect() the node must not be unbalanced,
 * then we just check if balance factor is non-zero
 */
#define is_imperfect(node)  ((node)->balance)

/* The node is perfect when its balance factor is zero */
#define is_perfect(node)  (!(node)->balance)

/*
 * Rotations and balancing
 * =======================
 *
 * rotate_right(), rotate_left() and balance()
 */

/*
 * Rotation example (b->right = NULL)
 *   __c    __b__
 * __b   => a   c
 * a
 *
 * Nodes involved (before):
 * ____c
 * b__
 *   #
 *
 * Nodes involved (after):
 * b____
 *   __c
 *   #
 */
static inline struct avl_node*
rotate_right(struct avl_node *c)
{
	struct avl_node *b = c->left;

	c->left = b->right;
	b->right = c;

	/* return new root */
	return b;
}

/*
 * Rotation example (b->left = NULL)
 * a__      __b__
 *   b__ => a   c
 *     c
 *
 * Nodes involved (before):
 * a____
 *   __b
 *   #
 *
 * Nodes involved (after):
 * ____b
 * a__
 *   #
 */
static inline struct avl_node*
rotate_left(struct avl_node *a)
{
	struct avl_node *b = a->right;

	a->right = b->left;
	b->left = a;

	/* return new root */
	return b;
}

static void
balance(struct avl_node **candidate)
{
	struct avl_node *root = *candidate;
	struct avl_node *new_root;

	if (root->balance == -2) {
		struct avl_node *left_child = root->left;

		/* left-higher */

		if (left_child->balance == 1) {
			/* left child is right-higher */

			root->left = rotate_left(left_child);
			new_root = rotate_right(root);

			if (new_root->balance == -1) {
				left_child->balance = 0;
				root->balance = 1;
			} else if (new_root->balance == 0) {
				left_child->balance = 0;
				root->balance = 0;
			} else { /* new_root->balance == 1 */
				left_child->balance = -1;
				root->balance = 0;
			}

			new_root->balance = 0;
		} else {
			/* left child is left-higher */

			new_root = rotate_right(root);

			if (new_root->balance == 0) {
				/* only possible in deletion */
				new_root->balance = 1;
				root->balance = -1;
			} else {
				new_root->balance = 0;
				root->balance = 0;
			}
		}
	} else if (root->balance == 2) {
		struct avl_node *right_child = root->right;

		/* right-higher */

		if (right_child->balance == -1) {
			/* right child is left-higher */

			root->right = rotate_right(right_child);
			new_root = rotate_left(root);

			if (new_root->balance == 1) {
				right_child->balance = 0;
				root->balance = -1;
			} else if (new_root->balance == 0) {
				right_child->balance = 0;
				root->balance = 0;
			} else { /* new_root->balance == -1 */
				right_child->balance = 1;
				root->balance = 0;
			}

			new_root->balance = 0;
		} else {
			/* right child is right-higher */

			new_root = rotate_left(root);

			if (new_root->balance == 0) {
				/* only possible in deletion */
				new_root->balance = -1;
				root->balance = 1;
			} else {
				new_root->balance = 0;
				root->balance = 0;
			}
		}
	} else {
		/* no rebalance was done! */
		return;
	}

	/* update pointer in parent */
	*candidate = new_root;
}

/*
 * Deletion
 * ========
 *
 * delete_node(), deletion_balance() and avl_do_delete()
 */

/* delete the last node added to the history */
static inline void
delete_node(struct avl_node_history *h)
{
	struct avl_node **node = h->node[h->idx - 1];
	struct avl_node **substitute;

	if ((*node)->right == NULL) {
		*node = (*node)->left;
	} else {
		unsigned int tmp_index;

		/* the node's substitute will be its in-order successor */

		/*
		 * we do not add &(*node)->right to the history
		 * because it's a pointer to inside the deleted
		 * node. Instead, we simply save the history
		 * index and later we put &(*substitute)->right
		 * in this space.
		 */
		tmp_index = h->idx++;

		/* at the moment the right child is the substitute */
		substitute = &(*node)->right;

		/*
		 * if the loop runs the substitute becomes
		 * the leftmost node of the right child
		 */
		while ((*substitute)->left) {
			substitute = &(*substitute)->left;
			h->node[h->idx++] = substitute;
		}

		/* put &(*substitute)->right where history has lacked */
		h->node[tmp_index] = &(*substitute)->right;

		(*substitute)->left = (*node)->left;
		(*substitute)->balance = (*node)->balance;

		/* now substitute takes the place of the deleted node */
		if (*substitute == (*node)->right) {
			*node = *substitute;
		} else {
			struct avl_node *tmp = (*substitute)->right;

			(*substitute)->right = (*node)->right;
			*node = *substitute;
			*substitute = tmp;
		}
	}
}

/*
 * In the first run inside the loop `idx` refers to the parent of
 * last node changed. We go the tree up to rebalance what is necessary
 */
static inline void
deletion_balance(struct avl_node_history *h)
{
	struct avl_node **current;
	struct avl_node **child;

	/* skip child */
	h->idx--;

	while (h->idx--) {
		current = h->node[h->idx];
		child = h->node[h->idx + 1];

		/* update balance factor */
		if (&(*current)->left == child)
			(*current)->balance++;
		else
			(*current)->balance--;

		/* rebalance the node if it's unbalanced */
		balance(current);

		/*
		 * If current is imperfect (balance factor = -1 or 1)
		 * regardless of whether a rebalance has been done or
		 * not, we know the height of the tree keep the same
		 * and we're done!
		 */
		if (is_imperfect(*current))
			break;
	}
}

void
avl_do_delete(struct avl_node_history *h)
{
	/*
	 * The node to be deleted is the last
	 * node added in the history.
	 *
	 * delete_node() may insert nodes in history according
	 * to the path traveled to find the substitute to the
	 * deleted node.
	 *
	 * Again, the history is used to rebalance the tree,
	 * so every node touched must have its ancestors
	 * updated.
	 */

	/* delete the node from tree */
	delete_node(h);

	/* from node up, rebalance nodes that get unbalanced */
	deletion_balance(h);
}

/*
 * Insertion
 * =========
 *
 * insertion_balance() and avl_do_insert()
 */

/*
 * NOTE: In the insertion ...
 * The last imperfect node (balance factor = -1 or 1) found
 * during search is known as the candidate. It's the only
 * node that might become unbalanced, so it is the candidate
 * to be rebalanced :-)
 */

static inline void
insertion_balance(struct avl_node_history *h)
{
	struct avl_node **current;
	struct avl_node **child;

	/* skip new node */
	h->idx--;

	while (h->idx--) {
		current = h->node[h->idx];
		child = h->node[h->idx + 1];

		/* update balance factor */
		if (&(*current)->left == child)
			(*current)->balance--;
		else
			(*current)->balance++;

		/* rebalance the node if it's unbalanced */
		balance(current);

		/*
		 * If current is perfect (balance factor = 0)
		 * regardless of whether a rebalance has been done or
		 * not, we know the height of the tree keep the same
		 * and we're done!
		 */
		if (is_perfect(*current))
			break;
	}
}

/* set up new node, insert it, update balance factors and rebalance */
void
avl_do_insert(struct avl_node_history *h, struct avl_node *new)
{
	/* set up new node */
	new->left = NULL;
	new->right = NULL;
	new->balance = 0;

	/* insert the new node in tree */
	*h->node[h->idx - 1] = new;

	/* update the balance factor, and rebalance */
	insertion_balance(h);
}
