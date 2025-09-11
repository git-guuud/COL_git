#include <iostream>
#include "filetree.cpp"


using namespace std;

int main() {
    unordered_map<string, File*> open_files;

    auto ver_cmp = [](File* a, File* b) {
        return a->active_version->version_id > b->active_version->version_id;
    };

    FileHeap<File*, decltype(ver_cmp)> ver_heap(ver_cmp);

    auto mod_cmp = [](File* a, File* b) {
        return a->last_modified > b->last_modified;
    };

    FileHeap<File*, decltype(mod_cmp)> mod_heap(mod_cmp);

    cout << "CLI running..." << endl; 
    while (1) {
        string cmd, filename, content;
        int version_id = -1;
        cin >> cmd;
        while (cin.peek() == ' ') cin.ignore();

        if (cmd == "RECENT_FILES" || cmd == "BIGGEST_TREES") 
        {
            int k;
            if (!(cin >> k) || k<0) {
                cout << "Invalid num. Please enter a positive integer." << endl;
                cin.clear();
                cin.ignore(100000, '\n');
                continue;
            }

            if (cmd == "RECENT_FILES") {
                vector<File*> recent_files = mod_heap.k_max(k);
                for (File* f : recent_files) {
                    cout << ctime(&f->last_modified) << f->root->message << endl;
                }
            } else if (cmd == "BIGGEST_TREES") {
                vector<File*> biggest_trees = ver_heap.k_max(k);
                for (File* f : biggest_trees) {
                    cout << f->total_versions << " " << f->root->message << endl;
                }
            }
            continue;
        }
        

        if (cin.peek() == '\n') {
            cout << "No file name provided" << endl;
            continue;
        }
        cin >> filename;
        while (cin.peek() == ' ') cin.ignore();
        if (cin.peek() != '\n') {
            if (cmd == "ROLLBACK") {
                if (!(cin >> version_id)) {
                    cout << "Invalid version ID. Please enter an integer." << endl;
                    cin.clear();
                    cin.ignore(100000, '\n');
                    continue;
                }
            } else {
                getline(cin, content);
            }
        }

        
        if (cmd == "CREATE") {
            if (open_files.find(filename) == open_files.end()) {
                open_files[filename] = create_file(filename);
                cout << "File created: " << filename << endl;
                ver_heap.insert(open_files[filename]);
                mod_heap.insert(open_files[filename]);
            } else {
                cout << "ERROR! File already exists: " << filename << endl;
            }
            continue;
        }
        
        File* file = nullptr;
        if (open_files.find(filename) != open_files.end()) {
            file = open_files[filename];
        } else {
            cout << "ERROR! File not found: " << filename << endl;
            continue;
        }
        

        if (cmd == "READ") {
            cout << file->active_version->content << endl;
        }

        if (cmd == "INSERT") {
            update_file(file, file->active_version->content + content);

        }

        if (cmd == "UPDATE") {
            update_file(file, content);
        }

        if (cmd == "SNAPSHOT") {
            snapshot_file(file, content);
        }

        if (cmd == "ROLLBACK") {
            if (version_id == -1) {
                if (file->active_version->parent == nullptr) {
                    cout << "No previous version to rollback to." << endl;
                    continue;
                }
            } else if (file->version_map.find(version_id) == file->version_map.end()) {
                cout << "Version ID not found: " << version_id << endl;
                continue;
            }
            rollback_file(file, version_id);
        }

        if (cmd == "HISTORY") {
            TreeNode* version = file->active_version;
            while (version != nullptr) {
                cout << "Version ID: " << version->version_id
                << ", Message: " << version->message
                << ", Snapshot Time: " << ctime(&version->snapshot_timestamp);
                version = version->parent;
            }
        }
    }
}