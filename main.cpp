//-----------------------------------------------------------
// Interactive lookup over the most common baby names, backed
// by the BinaryTree class.
//-----------------------------------------------------------
#include "tree.h"

#include <cctype>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

namespace {

// Load "Name Count Percentage Rank" rows into a tree. Returns false on I/O failure.
bool loadFile(const std::string& path, BinaryTree& tree) {
    std::ifstream in(path);
    if (!in) {
        return false;
    }
    NameRecord record;
    while (in >> record.name >> record.count >> record.percentage >> record.rank) {
        tree.insert(record);
    }
    return true;
}

// Normalize a query to the dataset's "Xxxx" capitalization.
std::string normalize(std::string name) {
    for (auto& ch : name) {
        ch = static_cast<char>(std::tolower(static_cast<unsigned char>(ch)));
    }
    if (!name.empty()) {
        name[0] = static_cast<char>(std::toupper(static_cast<unsigned char>(name[0])));
    }
    return name;
}

void printRecord(const NameRecord& r) {
    std::cout << "  name:       " << r.name << '\n'
              << "  rank:       " << r.rank << '\n'
              << "  count:      " << r.count << '\n'
              << "  percentage: " << r.percentage << "%\n";
}

void printHelp() {
    std::cout << "Commands:\n"
                 "  <name>   look up a name (case-insensitive)\n"
                 "  list     print every name alphabetically\n"
                 "  help     show this help\n"
                 "  quit     exit\n";
}

}  // namespace

int main(int argc, char* argv[]) {
    const std::string path = argc > 1 ? argv[1] : "male.txt";

    BinaryTree tree;
    if (!loadFile(path, tree)) {
        std::cerr << "error: could not open " << path << '\n';
        return 1;
    }
    std::cout << "Loaded " << tree.size() << " names from " << path << ".\n"
              << "Type a name to look it up, or 'help' for commands.\n";

    std::string line;
    while (std::cout << "> " << std::flush, std::getline(std::cin, line)) {
        std::istringstream words(line);
        std::string word;
        if (!(words >> word)) {
            continue;  // blank line
        }

        const std::string query = normalize(word);
        if (query == "Quit" || query == "Exit") {
            break;
        }
        if (query == "Help") {
            printHelp();
        } else if (query == "List") {
            for (const auto& record : tree.inOrder()) {
                std::cout << std::left << std::setw(12) << record.name
                          << " rank " << std::setw(4) << record.rank
                          << " count " << std::setw(6) << record.count
                          << ' ' << record.percentage << "%\n";
            }
        } else if (auto record = tree.search(query)) {
            printRecord(*record);
        } else {
            std::cout << "'" << query << "' not found\n";
        }
    }
    return 0;
}
