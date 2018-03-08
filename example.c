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
 * 06/02/2018
 *
 * example of use
 *
 * AVL named after Georgy Maximovich Adelson-Velsky and
 * Evgenii Mikhailovich Landis.
 */

#include <stddef.h> /* offsetof() */

/* the AVL tree interface */
#include "avl_tree.h"

#include "example.h"

#ifndef container_of
#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
#endif

/*
 * The `struct avl_node_history` is used to store every node in
 * the search path. We use this history to update balance factors
 * and rebalance the tree after an insertion or deletion
 */

/* See how to use avl_search() in avl_[delete/insert]() */
struct example*
avl_search(struct avl_node_history *h, struct avl_root *root,
           unsigned long key)
{
	struct avl_node **current = &root->avl_node;

	tree_search_for_each (h, current) {
		struct example *tmp;

		tmp = container_of(*current, struct example, node);

		/* assign address of the left or right pointer to current */
		if (key > tmp->key)
			current = &(*current)->right;
		else if (key < tmp->key)
			current = &(*current)->left;
		else
			return tmp; /* node found */
	}

	return NULL;
}

int
avl_delete(struct avl_root *root, unsigned long key)
{
	struct avl_node_history history;

	if (avl_search(&history, root, key) == NULL)
		return -1; /* node not found */

	avl_do_delete(&history);

	return 0;
}

int
avl_insert(struct avl_root *root, struct example *new)
{
	struct avl_node_history history;

	if (avl_search(&history, root, new->key))
		return -1; /* node already exists */

	/* set up new node, insert and rebalance the candidate */
	avl_do_insert(&history, &new->node);

	return 0;
}
