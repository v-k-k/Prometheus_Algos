#include "bst.h"

// Actual DEFINITION of the global root pointer.
// This provides the storage for 'root'. ONLY ONE .c/.cpp FILE MUST HAVE THIS.
struct BTreeNode *root = NULL;

struct BTreeNode* BuildTreePreorder(const int *source_array, int array_size, int *current_idx_ptr) {
    // Check if the current index is within bounds and if the value is not the sentinel '0'
    if (*current_idx_ptr >= array_size || source_array[*current_idx_ptr] == 0) {
        (*current_idx_ptr)++; // Increment index to move past the '0' or out-of-bounds marker
        return NULL;
    }

    // Allocate memory for a new node
    struct BTreeNode* newNode = (struct BTreeNode*)malloc(sizeof(struct BTreeNode));
    if (newNode == NULL) {
        // Handle memory allocation failure
        fprintf(stderr, "Error: Memory allocation failed for new node!\n");
        exit(EXIT_FAILURE); // Exit the program on severe error
    }

    // Assign data from the source_array to the new node
    newNode->data = source_array[*current_idx_ptr];
    (*current_idx_ptr)++; // Increment index for the next element

    // Recursively build the left child, passing all necessary parameters
    newNode->lchild = BuildTreePreorder(source_array, array_size, current_idx_ptr);
    // Recursively build the right child, passing all necessary parameters
    newNode->rchild = BuildTreePreorder(source_array, array_size, current_idx_ptr);

    return newNode;
}

void BsTreeInsert(int key)
 {
    struct BTreeNode *t = root;
    struct BTreeNode *r = NULL, *p;
    
    if(root == NULL)
    {
        p = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
        p->data = key;
        p->lchild = p->rchild = NULL;
        root = p;
        return;
    }
    while(t != NULL)
    {
        r = t;
        if(key < t->data)
            t = t->lchild;
        else if(key > t->data)
            t = t->rchild;
        else
            return;
    }
    p = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
    p->data = key;
    p->lchild = p->rchild = NULL;
    
    if(key < r->data) r->lchild = p;
    else r->rchild = p;
 }

 void BTreeInorderFromArray(struct BTreeNode *p, int *inorder_arr, int *index_ptr, int arr_capacity) {
    if (p) { // Base case: if 'p' is NULL, do nothing and return. Correct.
        // 1. Visit Left Child: Recurse all the way down the left subtree first.
        BTreeInorderFromArray(p->lchild, inorder_arr, index_ptr, arr_capacity);

        // 2. Visit Root (Assign Data): This is where the magic happens for sorted arrays.
        // The current node 'p' is the *next* node in the inorder sequence.
        // So, it should receive the *next* value from the sorted array.
        // The (p != NULL) check is redundant here because of the outer 'if (p)'.
        if (*index_ptr < arr_capacity) {
            p->data = inorder_arr[*index_ptr]; // Assign data from the sorted array
            (*index_ptr)++; // Move to the next element in the sorted array
        } else {
            // This is a crucial check. If the tree has more nodes than the array has
            // elements (or if '0's were skipped), we need to handle it.
            fprintf(stderr, "Warning: Inorder array capacity exceeded or insufficient for tree nodes. Not all elements saved.\n");
        }

        // 3. Visit Right Child: After processing the current node, recurse on the right.
        BTreeInorderFromArray(p->rchild, inorder_arr, index_ptr, arr_capacity);
    }
}

 void BTreeInorderToArray(struct BTreeNode *p, int *inorder_arr, int *index_ptr, int arr_capacity) {
    if (p) {
        // 1. Visit Left Child
        BTreeInorderToArray(p->lchild, inorder_arr, index_ptr, arr_capacity);

        // 2. Visit Root (Save Data)
        if (*index_ptr < arr_capacity) {
            inorder_arr[*index_ptr] = p->data;
            (*index_ptr)++; // Increment the index for the next element
        } else {
            // Optional: Handle array full case (e.g., print warning, resize if dynamic)
            // For a fixed-size array, this just means we stop saving.
            fprintf(stderr, "Warning: Inorder array capacity exceeded. Not all elements saved.\n");
        }


        // 3. Visit Right Child
        BTreeInorderToArray(p->rchild, inorder_arr, index_ptr, arr_capacity);
    }
 }

 struct BTreeNode * BTreeSearch(int key)
 {
    struct BTreeNode *t = root;
    
    while(t != NULL)
    {
        if(key == t->data)
            return t;
        else if(key < t->data)
            t = t->lchild;
        else
            t = t->rchild;
    }
    return NULL;
 }

 struct BTreeNode *BTreeRInsert(struct BTreeNode *p,int key)
 {
    struct BTreeNode *t = NULL;
    
    if(p == NULL)
    {
        t = (struct BTreeNode *)malloc(sizeof(struct BTreeNode));
        t->data = key;
        t->lchild = t->rchild = NULL;
        return t;
    }
    if(key < p->data)
        p->lchild = BTreeRInsert(p->lchild,key);
    else if(key > p->data)
        p->rchild = BTreeRInsert(p->rchild,key);
    
    return p;        
}

 int BTreeHeight(struct BTreeNode *p)
 {
    int x, y;
    if(p == NULL)return 0;
    x = BTreeHeight(p->lchild);
    y = BTreeHeight(p->rchild);
    return x > y ? x+1 : y+1;
 }

 struct BTreeNode *BTreeInPre(struct BTreeNode *p)
 {
    while(p && p->rchild != NULL)
        p = p->rchild;
    
    return p;
 }

 struct BTreeNode *BTreeInSucc(struct BTreeNode *p)
 {
    while(p && p->lchild != NULL)
        p = p->lchild;
    
    return p;
 }

 void freeTree(struct BTreeNode* node) {
    if (node == NULL) {
        return;
    }
    freeTree(node->lchild);
    freeTree(node->rchild);
    free(node);
}

struct BTreeNode *BsTreeDeleteKey(struct BTreeNode *p, int key)
 {
    struct BTreeNode *q;
    
    if(p == NULL)
        return NULL;
    if(p->lchild == NULL && p->rchild == NULL)
    {
        if(p == root)
            root = NULL;
        free(p);
        return NULL;
        
    }
    
    if(key < p->data)
        p->lchild = BsTreeDeleteKey(p->lchild, key);
    else if(key > p->data)
        p->rchild = BsTreeDeleteKey(p->rchild, key);
    else
    {
        if(BTreeHeight(p->lchild) > BTreeHeight(p->rchild))
        {
            q = BTreeInPre(p->lchild);
            p->data = q->data;
            p->lchild = BsTreeDeleteKey(p->lchild, q->data);
        }
        else
        {
            q = BTreeInSucc(p->rchild);
            p->data = q->data;
            p->rchild = BsTreeDeleteKey(p->rchild, q->data);
        }          
    }
    return p;
}

void countPathsFromNode(struct BTreeNode* node, int target_sum, int current_sum, int* total_paths_count) {
    if (node == NULL) {
        return;
    }

    current_sum += node->data;

    // If the current sum equals the target, we found a path
    if (current_sum == target_sum) {
        (*total_paths_count)++;
    }

    // Recursively call for left and right children to continue the current path
    countPathsFromNode(node->lchild, target_sum, current_sum, total_paths_count);
    countPathsFromNode(node->rchild, target_sum, current_sum, total_paths_count);
}

int findMonotonicPathsCount(struct BTreeNode* root, int K) {
    if (root == NULL) {
        return 0;
    }

    int total_paths_count = 0; // Initialize counter

    // For each node in the tree (starting from root),
    // consider it as the potential beginning of a new path.
    // This is the "outer" traversal (e.g., preorder traversal of tree nodes).

    // 1. Check if the root node itself forms a path
    if (root->data == K) {
        total_paths_count++;
    }

    // 2. Find paths starting from the current 'root' (including multi-node paths)
    //    Pass a pointer to total_paths_count so inner recursion can update it.
    countPathsFromNode(root->lchild, K, root->data, &total_paths_count); // Path starts with root->data
    countPathsFromNode(root->rchild, K, root->data, &total_paths_count); // Path starts with root->data

    // 3. Recursively call for left and right subtrees
    //    This ensures that every node in the tree is considered as a potential
    //    starting point for a new path.
    total_paths_count += findMonotonicPathsCount(root->lchild, K);
    total_paths_count += findMonotonicPathsCount(root->rchild, K);

    return total_paths_count;
}

void countQualifyingLeafNodes(struct BTreeNode *node, int *count_ptr) {
    if (node == NULL) return;

    // A node qualifies as a "leaf" if it has no children AND its data is not 0.
    // This directly translates the C# 'node.Left.Value == 0 && node.Right.Value == 0'
    // as an indication of NULL children, and 'node.Value != 0' as an explicit filter.
    if (node->lchild == NULL && node->rchild == NULL && node->data != 0)
        (*count_ptr)++;

    // Recursively visit children
    countQualifyingLeafNodes(node->lchild, count_ptr);
    countQualifyingLeafNodes(node->rchild, count_ptr);
}

void collectQualifyingLeafValues(struct BTreeNode *node, int *leafs_array, int *current_idx_ptr) {
    if (node == NULL) return;

    // Check the same leaf criteria as in the counting phase
    if (node->lchild == NULL && node->rchild == NULL && node->data != 0) {
        leafs_array[*current_idx_ptr] = node->data;
        (*current_idx_ptr)++;
    }

    // Recursively visit children
    collectQualifyingLeafValues(node->lchild, leafs_array, current_idx_ptr);
    collectQualifyingLeafValues(node->rchild, leafs_array, current_idx_ptr);
}

int findPathFromCurrentNodeRecursive(struct BTreeNode* node, int target_sum,
                                     int* current_path_buffer, int* path_len_ptr,
                                     int max_tree_height,
                                     int** found_path_ptr_ptr, int* found_path_len_ptr) {
    // If a path has already been found globally, or current node is NULL, stop this branch.
    if (*found_path_ptr_ptr != NULL || node == NULL) {
        return 0; // Indicate that no new path was found in this sub-branch.
    }

    // Add current node to the temporary path buffer
    if (*path_len_ptr >= max_tree_height) {
        fprintf(stderr, "Path buffer overflow! Increase max_tree_height.\n");
        exit(EXIT_FAILURE); // Or handle more gracefully
    }
    current_path_buffer[*path_len_ptr] = node->data;
    (*path_len_ptr)++;

    // Calculate current sum of the path being built
    // Use long long to prevent potential overflow during sum accumulation for very long paths.
    long long current_sum_val = 0;
    for (int i = 0; i < *path_len_ptr; i++) {
        current_sum_val += current_path_buffer[i];
    }

    // If the current sum equals the target, we found the unique path
    if (current_sum_val == target_sum) {
        // Allocate memory for the found path
        *found_path_ptr_ptr = (int*)malloc(*path_len_ptr * sizeof(int));
        if (*found_path_ptr_ptr == NULL) {
            fprintf(stderr, "Memory allocation failed for found path array!\n");
            exit(EXIT_FAILURE);
        }
        // Copy the path values
        memcpy(*found_path_ptr_ptr, current_path_buffer, *path_len_ptr * sizeof(int));
        *found_path_len_ptr = *path_len_ptr;
        (*path_len_ptr)--; // Backtrack for the current node before returning
        return 1; // Indicate that the path has been found
    }

    // Recursively call for left and right children to continue the current path
    // If path is found in a recursive call, propagate the 'found' status.
    if (findPathFromCurrentNodeRecursive(node->lchild, target_sum,
                                         current_path_buffer, path_len_ptr,
                                         max_tree_height,
                                         found_path_ptr_ptr, found_path_len_ptr)) {
        (*path_len_ptr)--; // Backtrack for the current node
        return 1;
    }
    if (findPathFromCurrentNodeRecursive(node->rchild, target_sum,
                                         current_path_buffer, path_len_ptr,
                                         max_tree_height,
                                         found_path_ptr_ptr, found_path_len_ptr)) {
        (*path_len_ptr)--; // Backtrack for the current node
        return 1;
    }

    // Backtrack: remove current node from the temporary path buffer
    (*path_len_ptr)--;
    return 0; // Path not found in this specific continuation
}

void dfs_traverse_and_search(struct BTreeNode* node, int target_sum,
                             int* current_path_buffer, int max_tree_height,
                             int** found_path_ptr_ptr, int* found_path_len_ptr) {
    // If a path has already been found globally, or current node is NULL, stop.
    if (node == NULL || *found_path_ptr_ptr != NULL) {
        return;
    }

    // Attempt to find a path STARTING from 'node'.
    // A new path search starts, so its temporary length starts at 0 for this call.
    int path_segment_len = 0; // This variable is local to this dfs_traverse_and_search frame.
    findPathFromCurrentNodeRecursive(node, target_sum,
                                     current_path_buffer, &path_segment_len,
                                     max_tree_height,
                                     found_path_ptr_ptr, found_path_len_ptr);

    // If a path was found starting from 'node' (or its descendants via findPathFromCurrentNodeRecursive),
    // then we are done with the global search.
    if (*found_path_ptr_ptr != NULL) {
        return;
    }

    // If no path was found starting from 'node', continue the DFS
    // by considering its children as NEW potential starting points for other paths.
    dfs_traverse_and_search(node->lchild, target_sum,
                            current_path_buffer, max_tree_height,
                            found_path_ptr_ptr, found_path_len_ptr);

    // Check again after left subtree search, in case the path was found there.
    if (*found_path_ptr_ptr != NULL) {
        return;
    }

    dfs_traverse_and_search(node->rchild, target_sum,
                            current_path_buffer, max_tree_height,
                            found_path_ptr_ptr, found_path_len_ptr);
}

int* findUniqueMonotonicPath(struct BTreeNode* root, int K, int* out_path_len) {
    *out_path_len = 0; // Initialize length to 0

    if (root == NULL) {
        return NULL;
    }

    // Maximum possible height of the tree. A skewed tree can have height N.
    // Using a fixed size for simplicity. For very deep trees, consider dynamic resizing
    // or calculating actual height first.
    int max_tree_height = 1000;
    int* current_path_buffer = (int*)malloc(max_tree_height * sizeof(int));
    if (current_path_buffer == NULL) {
        fprintf(stderr, "Memory allocation failed for current_path_buffer!\n");
        exit(EXIT_FAILURE);
    }

    int* found_path = NULL; // This will hold the pointer to the dynamically allocated result path
    int found_len = 0;      // This will hold the length of the result path

    // Start the main DFS traversal, which will initiate sub-searches from each node.
    // &found_path and &found_len are passed so the recursive functions can modify them directly.
    dfs_traverse_and_search(root, K, current_path_buffer, max_tree_height, &found_path, &found_len);

    free(current_path_buffer); // Free the temporary buffer used during the search
    *out_path_len = found_len; // Set the output path length
    return found_path;         // Return the found path (or NULL)
}
