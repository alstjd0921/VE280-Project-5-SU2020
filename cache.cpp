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
    LRUCache(int cache_size, int memory_size);

    ~LRUCache();

    int read(int address);

    void write(int address, int data);

    void printCache();

    void printMem();
};

bool LRUCache::compare(const block *a, const block *b) {
    return a->address == b->address;
}

LRUCache::LRUCache(int cache_size, int memory_size) {
    max_cache_size = cache_size;
    mem_size = memory_size;
    cur_cache_size = 0;
    memory = new int[mem_size];
    for (int i = 0; i < mem_size; i++) {
        memory[i] = 0;
    }
}

LRUCache::~LRUCache() {
    if (mem_size > 0) delete[] memory;
}

int LRUCache::read(int address) {
    if (address >= mem_size) {
        throw -1;
    }

    bool flag = false;
    block *b = nullptr;
    for (int i = 0; i < cur_cache_size; i++) {
        block *temp = cache.removeFront();
        if (temp->address == address) {
            flag = true;
            b = temp;
            continue;
        }
        cache.insertBack(temp);
    }

    if (flag) {
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

int main() {
    int cache_size, memory_size;
    cin >> cache_size >> memory_size;
    LRUCache cache(cache_size, memory_size);
    string command;
    getline(cin, command);
    while (getline(cin, command)) {
        istringstream Istream;
        Istream.str(command);

        string opr;
        string addition;
        Istream >> opr;
        if (opr == "READ") {
            int address;
            bool judge = bool(Istream >> address);
            if (!judge) {
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            if (Istream >> addition) {
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            if (address >= memory_size) {
                cout << "ERROR: Address out of bound" << endl;
                continue;
            }
            cout << cache.read(address) << endl;
        } else if (opr == "WRITE") {
            int address, data;
            bool temp1 = bool(Istream >> address);
            if (!temp1) {
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            bool temp2 = bool(Istream >> data);
            if (!temp2) {
                cout << "ERROR: Not enough operands" << endl;
                continue;
            }
            if (Istream >> addition) {
                cout << "ERROR: Too many operands" << endl;
                continue;
            }
            if (address >= memory_size) {
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