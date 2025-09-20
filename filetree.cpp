#include <string>
#include <vector>
#include "map.cpp"
#include <ctime>
using namespace std;



struct TreeNode {
    int version_id;
    string content;
    string message;
    time_t created_timestamp;
    time_t snapshot_timestamp;
    TreeNode* parent;
    vector<TreeNode*> children;
};



struct File {
    TreeNode* root;
    TreeNode* active_version;
    unordered_map<int, TreeNode*> version_map;
    int total_versions;
    time_t last_modified;
};



template<typename T, typename C>
class FileHeap {
public:
    vector<T> files;
    int size = 0;

    C cmp;

    FileHeap(const C& c) : cmp(c) {
    }

    void insert(T file) {
        files.push_back(file);
        int curr = size;
        while (curr!=0) {
            int parent = (curr-1)/2;
            if (cmp(files[curr], files[parent])) {
                swap(files[curr], files[parent]);
                curr = parent;
            } 
            else break;
        }
        size++;
    }

    T pop_max() {
        if (size == 0) throw runtime_error("Heap is empty");

        T max_file = files[0];
        files[0] = files[size - 1];
        files.pop_back();
        size--;
        heapify(0);
        return max_file;
    }

    void heapify(int index) {
        int left = 2 * index + 1;
        if (left > size-1) return;
        else if (left == size-1) {
            if (cmp(files[left], files[index])) {
                swap(files[left], files[index]);
            }
        }
        else {
            int right = left + 1;
            int largest = cmp(files[left], files[right]) ? left : right;
            if (cmp(files[largest], files[index])) {
                swap(files[largest], files[index]);
                heapify(largest);
            }
        }
    }

    void build_heap() {
        for (int i = size / 2 - 1; i >= 0; i--) {
            heapify(i);
        }
    }

    vector<T> k_max(int k) {
        build_heap();
        vector<T> result;
        auto p_cmp = [this](const pair<int, T>& a, const pair<int, T>& b) {
            return cmp(a.second, b.second);
        };
        auto temp_heap = FileHeap<pair<int, T>, decltype(p_cmp)>(p_cmp);
        temp_heap.insert({0, files[0]});
        for (int i = 0; i < k && i < size; i++) {
            pair<int, T> p = temp_heap.pop_max();
            result.push_back(p.second);
            if (p.first*2+1 < size) 
                temp_heap.insert({p.first*2+1, files[p.first*2+1]});

            if (p.first*2+2 < size) 
                temp_heap.insert({p.first*2+2, files[p.first*2+2]});

        }
        return result;
    }
};




File* create_file(string filename) {
    File* new_file = new File();
    new_file->root = new TreeNode {
        0,
        "",
        filename,
        time(nullptr),
        time(nullptr),
        nullptr,
        {}
    };
    new_file->active_version = new_file->root;
    new_file->total_versions = 1;
    new_file->version_map.insert(0, new_file->root);
    new_file->last_modified = time(nullptr);
    return new_file;
}


void update_file(File* file, string content) {
    if (file == nullptr) return;

    TreeNode* version = file->active_version;
    if (version->snapshot_timestamp != NULL) {
        TreeNode* new_version = new TreeNode {
            file->total_versions,
            content,
            "",
            time(nullptr),
            NULL,
            version,
            {}
        };
        version->children.push_back(new_version);
        file->active_version = new_version;
        file->version_map.insert(file->total_versions, new_version);
        file->total_versions++;
    } else {
        version->content = content;
    }
    file->last_modified = time(nullptr);
}


void snapshot_file(File* file, string message) {
    if (file == nullptr) return;

    TreeNode* version = file->active_version;
    version->snapshot_timestamp = time(nullptr);
    version->message = message;
}


void rollback_file(File* file, int version_id) {
    if (version_id == -1) {
        file->active_version = file->active_version->parent;
    } else {
        file->active_version = file->version_map.get(version_id);
    }
}