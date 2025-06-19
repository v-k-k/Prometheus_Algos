#ifndef BST_H
#define BST_H

#include "../../helpers/tools/utils.h"

/**
 * @brief Recursively builds a binary tree from a preorder traversal array.
 * '0' in the source array signifies a NULL child.
 * @param source_array The array containing the preorder traversal data.
 * @param array_size The total size of the source_array.
 * @param current_idx_ptr A pointer to an integer representing the current index
 * in the source_array. This allows the function to update the index for
 * subsequent recursive calls.
 * @return A pointer to the newly created BTreeNode, or NULL if no node is to be created.
 */
struct BTreeNode* BuildTreePreorder(const int *source_array, int array_size, int *current_idx_ptr);

void BsTreeInsert(int key);

void BTreeInorderToArray(struct BTreeNode *p, int *inorder_arr, int *index_ptr, int arr_capacity);

void BTreeInorderFromArray(struct BTreeNode *p, int *inorder_arr, int *index_ptr, int arr_capacity);

struct BTreeNode * BTreeSearch(int key);

struct BTreeNode *BTreeRInsert(struct BTreeNode *p,int key);

int BTreeHeight(struct BTreeNode *p);

struct BTreeNode *BTreeInPre(struct BTreeNode *p);

struct BTreeNode *BTreeInSucc(struct BTreeNode *p);

struct BTreeNode *BTreeDeleteKey(struct BTreeNode *p, int key);

// Function to free memory
void freeTree(struct BTreeNode* node);

/**
 * @brief Helper function to search for monotonic paths starting from 'node'
 * with a sum equal to target_sum. Increments total_paths_count.
 * @param node The current node in the path.
 * @param target_sum The desired sum for the path.
 * @param current_sum The sum of values encountered so far in the current path.
 * @param total_paths_count A pointer to an integer to store the total count of found paths.
 */
void countPathsFromNode(struct BTreeNode* node, int target_sum, int current_sum, int* total_paths_count);

/**
 * @brief Finds all monotonic paths (top-down) in the binary tree whose sum
 * of node values equals K. Returns the total count of such paths.
 * A path can start at any node.
 * @param root The root of the binary tree.
 * @param K The target sum.
 * @return The total number of monotonic paths found.
 */
int findMonotonicPathsCount(struct BTreeNode* root, int K);

#endif