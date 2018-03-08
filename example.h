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
 * 07/02/2018
 *
 * example of use
 *
 * AVL named after Georgy Maximovich Adelson-Velsky and
 * Evgenii Mikhailovich Landis.
 */

#ifndef AVL_EXAMPLE_H
#define AVL_EXAMPLE_H

#include "avl_tree.h"

struct example {
	struct avl_node node;
	unsigned long key;
};

struct example*
avl_search(struct avl_node_history *h, struct avl_root *root,
           unsigned long key);

int
avl_delete(struct avl_root *root, unsigned long key);

int
avl_insert(struct avl_root *root, struct example *new);

#endif /* AVL_EXAMPLE_H */
