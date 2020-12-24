/**
 * @file rgbtree.cpp
 * Implementation of rgbtree class.
 */

#include <utility>
#include <algorithm>
#include "rgbtree.h"

using namespace std;

rgbtree::rgbtree(const map<RGBAPixel,string>& photos)
{

/* your code here! */
  for (auto & key_val: photos) {
    tree.push_back(key_val.first);
  }
  buildTree(0,tree.size() - 1,0);


}

RGBAPixel rgbtree::findNearestNeighbor(const RGBAPixel & query) const
{

/* your code here! */
  return findNearestHelper(query,0,tree.size()-1,0);
}

bool rgbtree::smallerByDim(const RGBAPixel & first,
                                const RGBAPixel & second, int curDim) const
{

/* your code here! */
  int d = curDim % 3;
  switch (d) {
    case 0:
      if (first.r == second.r) return (first<second);
      return first.r < second.r;
    case 1:
      if (first.g == second.g) return (first<second);
      return first.g < second.g;
    case 2:
      if (first.b == second.b) return (first<second);
      return first.b < second.b;
  }
  return false;
}

/**
 * This function splits the trees[start..end] subarray at position start k
 */
void rgbtree::quickSelect(int start, int end, int k, int d)
{
  
/* your code here! */
  if ((k < start) || (k > end)) return;
  if (start >= end) return;
  int p = partition(start,end,d);
  if (k == p) return;
  if (k < p) {
    quickSelect(start,p-1,k,d);
  } else {
    quickSelect(p+1,end,k,d); 
  } 
}


/**
 * This method does a partition around pivot and will be used 
 * in quick select. It uses tree[lo] as the default pivot.
 * It returns the index of the pivot in the updated vector.
 * You will likely need to modify and complete this code.
 */
int rgbtree::partition(int lo, int hi, int d) 
{
    // this is the partition code from HW2 it doesn't work!!
    int p = lo;
    for( int i=lo+1; i <= hi; i++ ) {
      if(smallerByDim(tree[i],tree[lo],d)) { 
        p++; 
        swap(tree[i],tree[p]);
      }
    }
    swap(tree[lo],tree[p]);
    return p;

}


/**
 * Helper function to help determine if the nearest neighbor could 
 * be on the other side of the KD tree.
 */
int rgbtree::distToSplit(const RGBAPixel& query, const RGBAPixel& curr, int dimension) const
{

/* your code here! */
  int d = dimension % 3;
  switch (d) {
    case 0:
      return (query.r - curr.r) * (query.r - curr.r);
    case 1:
      return (query.g - curr.g) * (query.g - curr.g);
    case 2:
      return (query.b - curr.b) * (query.b - curr.b);
  }
  return -1;
}

/* buildTree
 * Helper function to split the subtree in dimension d in the middle, 
 * nodes with smaller value in dimension d is in the left part. Nodes with greater value in 
 * dimension d is in the right part.
 */
void rgbtree::buildTree(int start, int end, int d) {
  if (start >= end) return;
  quickSelect(start,end,(start+end)/2,d);
  buildTree(start,(start+end)/2 - 1, d+1);
  buildTree((start+end)/2 + 1,end,d+1);
}

/* findNearestHelper
 * helper function to help find nearest neighbour
 */
RGBAPixel rgbtree::findNearestHelper(const RGBAPixel & query,int start, int end, int dimension) const {
  if (start >= end) return tree[start];
  RGBAPixel rootMin = tree[(start + end) /2];
  if (smallerByDim(query,rootMin,dimension)) {
    RGBAPixel inMin = findNearestHelper(query,start,(start+end)/2 - 1,dimension + 1);
    bool needOutMin = distToSplit(query,rootMin,dimension) < distance(query,inMin);
    if (needOutMin) {
      RGBAPixel outMin = findNearestHelper(query,(start+end)/2 + 1,end,dimension + 1);
      if (distance(query,outMin) < distance(query,inMin)) inMin = outMin;
    }
    if (distance(query,rootMin) < distance(query,inMin)) return rootMin;
    return inMin;
  }
  else 
  {
    RGBAPixel inMin = findNearestHelper(query,(start+end)/2 + 1,end,dimension + 1);
    bool needOutMin = distToSplit(query,rootMin,dimension) < distance(query,inMin);
    if (needOutMin) {
      RGBAPixel outMin = findNearestHelper(query,start,(start+end)/2 - 1,dimension + 1);
      if (distance(query,outMin) < distance(query,inMin)) inMin = outMin;
    }
    if (distance(query,rootMin) < distance(query,inMin)) return rootMin;
    return inMin;
  }
  
  


}

/* distance
 * calculate squared distance between two RGBAPixel using three colors
 */
int rgbtree::distance(const RGBAPixel &first, const RGBAPixel & second) const {
  return (first.r - second.r) * (first.r - second.r) + (first.g - second.g) * (first.g - second.g) + (first.b - second.b) * (first.b - second.b);

}

