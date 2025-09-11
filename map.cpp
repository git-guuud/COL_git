template <typename K, typename V>
class HashMap {
public:
    HashMap(size_t size);
    ~HashMap();

    void insert(const K& key, const V& value);
    void get(const K& key, V& value) const;

private:
    struct HashNode {
        K key;
        V value;
        HashNode* next;
    };

    HashNode** table;
    size_t table_size;
    size_t hash(const K& key) const;
};
