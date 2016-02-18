/* treap.c
   Copyright (C) 1993 by Matthew Clegg.  All rights reserved.

   Implementation of treap data structure.
*/

#include <stdio.h>
#include "treap.h"

extern void free ();
extern void *malloc ();
extern long random ();

static void sift_upwards (Treap t, Treap_node *q)
/* On entry, t is an ordered tree, where the only node violating the
   property is q.  Moreover, the subtree rooted at q is a treap.
   Rotates q upwards until the treap property is restored for t.
*/
{
  Treap_node *p, *n;

  while ((q->parent != NULL) && (q->parent->priority < q->priority)) {
    p = q->parent;
    q->parent = p->parent;
    p->parent = q;

    if (q->parent != NULL) {
      if (q->parent->left == p)
	q->parent->left = q;
      else
	q->parent->right = q;
    } else
      *t = q;

    if (q == p->left) {  /* right rotation */
      n = q->right;
      q->right  = p;
      p->left   = n;
    } else { /* left rotation */
      n = q->left;
      q->left = p;
      p->right = n;
    }
    if (n != NULL) n->parent = p;
  }
}

treap_data_element Treap_Lookup 
  (Treap t, treap_comparator cmp, treap_key_type k)
/* Searches the treap for an element whose key matches k.
   If found, returns the corresponding data.  Otherwise, returns NULL.
*/
{
  Treap_node *p = *t;
  int c;  /* comparison value. */
  int new_priority; /* a new priority to be assigned to the treap element. */

  while (p != NULL) {
    if ((c = cmp(k, p->key)) < 0)
      p = p->left;
    else if (c > 0)
      p = p->right;
    else
      break;
  }

  if (p == NULL) return (NULL);

  /* We have found a matching item.  Now choose a new priority for it
     and move it closer to the top of the treap. */

  new_priority = random ();
  if (new_priority <= p->priority) return (p->data);

  p->priority = new_priority;
  sift_upwards (t, p);

  return (p->data);
}

/* After doing extensive timing trials, it appears that the implementation
   presented here sorts a list of 10,000 integers about 2 times slower
   than does unix quicksort.  It seems that the only way to get a
   substantial increase in speed would be to replace the comparison
   function by a macro, as below.  On beowulf (a Sun 4), the following
   average times were reported for sorting a list of 10,000 integers
   (100 trials were performed):

                         With Comparison       With Comparison
                                Function                 Macro
                         ---------------       ---------------
   Treap sort time                649 ms                574 ms
   Quick sort time                476 ms                491 ms
   Ratio                         1.36                  1.17

   Time to delete all
   keys from treap                562 ms                492 ms


   Here are the corresponding results from running the same simulations
   on dsl9 (a Sun IPX):

                         With Comparison       With Comparison
                                Function                 Macro
                         ---------------       ---------------
   Treap sort time                339 ms                307 ms
   Quick sort time                217 ms                216 ms
   Ratio                         1.56                  1.42

   Time to delete all
   keys from treap                283 ms                238 ms

*/

#define mcmp(x,y) \
  (((int) (x) < (int) (y))? -1: (((int) (x) == (int) (y))? 0: 1))

void Treap_Insert
  (Treap t, treap_comparator cmp, treap_key_type k, treap_data_element d)
/* Inserts the pair (k,d) into the treap t. */
{
  Treap_node *n = *t;    /* The current node we are examining in
				   our path down through the tree. */
  Treap_node *q = NULL;  /* The new node which we are inserting
				   into the tree. */

  int c = 0;  /* the result of the most recent comparison. */

  q = (Treap_node *) malloc (sizeof(Treap_node));
  q->left = q->right = q->parent = NULL;
  q->data = d;
  q->key  = k;
  q->priority = random ();

  /* If the treap is initially empty, then we treat this as a special
     case: */
  
  if (*t == NULL) {
    *t = q;
    return;
  }

  /* Now we search down through the treap, looking for where to insert
     our node: */
  while (n != NULL) {
    q->parent = n;
    c = cmp (k, n->key);
    n = (c < 0)? n->left: n->right;
  }

  if (c < 0)
    q->parent->left = q;
  else
    q->parent->right = q;

  /* At this point, q has been inserted into the tree as a leaf,
     and the ordering of the keys of the tree has been maintained.
     We now need to move q upwards through the tree in order to
     restore the heap order.
     */

  sift_upwards (t, q);
}

treap_data_element Treap_Delete 
  (Treap t, treap_comparator cmp, treap_key_type *k)
/* Searches the treap t for an element whose key matches *k.
   If found, deletes the element, sets *k equal to the key pointer in
   that that element, and returns the associated data.  If not found,
   returns NULL.
*/
{
  Treap_node *p = *t;   /* The node which we are deleting from the treap. */
  Treap_node *n, *q;    /* descendants of p. */

  int c;                /* result of most recent comparison. */
  treap_data_element d; /* data from the deleted node. */

  while (p != NULL) {
    c = cmp (*k, p->key);
    if (c < 0)
      p = p->left;
    else if (c > 0)
      p = p->right;
    else
      break;
  }

  if (p == NULL)  /* the requested node does not exist. */
    return NULL;

  /* At this point, p points to our node.  We must push it down to a 
     leaf, and then we can detach the leaf. */
  while ((p->left != NULL) && (p->right != NULL)) {
    if (p->left->priority > p->right->priority) { /* right rotation */
      q = p->left;
      n = q->right;
      q->right = p;
      p->left = n;
    } else { /* left rotation */
      q = p->right;
      n = q->left;
      q->left = p;
      p->right = n;
    }
    if (p->parent != NULL) {
      if (p->parent->left == p)
	p->parent->left = q;
      else
	p->parent->right = q;
    } else
      *t = q;
    q->parent = p->parent;
    p->parent = q;
    if (n != NULL) n->parent = p;
  }

  /* At this point, either p's left child is NULL or p's right child
     is NULL, so we can safely remove p from the tree. */
  if (p->left == NULL)
    q = p->right;
  else
    q = p->left;
  if (q != NULL) q->parent = p->parent;

  if (p->parent != NULL) {
    if (p == p->parent->left)
      p->parent->left = q;
    else
      p->parent->right = q;
  } else
    *t = q;

  /* p has now been removed from the tree.  we can now free the
     data structures associated to p.
  */
  *k = p->key;
  d = p->data;

  free (p);

  return d;
}

void Treap_Iterate (Treap t, treap_iterator it)
/* Applies the iterator to each node of the treap t.  If it ever returns
   a non-zero value, then terminates the iteration early.
*/
{
  Treap_node *p = *t;

  if (p == NULL)
    return;  /* an empty tree */

  while (p->left != NULL) p = p->left;

  do {
    if (it (p->key, p->data))
      return;

    if (p->right != NULL) {
      /* We have just visited an internal node, so we must visit the
	 right subtree of that node. */
      p = p->right;
      while (p->left != NULL) p = p->left;
    } else {
      /* We must back up to the last unvisited ancestor. */
      while ((p->parent != NULL) && (p == p->parent->right))
	p = p->parent;
      p = p->parent;
    }
  } while (p != NULL);
}

treap_data_element Treap_Lookup_First (Treap t, treap_key_type *k)
/* Returns the first element from the treap according to the treap order.
   Return a pointer to the corresponding data item and also a stores
   the key pointer in k.  Does not modify the treap.
*/
{
  Treap_node *n = *t;

  if (n == NULL)
    return (NULL);

  while (n->left != NULL) n = n->left;

  *k = n->key;
  return n->data;
}

treap_data_element Treap_Delete_First (Treap t, treap_key_type *k)
/* Deletes the first element from the treap according to the treap order.
   Return a pointer to the corresponding data item and also a stores
   the key pointer in k.
*/
{
  Treap_node *n = *t;
  Treap_node *p;
  treap_data_element d;

  if (n == NULL)
    return (NULL);

  while (n->left != NULL) n = n->left;
  p = n->parent;
  if (p != NULL) 
    p->left = n->right;
  else
    *t = n->right;
  if (n->right != NULL)
    n->right->parent = p;

  *k = n->key;
  d = n->data;
  free (n);

  return (d);
}
