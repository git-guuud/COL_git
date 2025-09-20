# Running the executable
To run the executable open the extracted directory in terminal.
Run 
`source build.sh || ./build.sh`

## Core File Operations

- **`CREATE <filename>`**  
  Creates a file with a root version (ID 0), empty content, and an initial snapshot message.  
  Note that the root is marked as a snapshot.

- **`READ <filename>`**  
  Displays the content of the file’s currently active version.

- **`INSERT <filename> <content>`**  
  Appends content to the file. This creates a new version if the active version is already a snapshot;  
  otherwise, it modifies the active version in place.

- **`UPDATE <filename> <content>`**  
  Replaces the file’s content. Follows the same versioning logic as **INSERT**.

- **`SNAPSHOT <filename> <message>`**  
  Marks the active version as a snapshot, making its content immutable.  
  It stores the provided message and the current time.

- **`ROLLBACK <filename> [versionID]`**  
  Sets the active version pointer to the specified **versionID**.  
  If no ID is provided, it rolls back to the parent of the current active version.

- **`HISTORY <filename>`**  
  Lists all snapshotted versions of the file chronologically, which lie on the path  
  from the active node to the root in the file tree, showing their ID, timestamp, and message.

---

## System-Wide Analytics

- **`RECENT_FILES [num]`**  
  Lists files in descending order of their last modification time, restricted to the first **num** entries.

- **`BIGGEST_TREES [num]`**  
  Lists files in descending order of their total version count, restricted to the first **num** entries.


## Input handling
The first step in handling user commands is to parse the input and identify the operation to be performed. 
<br>
Here, simply the first word of the input string is taken as the command. The next word as the target filename, and any subsequent words as additional parameters (content, versionID). If anything extra is present after that in the line, it is ignored.
<br>
A single command should be given in a single line only.




### Custom `unordered_map`
- Works with integer and string keys and type for values. 
- Implements a hash map with **separate chaining** for collisions. i.e. collisions are handled via making linked lists at each bucket.
- Uses **DJB2** hash for strings, identity hash for integers.
- **Operations**:
  - `insert(key, value)`
  - `find(key)` – Checks if a key exists.
  - `get(key)` – Retrieves the value for a key.
  - **Automatic rehashing** when load factor exceeds `0.69`.

---