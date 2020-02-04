// File:        gridlist.cpp
// Date:        2020-01-18 00:11
// Description: Contains partial implementation of GridList class
//              CPSC 221 2019W2 PA1
//              Function bodies to be completed by yourselves
//
// ADD YOUR FUNCTION IMPLEMENTATIONS IN THIS FILE
//

#include "gridlist.h"
#include "gridlist_given.cpp"

// Creates a PNG of appropriate pixel dimensions according to the GridList's structure
//   and colours each pixel according the each GridNode's Block data.
// The fully coloured PNG is returned.
PNG GridList::Render() const
{
  Block nwBlock = northwest->data;
  int dimension = nwBlock.Dimension();
  PNG* im =  new PNG(dimx * dimension, dimy * dimension);

  GridNode* currNode = northwest;
  for (int i = 0; i < dimy; i++) {
    for (int j = 0; j < dimx; j++) {
      Block currBlock = currNode->data;
      currBlock.Render(*im, i * dimension, j * dimension);
      currNode = currNode->next;
    }
  }

  return *im;
}

// Allocates a new GridNode containing bdata and
//   attaches it to end of this list.
// Be careful of the special case of inserting into an empty list.
void GridList::InsertBack(const Block& bdata) {
  GridNode* newNode = new GridNode(bdata);
  if (northwest == NULL) {
    northwest = newNode;
    southeast = newNode;
    newNode->prev = NULL;
    newNode->next = NULL;
  } else {
    GridNode* prevNode = southeast;
    southeast->next = newNode;
    newNode->prev = southeast;
    newNode->next = NULL;
    southeast = newNode;
  }
}

// if this list has an odd number of column blocks, then the right side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two column blocks
//       inner list must have at least one column block
//       inner list must have the same vertical resolution, vertical block dimension, and block size
// POST: this list has nodes of the entire inner list in between the left and right halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_H(GridList& inner)
{
  if (dimy == inner.dimy && dimy >= 2 && inner.dimy >= 1) {
    GridNode* curr = northwest;
    GridNode* inNextRow = inner.northwest;
    GridNode* inCurr = inner.northwest;
    int midx = dimx / 2;

    for (int i = 0; i < dimy; i++) {
      for (int j = 0; j < dimx; j++) {
          if (j == midx - 1) {
            inCurr = inNextRow;
            GridNode* after = curr->next;
            curr->next = inCurr;
            inCurr->prev = curr;
            for (int k = 0; k < inner.dimx - 1; k++) {
              inCurr = inCurr->next;
            }
            inNextRow = inCurr->next;
            inCurr->next = after;
            after->prev = inCurr;
            curr = inCurr;
          }
          curr = curr->next;
      }
    }

    dimx += inner.dimx;
    inner.northwest = NULL;
    inner.southeast = NULL;
    // inner.northwest->next = NULL;
    // inner.southeast->prev = NULL;
    inner.dimx = 0;
    inner.dimy = 0;

  }
}



// inner list must have the same horizontal resolution, horizontal block dimension, and block size
// if this list has an odd number of row blocks, then the bottom side will have more blocks
// PRE:  this list and inner are not the same physical list
//       this list must have at least two row blocks
//       inner list must have at least one row block
//       inner list must have same horizontal resolution, horizontal block dimension, and block size
// POST: this list has nodes of the entire inner list in between the upper and lower halves of this list
//         and inner list becomes empty
//       otherwise both lists are not modified
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::Sandwich_V(GridList& inner)
{
  if (dimx == inner.dimx && dimx >= 2 && inner.dimx >= 1) {
    GridNode* curr = northwest;
    int midy = dimy / 2;

    for (int i = 0; i < dimy; i++) {
      for (int j = 0; j < dimx; j++) {
        curr = curr->next;
      }
      if (i == midy - 1) {
        GridNode* after = curr;
        curr = curr->prev;
        curr->next = inner.northwest;
        inner.northwest->prev = curr;
        inner.southeast->next = after;
        after->prev = inner.southeast;

      }
    }
    dimy += inner.dimy;
    inner.northwest = NULL;
    inner.southeast = NULL;
    // inner.northwest->next = NULL;
    // inner.southeast->prev = NULL;
    inner.dimx = 0;
    inner.dimy = 0;
  }
}


// PRE:  this list and otherlist have the same pixel dimensions, block dimensions, and block size
// POST: this list and otherlist are checkered with each other's nodes
//       each list's dimensions remain the same
// THIS MUST BE ACHIEVED USING POINTER REASSIGNMENTS.
// DO NOT ALLOCATE OR DELETE ANY NODES IN THIS FUNCTION.
void GridList::CheckerSwap(GridList& otherlist)
{
  CheckerSwapHelper(otherlist);
  otherlist.CheckerSwapHelper(*this);
}

void GridList::CheckerSwapHelper(GridList& otherlist)
{
  GridNode* currNodeThis = northwest;
  GridNode* currNodeOther = otherlist.northwest;

  for (int i = 0; i < dimy; i++) {
    GridNode* before = northwest;
    for (int j = 0; j < dimx; j++) {
      if (i % 2 == 0) {
        if (j % 2 == 1) {
          before = currNodeThis->prev;
          currNodeOther->prev = before;
          currNodeOther->next = currNodeThis->next;
          before->next = currNodeOther;
          currNodeThis = currNodeOther;
          if (currNodeThis->next != NULL) currNodeThis->next->prev = currNodeOther;
        }
      } else {
        if (j % 2 == 0) {
          before = currNodeThis->prev;
          currNodeOther->prev = before;
          currNodeOther->next = currNodeThis->next;
          before->next = currNodeOther;
          currNodeThis = currNodeOther;
          if (currNodeThis->next != NULL) currNodeThis->next->prev = currNodeOther;
        }
      }
      currNodeThis = currNodeThis->next;
      currNodeOther = currNodeOther->next;
    }
  }
}

// POST: this list has the negative effect applied selectively to GridNodes to form
//         a checkerboard pattern.
//       The northwest block is normal (does not have the negative effect applied).
// Ensure that the checkering looks correct for both odd and even horizontal block dimensions
void GridList::CheckerN()
{
  // enter your code here
  GridNode* currNode = northwest;
  for (int i = 0; i < dimy; i++) {
    for (int j = 0; j < dimx; j++) {
      if (i % 2 == 0) {
        if (j % 2 == 1) currNode->data.Negative();
      }
      else {
        if (j % 2 == 0) currNode->data.Negative();
      }
      currNode = currNode->next;
    }
  }
}

// Deallocates any dynamic memory associated with this list
//   and re-initializes this list to an empty state
void GridList::Clear()
{
  // enter your code here
}

// Allocates new nodes into this list as copies of all nodes from otherlist
void GridList::Copy(const GridList& otherlist)
{
  // enter your code here
}

// IMPLEMENT ANY PRIVATE FUNCTIONS YOU HAVE ADDED BELOW
//
//
