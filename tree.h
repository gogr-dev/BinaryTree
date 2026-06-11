//-----------------------------------------------------------
// A binary search tree of baby-name records, keyed by name.
//-----------------------------------------------------------
#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

// One row of the names dataset.
struct NameRecord {
    std::string name;
    int count = 0;
    double percentage = 0.0;
    int rank = 0;
};

class BinaryTree {
public:
    BinaryTree() = default;
    ~BinaryTree() = default;                       // unique_ptr frees the whole tree

    BinaryTree(const BinaryTree& other);           // deep copy
    BinaryTree& operator=(const BinaryTree& other);
    BinaryTree(BinaryTree&&) noexcept = default;
    BinaryTree& operator=(BinaryTree&&) noexcept = default;

    // Insert a record. Returns false (and leaves the tree unchanged) if a record
    // with the same name already exists.
    bool insert(const NameRecord& record);

    // Find a record by exact name.
    std::optional<NameRecord> search(const std::string& name) const;

    // Remove a record by name. Returns false if the name was not found.
    bool remove(const std::string& name);

    std::size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    // All records in alphabetical order (in-order traversal).
    std::vector<NameRecord> inOrder() const;

private:
    struct Node {
        explicit Node(const NameRecord& r) : record(r) {}
        NameRecord record;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
    };

    static std::unique_ptr<Node> clone(const Node* node);
    static void collect(const Node* node, std::vector<NameRecord>& out);

    std::unique_ptr<Node> root_;
    std::size_t size_ = 0;
};
