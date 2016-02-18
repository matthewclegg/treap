Treap Data Structure

Copyright (C) 1993 by Matthew Clegg.  

This code is made available under the GNU Public License (version 3).

A treap is a randomly organized binary tree.  Because of its randomized
structure, it is highly likely to be of O(log n) depth, regardless of the
order in which the keys are inserted.  Associated to each node of the
treap is a randomly generated priority value.  With respect to the
priority values, the treap has a heap structure.  Thus, a treap combines
the features of both a tree and a heap.

For more information about treaps, see:

Aragon, C.R., Seidel, R.G., Randomized search trees,
in 30th Annual Symposium on Foundations of Computer Science

In this implementation, every element of the treap consists of
a key and data.  The keys are used for organizing the treap,
so that the pair (key, data) with smallest key is stored in
the leftmost node of the treap.

This implementation supports the operations:
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
