/* treap.h
   Copyright (C) 1993 by Matthew Clegg.  All rights reserved.

   This header file is the interface to an implementation of the
   treap data structure.  Treaps are a randomized data structure for
   solving the dictionary problem.  For more information about
   them, see:

    Aragon, C.R., Seidel, R.G., Randomized search trees,
      in 30th Annual Symposium on Foundations of Computer Science

   In our implementation, every element of the treap consists of
   a key and data.  The keys are used for organizing the treap,
   so that the pair (key, data) with smallest key is stored in
   the leftmost node of the treap.

   Our implementation supports the operations:
     Lookup       -- searches the treap for a matching key 
     Insert       -- inserts a new (key, data) pair into the treap
     Delete       -- deletes a pair (key, data) based on the key
     Lookup_First -- returns the leftmost (key, data) pair
     Delete_First -- removes the leftmost (key, data) pair
     Iterate      -- applies a specifed iterator function to each pair

   The Treap data type is a pointer to a cell which contains the
   address of the root of the treap.  The reason for the use of
   this double indirection is that the root of the treap may change
   over time.  An empty treap can be initialized with the following
   statements:

     Treap_node *root = NULL;
     Treap      T     = &root;

   Thereafter, insert, delete, lookup, etc. operations may be 
   performed on the treap T.
*/

typedef void *treap_data_element;
  /* This data type is filled in by the user. */

typedef void *treap_key_type;
  /* The search key type, also filled in by the user. */

typedef int (*treap_comparator) (treap_key_type a, treap_key_type b);
  /* This routine is also supplied by the user.  
     It should return -1, 0, or 1 as to whether a < b, a == b, or a > b.
  */

typedef int (*treap_iterator) (treap_key_type k, treap_data_element d);

typedef struct Treap_node_struct {
  treap_key_type     *key;   /* the key associated to this node. */
  treap_data_element *data;  /* the data represented by this node. */
  struct Treap_node_struct *left, *right, *parent;  /* tree pointers */
  int priority;  /* a random value used for heap-ordering the tree */
} Treap_node, *Treap_node_ptr, **Treap;


treap_data_element Treap_Lookup 
  (Treap t, treap_comparator cmp, treap_key_type k);
/* Searches the treap for an element whose key matches k.
   If found, returns the corresponding data.  Otherwise, returns NULL.
*/

void Treap_Insert
  (Treap t, treap_comparator cmp, treap_key_type k, treap_data_element d);
/* Inserts the pair (k,d) into the treap t.  Assumes on entry that no
   node currently exists in the treap with key k. 
*/

treap_data_element Treap_Delete 
  (Treap t, treap_comparator cmp, treap_key_type *k);
/* Searches the treap t for an element whose key matches *k.
   If found, deletes the element, sets *k equal to the key pointer in
   that that element, and returns the associated data.  If not found,
   returns NULL.
*/


treap_data_element Treap_Lookup_First (Treap t, treap_key_type *k);
/* Returns the first element from the treap according to the treap order.
   Return a pointer to the corresponding data item and also a stores
   the key pointer in k.  Does not modify the treap.
*/

treap_data_element Treap_Delete_First (Treap t, treap_key_type *k);
/* Deletes the first element from the treap according to the treap order.
   Return a pointer to the corresponding data item and also a stores
   the key pointer in k.
*/

void Treap_Iterate (Treap t, treap_iterator it);
/* Applies the iterator to each node of the treap t.  If it ever returns
   a non-zero value, then terminates the iteration early.
*/
