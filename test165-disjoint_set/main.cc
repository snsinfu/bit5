#include <cassert>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>


using std::size_t;


struct set_entry
{
    set_entry* parent = nullptr;
    size_t     size   = 0;
    int        value;
};

void       make(set_entry* e);
set_entry* find(set_entry* e);
void       merge(set_entry* e1, set_entry* e2);


int main()
{
    size_t const num_entries = 8096;

    std::vector<set_entry> entries(num_entries);
    std::vector<set_entry*> sets; // XXX: Borrowed pointer!

    for (size_t i = 0; i < num_entries; i++) {
        auto entry = &entries[i];
        entry->value = int(i);
        make(entry);
        sets.push_back(entry);
    }

    // Merge sets in a random-ish order.
    size_t const max_sets = 10;

    for (size_t i = 0; ; i++) {
        if (sets.size() <= max_sets) {
            break;
        }

        auto const a = (i + 1) * (i + 1) % sets.size();
        auto const b = (i * i / 2) % sets.size();
        if (a == b) {
            continue;
        }

        merge(sets[a], sets[b]);
        sets[a] = find(sets[a]);
        sets[b] = sets.back();
        sets.pop_back();
    }

    // Validate number of entries. Note that the disjoint set is implemented as
    // parent pointer trees, so we cannot directly enumerate members in given
    // set. The data structure ony supports efficient membership query.
    size_t num_seen = 0;
    for (size_t i = 0; i < sets.size(); i++) {
        num_seen += sets[i]->size;
        std::cout << "set #" << i << " contains " << sets[i]->size << " entries\n";
    }
    std::cout << num_seen << " entries in " << sets.size() << " sets\n";
    std::cout << '\n';

    assert(num_seen == num_entries);

    // Membership query. We can efficiently `find` the root of the containing
    // tree. However, finding out the entry in `sets` requires a linear scan.
    size_t const target = 42;
    auto const tree = find(&entries[target]);

    size_t set_index = 0;
    for (; set_index < sets.size(); set_index++) {
        if (sets[set_index] == tree) {
            break;
        }
    }

    std::cout << "entry #" << target << " is contained in set #" << set_index << "\n";

    // The disjoint set data structure is best suited for Kruskal's algorithm,
    // which does not require membership enumeration nor query.
}


void
make(set_entry* e)
{
    e->parent = e;
    e->size = 1;
}

set_entry*
find(set_entry* e)
{
    // Path compression.
    while (e->parent != e) {
        auto const parent = e->parent;
        e->parent = parent->parent;
        e = parent;
    }
    return e;
}

void
merge(set_entry* e1, set_entry* e2)
{
    auto tree1 = find(e1);
    auto tree2 = find(e2);

    if (tree1 == tree2) {
        return;
    }

    if (tree1->size < tree2->size) {
        auto tmp = tree1;
        tree1 = tree2;
        tree2 = tmp;
    }

    tree2->parent = tree1;
    tree1->size += tree2->size;
}
