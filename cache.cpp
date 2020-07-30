#include "dlist.h"
#include <iostream>
#include <sstream>

using namespace std;

class LRUCache {
private:
    int mem_size;
    int *memory;

    struct block {
        int address;
        int data;
    };

    Dlist<block> cache;
    int cur_cache_size;
    int max_cache_size;

    static bool compare(const block *a, const block *b);

public:
    LRUCache(int max_cache_size, int mem_size);

    ~LRUCache();

    int read(int address);

    void write(int address, int data);

    void printCache();

    void printMem();
};

bool LRUCache::compare(const block *a, const block *b) {
    return a->address == b->address;
}

LRUCache::LRUCache(int max_cache_size, int mem_size) {
    this->max_cache_size = max_cache_size;
    this->mem_size = mem_size;
}

LRUCache::~LRUCache() {
    if (mem_size > 0) {
        delete[] memory;
    }
}

int LRUCache::read(int address) {
    if (address >= mem_size) {
        throw -1;
    }
    bool hit = false;
    block *b = nullptr;

    for (int i = 0; i < cur_cache_size; i++) {
        block *ptr = cache.removeFront();
        if (ptr->address == address) {
            hit = true;
            b = ptr;
            continue;
        }
        cache.insertBack(ptr);
    }
    if (hit) {
        cache.insertFront(b);
        return b->data;
    } else {
        if (cur_cache_size == max_cache_size) {
            block *ptr = cache.removeBack();
            cur_cache_size--;
            memory[ptr->address] = ptr->data;
            delete ptr;
        }
        b = new block;
        b->address = address;
        b->data = memory[address];
        cache.insertFront(b);
        cur_cache_size++;
        return b->data;
    }
}

void LRUCache::write(int address, int data) {
    read(address);
    block *b = cache.removeFront();
    b->data = data;
    cache.insertFront(b);
}

void LRUCache::printCache() {
    Dlist<block> copy = cache;
    while (!copy.isEmpty()) {
        block *ptr = copy.removeFront();
        cout << ptr->address << " " << ptr->data << endl;
        delete ptr;
    }
}

void LRUCache::printMem() {
    for (int i = 0; i < mem_size; i++) {
        cout << memory[i] << " ";
    }
    cout << endl;
}

int main(int argc, const char *argv[]) {
    int max_cache_size, mem_size;
    cin >> max_cache_size >> mem_size;
    LRUCache cache(max_cache_size, mem_size);
    string command;
    getline(cin, command);
    while (getline(cin, command)) {
        istringstream Istream;
        Istream.str(command);

        string opr, addition;
        Istream >> opr;

        if (opr == "READ") {
            int address;
            bool flag = bool(Istream >> address);
            if (!flag) {
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            if (Istream >> addition) {
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            if (address >= mem_size) {
                cout << "ERROR: Address out of bound" << endl;
                continue;
            }
            cout << cache.read(address) << endl;
        } else if (opr == "WRITE") {
            int address, data;
            bool flag = bool(Istream >> address);
            if (!flag) {
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            flag = bool(Istream >> data);
            if (!flag) {
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            if (Istream >> addition) {
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            if (address >= mem_size) {
                cout << "ERROR: Address out of bound" << endl;
                continue;
            }
            cache.write(address, data);
        } else if (opr == "PRINTCACHE") {
            cache.printCache();
        } else if (opr == "PRINTMEM") {
            cache.printMem();
        } else if (opr == "EXIT") {
            break;
        } else {
            cout << "ERROR: Unknown instruction" << endl;
            continue;
        }
    }

    return 0;
}