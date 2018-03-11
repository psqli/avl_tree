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
 * 15/12/2017
 * iterative (non-recursive) AVL tree that uses balance factor and
 * doesn't have parent pointer
 *
 * read the README!
 *
 * AVL named after Georgy Maximovich Adelson-Velsky and
 * Evgenii Mikhailovich Landis.
 */

#ifndef AVL_TREE_H
#define AVL_TREE_H

struct avl_node {
	struct avl_node *left;
	struct avl_node *right;
	int balance;
};

struct avl_root {
	struct avl_node *avl_node;
};

#define AVL_ROOT  (struct avl_root) {NULL, }

#define AVL_HISTORY_MAX_HEIGHT  64

struct avl_node_history {
	struct avl_node **node[AVL_HISTORY_MAX_HEIGHT];
	unsigned int idx;
};

/*
 * DANGER: at the moment we are not checking whether a search
 * go beyond AVL_HISTORY_MAX_HEIGHT limit. It's possible to
 * cause a buffer overflow!
 * TODO: fix it efficiently!
 *
 * NOTE: it's possible to know the maximum possible (worst-case)
 * height for a given number of nodes. This way an implementation
 * can limit the number of elements.
 */

/* 'h' = node history */
#define tree_search_for_each(h, current) \
	for ((h)->idx = 0; *((h)->node[(h)->idx++] = current); )

void
avl_do_delete(struct avl_node_history *h);

void
avl_do_insert(struct avl_node_history *h, struct avl_node *new);

#endif /* AVL_TREE_H */
