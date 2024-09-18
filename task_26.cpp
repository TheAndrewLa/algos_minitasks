#include <vector>

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
    TreeNode* buildNode(std::vector<int>::const_iterator start, std::vector<int>::const_iterator end) {
        if (start == end)
            return new TreeNode{*start};

        auto mid = start + (std::distance(start, end) / 2);
        auto node = new TreeNode{*mid};
        
        node->left = buildNode(start, mid - 1);
        node->right = buildNode(mid + 1, end);
    }

public:
    void unwindBST(std::vector<int>& values, TreeNode* node) {
        if (node == nullptr)
            return;
        
        unwindBST(values, node->left);
        values.push_back(node->val);
        unwindBST(values, node->right);
    }

    TreeNode* buildBST(const std::vector<int>& values) {
        return buildNode(values.begin(), values.end() - 1);
    }

    TreeNode* balanceBST(TreeNode* root) {
        std::vector<int> vals;
        unwindBST(vals, root);
        return buildBST(vals);
    }
};
