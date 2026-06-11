//-----------------------------------------------------------
// Assertion-based tests for the BinaryTree class.
// Build and run with: make test
//-----------------------------------------------------------
#include "../tree.h"

#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {

int tests_run = 0;

void ok(bool condition, const std::string& label) {
    ++tests_run;
    if (!condition) {
        std::cerr << "FAIL: " << label << '\n';
        std::exit(1);
    }
    std::cout << "ok: " << label << '\n';
}

BinaryTree sampleTree() {
    BinaryTree tree;
    // Mixed insertion order so the tree has real shape (root M, branches both ways).
    tree.insert({"Mason", 100, 1.0, 1});
    tree.insert({"Ethan", 90, 0.9, 2});
    tree.insert({"Sam", 80, 0.8, 3});
    tree.insert({"Aiden", 70, 0.7, 4});
    tree.insert({"Liam", 60, 0.6, 5});
    tree.insert({"Noah", 50, 0.5, 6});
    tree.insert({"Tom", 40, 0.4, 7});
    return tree;
}

void testInsertAndSearch() {
    BinaryTree tree = sampleTree();
    ok(tree.size() == 7, "size after 7 inserts");

    auto found = tree.search("Liam");
    ok(found.has_value(), "search finds an inserted name");
    ok(found->count == 60 && found->percentage == 0.6 && found->rank == 5,
       "search returns the full record");
    ok(!tree.search("Zed").has_value(), "search misses an absent name");
}

void testDuplicateRejected() {
    BinaryTree tree = sampleTree();
    ok(!tree.insert({"Liam", 1, 0.1, 99}), "duplicate insert returns false");
    ok(tree.size() == 7, "duplicate insert does not grow the tree");
    ok(tree.search("Liam")->count == 60, "duplicate insert does not overwrite");
}

void testInOrderIsAlphabetical() {
    auto records = sampleTree().inOrder();
    ok(records.size() == 7, "inOrder returns every record");
    for (std::size_t i = 1; i < records.size(); ++i) {
        ok(records[i - 1].name < records[i].name,
           "inOrder sorted: " + records[i - 1].name + " < " + records[i].name);
    }
}

void testRemoveLeafAndSingleChild() {
    BinaryTree tree = sampleTree();
    ok(tree.remove("Tom"), "remove a leaf");
    ok(!tree.search("Tom").has_value(), "removed leaf is gone");

    ok(tree.remove("Sam"), "remove a node with one child");
    ok(!tree.search("Sam").has_value(), "removed node is gone");
    ok(tree.search("Noah").has_value() && tree.search("Liam").has_value(),
       "children survive a single-child removal");
    ok(tree.size() == 5, "size tracks removals");
}

void testRemoveTwoChildrenKeepsSuccessorData() {
    // Regression test: the original implementation copied only the successor's
    // NAME into the deleted slot, pairing it with the deleted node's stats.
    BinaryTree tree = sampleTree();
    ok(tree.remove("Mason"), "remove the root (two children)");
    ok(!tree.search("Mason").has_value(), "removed root is gone");

    auto successor = tree.search("Noah");  // in-order successor of Mason
    ok(successor.has_value(), "successor still findable after promotion");
    ok(successor->count == 50 && successor->percentage == 0.5 && successor->rank == 6,
       "successor keeps its OWN stats after promotion");
    ok(tree.size() == 6, "size after removing root");

    auto records = tree.inOrder();
    for (std::size_t i = 1; i < records.size(); ++i) {
        ok(records[i - 1].name < records[i].name, "still sorted after removal");
    }
}

void testRemoveMissing() {
    BinaryTree tree = sampleTree();
    ok(!tree.remove("Zed"), "removing an absent name returns false");
    ok(tree.size() == 7, "failed removal leaves size unchanged");
}

void testCopyIsDeepAndComplete() {
    // Regression test: the original copy helper dropped Percentage and Rank.
    BinaryTree original = sampleTree();
    BinaryTree copy = original;

    auto record = copy.search("Aiden");
    ok(record.has_value(), "copied tree contains the records");
    ok(record->count == 70 && record->percentage == 0.7 && record->rank == 4,
       "copy preserves the WHOLE record");
    ok(copy.size() == original.size(), "copy preserves size");

    copy.remove("Aiden");
    ok(original.search("Aiden").has_value(), "copy is deep: original unaffected");

    BinaryTree assigned;
    assigned.insert({"Zed", 1, 0.1, 999});
    assigned = original;  // the original class had no operator= (double-free risk)
    ok(assigned.size() == original.size(), "assignment replaces contents");
    ok(!assigned.search("Zed").has_value(), "assignment discards old contents");
}

void testLoadRealDataFile() {
    std::ifstream in("male.txt");
    if (!in) {
        std::cout << "skip: male.txt not found (run from repo root)\n";
        return;
    }
    BinaryTree tree;
    NameRecord r;
    std::size_t lines = 0;
    while (in >> r.name >> r.count >> r.percentage >> r.rank) {
        tree.insert(r);
        ++lines;
    }
    ok(lines == 1000, "male.txt parses fully (1000 rows)");
    ok(tree.size() == lines, "every row inserted exactly once (no duplicate names)");
    ok(tree.search("Liam")->rank == 1, "top-ranked name is found");
}

}  // namespace

int main() {
    testInsertAndSearch();
    testDuplicateRejected();
    testInOrderIsAlphabetical();
    testRemoveLeafAndSingleChild();
    testRemoveTwoChildrenKeepsSuccessorData();
    testRemoveMissing();
    testCopyIsDeepAndComplete();
    testLoadRealDataFile();
    std::cout << "\nALL " << tests_run << " ASSERTIONS PASSED\n";
    return 0;
}
