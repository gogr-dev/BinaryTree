//-----------------------------------------------------------
// Implementation of the BinaryTree class.
//-----------------------------------------------------------
#include "tree.h"

#include <utility>

//-----------------------------------------------------------
// Copying
//-----------------------------------------------------------

std::unique_ptr<BinaryTree::Node> BinaryTree::clone(const Node* node) {
    if (node == nullptr) {
        return nullptr;
    }
    auto copy = std::make_unique<Node>(node->record);  // copies the whole record
    copy->left = clone(node->left.get());
    copy->right = clone(node->right.get());
    return copy;
}

BinaryTree::BinaryTree(const BinaryTree& other)
    : root_(clone(other.root_.get())), size_(other.size_) {}

BinaryTree& BinaryTree::operator=(const BinaryTree& other) {
    if (this != &other) {
        root_ = clone(other.root_.get());
        size_ = other.size_;
    }
    return *this;
}

//-----------------------------------------------------------
// Insert
//-----------------------------------------------------------

bool BinaryTree::insert(const NameRecord& record) {
    std::unique_ptr<Node>* current = &root_;
    while (*current != nullptr) {
        if (record.name == (*current)->record.name) {
            return false;  // duplicate names are rejected, not silently doubled
        }
        current = record.name < (*current)->record.name ? &(*current)->left
                                                        : &(*current)->right;
    }
    *current = std::make_unique<Node>(record);
    ++size_;
    return true;
}

//-----------------------------------------------------------
// Search
//-----------------------------------------------------------

std::optional<NameRecord> BinaryTree::search(const std::string& name) const {
    const Node* current = root_.get();
    while (current != nullptr) {
        if (name == current->record.name) {
            return current->record;
        }
        current = name < current->record.name ? current->left.get()
                                              : current->right.get();
    }
    return std::nullopt;
}

//-----------------------------------------------------------
// Remove
//-----------------------------------------------------------

bool BinaryTree::remove(const std::string& name) {
    std::unique_ptr<Node>* current = &root_;
    while (*current != nullptr && (*current)->record.name != name) {
        current = name < (*current)->record.name ? &(*current)->left
                                                 : &(*current)->right;
    }
    if (*current == nullptr) {
        return false;
    }

    Node* node = current->get();
    if (node->left == nullptr) {
        *current = std::move(node->right);          // 0 children or right-only
    } else if (node->right == nullptr) {
        *current = std::move(node->left);           // left-only
    } else {
        // Two children: replace with the in-order successor (leftmost node of the
        // right subtree), copying the WHOLE record, then unlink the successor.
        std::unique_ptr<Node>* successor = &node->right;
        while ((*successor)->left != nullptr) {
            successor = &(*successor)->left;
        }
        node->record = (*successor)->record;
        *successor = std::move((*successor)->right);
    }
    --size_;
    return true;
}

//-----------------------------------------------------------
// Traversal
//-----------------------------------------------------------

void BinaryTree::collect(const Node* node, std::vector<NameRecord>& out) {
    if (node != nullptr) {
        collect(node->left.get(), out);
        out.push_back(node->record);
        collect(node->right.get(), out);
    }
}

std::vector<NameRecord> BinaryTree::inOrder() const {
    std::vector<NameRecord> out;
    out.reserve(size_);
    collect(root_.get(), out);
    return out;
}
