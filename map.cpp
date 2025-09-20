#include <vector>
#include <string>

using namespace std;

size_t h(int key) {
    return (size_t)key;
}

//DJB2
size_t h(string key) {
    size_t hash = 5381;
    for (char c : key) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

template <typename K, typename V>
class unordered_map {
private:
    struct map_node {
        K key;
        V value;
        map_node* next;
    };

    size_t bucket_count = 16;
    size_t cur_size = 0;

    size_t hash(const K& key) {
        return h(key);
    }

public:
    vector<map_node*> data = vector<map_node*>(bucket_count, nullptr);

    void insert(const K& key, const V& value) {
        size_t index = hash(key) % bucket_count;
        map_node* new_node = new map_node{key, value, nullptr};
        if (!data[index]) {
            data[index] = new_node;
        } else {
            map_node* current = data[index];
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
        cur_size++;
        if ((double)cur_size / bucket_count > 0.69) {
            rehash(bucket_count * 2);
        }
    }

    bool find(const K& key) {
        size_t index = hash(key) % bucket_count;
        map_node* current = data[index];
        while (current) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    V get(const K& key) {
        size_t index = hash(key) % bucket_count;
        map_node* current = data[index];
        while (current) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        throw std::runtime_error("Key not found");
    }

    void rehash(size_t new_bucket_count) {
        vector<map_node*> new_data(new_bucket_count, nullptr);
        for (size_t i = 0; i < bucket_count; ++i) {
            map_node* current = data[i];
            while (current) {
                size_t new_index = h(current->key) % new_bucket_count;
                map_node* next = current->next;
                current->next = new_data[new_index];
                new_data[new_index] = current;
                current = next;
            }
        }
        data = std::move(new_data);
        bucket_count = new_bucket_count;
    }
};
