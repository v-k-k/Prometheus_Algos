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

/**
 * @brief Recursively counts the number of "leaf" nodes in the tree
 * according to the specified criteria (non-zero data, no children).
 * @param node The current node being visited.
 * @param count_ptr A pointer to an integer that stores the cumulative count.
 */
void countQualifyingLeafNodes(struct BTreeNode *node, int *count_ptr);

/**
 * @brief Recursively collects the data of "leaf" nodes into an array.
 * @param node The current node being visited.
 * @param leafs_array The array to store the leaf node data.
 * @param current_idx_ptr A pointer to an integer that tracks the current insertion index in leafs_array.
 */
void collectQualifyingLeafValues(struct BTreeNode *node, int *leafs_array, int *current_idx_ptr);

/**
 * @brief Recursive helper function to find a single monotonic path with a given sum,
 * starting from the 'node' provided. It will attempt to extend the path downwards.
 *
 * @param node The current node in the traversal from the starting point.
 * @param target_sum The sum to find.
 * @param current_path_buffer Temporary array holding nodes of the path being built from the starting node.
 * @param path_len_ptr Pointer to current length of current_path_buffer.
 * @param max_tree_height Maximum possible depth of the tree (for current_path_buffer).
 * @param found_path_ptr_ptr A pointer to a pointer where the found path's array will be stored.
 * This is a global-like state indicating if the path has been found by any branch.
 * @param found_path_len_ptr A pointer to an integer where the length of the found path will be stored.
 * @return 1 if the path is found and copied (and stored in *found_path_ptr_ptr), 0 otherwise.
 * This allows early exit from recursion once the path is identified.
 */
int findPathFromCurrentNodeRecursive(struct BTreeNode* node, int target_sum,
                                     int* current_path_buffer, int* path_len_ptr,
                                     int max_tree_height,
                                     int** found_path_ptr_ptr, int* found_path_len_ptr);


/**
 * @brief Performs a Depth-First Search (DFS) on the entire tree. For each node visited,
 * it attempts to find a monotonic path starting from that node that sums to target_sum.
 * Stops as soon as the unique path is found.
 *
 * @param node The current node in the DFS traversal.
 * @param target_sum The sum to find.
 * @param current_path_buffer Shared temporary buffer for building paths.
 * @param max_tree_height Maximum tree height for buffer sizing.
 * @param found_path_ptr_ptr Pointer to the pointer that will store the result path.
 * @param found_path_len_ptr Pointer to store the length of the result path.
 */
void dfs_traverse_and_search(struct BTreeNode* node, int target_sum,
                             int* current_path_buffer, int max_tree_height,
                             int** found_path_ptr_ptr, int* found_path_len_ptr);


/**
 * @brief Finds the unique monotonic path (top-down) in the binary tree whose sum
 * of node values equals K.
 * Returns a dynamically allocated integer array containing the path values.
 * The caller is responsible for freeing this array using free().
 *
 * @param root The root of the binary tree.
 * @param K The target sum.
 * @param out_path_len A pointer to an integer where the length of the found path will be stored.
 * @return A dynamically allocated integer array representing the path, or NULL if no path is found.
 */
int* findUniqueMonotonicPath(struct BTreeNode* root, int K, int* out_path_len);

#endif