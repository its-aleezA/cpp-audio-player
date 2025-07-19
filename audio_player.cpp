//program for an audio player
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <shlobj.h>
#include <algorithm>
#include <random>
#include <string>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")

using namespace std;
namespace fs = std::filesystem;

//defining a node
template <typename T>
struct node
{
    T data;
    node<T>* nextNode;
    node<T>* previousNode;

    node()
    {
        data = T();
        nextNode = nullptr;
        previousNode = nullptr;
    }

    node(T val)
    {
        data = val;
        nextNode = nullptr;
        previousNode = nullptr;
    }
};

template <typename T>
class doublyLinked
{
    node<T>* tail, * head;
    int length;

    public:
    doublyLinked()
    {
        tail = nullptr;
        head = nullptr;
        length = 0;
    }

    doublyLinked(doublyLinked& dl)
    {
        head = nullptr;

        node<T>* temp = dl.head;
        while (temp)
        {
            insertAtEnd(temp->data);
            temp = temp->nextNode;
        }

        length = dl.length;
    }

    node<T>* getHead()
    {
        return head;
    }

    node<T>* getTail()
    {
        return tail;
    }

    void insertAtStart(T val)
    {
        node<T>* newNode = new node<T>(val);
        if (isEmpty())
        {
            head = newNode;
            tail = newNode;
            length++;
            return;
        }
        else
        {
            newNode->nextNode = head;
            head->previousNode = newNode;
            head = newNode;
            length++;
        }
    }

    void insertAtEnd(T val)
    {
        node<T>* newNode = new node<T>(val);
        if (isEmpty())
        {
            insertAtStart(val);
            return;
        }

        tail->nextNode = newNode;
        newNode->previousNode = tail;
        tail = newNode;

        length++;
    }

    void insertAtPosition(int n, T val)
    {
        node<T>* newNode = new node<T>(val);
        if (isEmpty() || n > length)
        {
            cout << "Invalid position" << endl;
            return;
        }
        if (n == 1)
        {
            insertAtStart(val);
            return;
        }
        if (n == length)
        {
            insertAtEnd(val);
            return;
        }

        node<T>* temp = head;

        for (int i = 1; i < n - 1; i++)
        {
            temp = temp->nextNode;
        }
        newNode->nextNode = temp->nextNode;
        newNode->previousNode = temp;
        temp->nextNode = newNode;
        newNode->nextNode->previousNode = newNode;

        length++;
    }

    void deleteFromStart()
    {
        //for empty list
        if (isEmpty())
        {
            cout << "the list is empty" << endl;
            return;
        }
        //for single element list
        if (head == tail)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
            length--;
            return;
        }
        //for multiple element list
        node<T>* temp = head;
        head = head->nextNode;
        delete temp;
        length--;
    }

    void deleteFromEnd()
    {
        //for empty list
        if (isEmpty())
        {
            cout << "the list is empty" << endl;
            return;
        }
        //for single element list
        if (head == tail)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
            length--;
            return;
        }
        //for multiple element list
        node<T>* temp = tail;
        tail = tail->previousNode;
        tail->nextNode = nullptr;
        delete temp;
        length--;
    }

    void deleteFromPosition(int n)
    {
        //for single element list
        if (head == tail)
        {
            delete head;
            head = nullptr;
            tail = nullptr;
            length--;
            return;
        }
        //for multiple element list
        if (isEmpty() || n > length)
        {
            cout << "Invalid position" << endl;
            length--;
            return;
        }

        //delete node at position n
        node<T>* temp = head;
        for (int i = 0; i < n - 2; i++)
        {
            temp = temp->nextNode;
        }
        node<T>* temp2 = temp->nextNode;
        temp->nextNode = temp2->nextNode;
        (temp2->nextNode)->previousNode = temp;
        delete temp2;

        length--;
    }

    void forwardDisplay()
    {
        node<T>* temp = head;

        cout << "\nForward Display: " << endl;
        cout << "There are " << length << " elements in the list" << endl;

        while (temp)
        {
            cout << temp->data << " ";
            temp = temp->nextNode;
        }
    }

    void backwardDisplay()
    {
        node<T>* temp = tail;

        cout << "\nBackward Display: " << endl;
        cout << "There are " << length << " elements in the list" << endl;

        while (temp)
        {
            cout << temp->data << " ";
            temp = temp->previousNode;
        }
    }

    bool isEmpty()
    {
        return head == nullptr;
    }

    ~doublyLinked()
    {
        while (head)
        {
            deleteFromStart();
        }
    }
};

// Static queue class for managing history
template <class T>
class staticQueue
{
    T* arr;
    int front, rear, capacity, length;

    public:
    staticQueue(int size = 0) : capacity(size), front(0), rear(-1), length(0)
    {
        arr = new T[capacity];
    }

    staticQueue(staticQueue& sq)
    {
        capacity = sq.capacity;
        arr = new T[capacity];
        for (int i = 0; i < length; i++)
        {
            enqueue(sq.arr[i]);
        }
        front = sq.front;
        rear = sq.rear;
        length = sq.length;
    }

    ~staticQueue()
    {
        delete[] arr;
    }

    void enqueue(T data)
    {
        if (isFull())
        {
            dequeue(); // Remove the oldest element if the queue is full
        }
        rear = (rear + 1) % capacity;
        arr[rear] = data;
        length++;
    }

    void dequeue()
    {
        if (isEmpty()) return;
        front = (front + 1) % capacity;
        length--;
    }

    T getFront()
    {
        if (isEmpty()) throw runtime_error("Queue is empty");
        return arr[front];
    }

    bool isEmpty()
    {
        return length == 0;
    }

    bool isFull()
    {
        return length == capacity;
    }

    void display()
    {
        for (int i = 0; i < length; i++)
        {
            cout << i+1 << ". " << arr[(front + i) % capacity] << endl;
        }
    }
};

// Class to represent audio tracks
class tracks
{
    string artist;
    string name;
    int duration;

    public:
    tracks(string n = "", string a = "", int d = 0) : artist(a), name(n), duration(d) {}
    void display()
    {
        cout << "Name: " << name << ", Duration: " << duration << ", Artist: " << artist << endl;
    }
    string getName()
    {
        return name;
    }
};
ostream& operator<< (ostream& os, tracks t)
{
    string name = t.getName();
    cout << "Playing " << fs::path(name).filename() << "..." << endl;
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, NULL, 0);
    wstring widePath(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, name.c_str(), -1, &widePath[0], size_needed);
    PlaySoundW(widePath.c_str(), NULL, SND_FILENAME);
    return os;
}

class playlist
{
    public:
    string name;
    doublyLinked<tracks> songs;
    staticQueue<string> history;

    playlist(string n = "") : name(n), history(5) {}

    void insertAtTail(tracks d) {
        songs.insertAtEnd(d);
        string name = d.getName();
        string songName = fs::path(name).filename().string();
        history.enqueue(songName);
    }

    void deleteAtHead() {
        songs.deleteFromStart();
    }

    void deleteAtAnyPosition(int p) {
        songs.deleteFromPosition(p);
    }

    void deleteAtTail() {
        songs.deleteFromEnd();
    }

    void traverse() {
        songs.forwardDisplay();
    }

    void shuffle() {
        // Create a vector of the playlist's nodes
        std::vector<node<tracks>*> nodes;
        node<tracks>* temp = songs.getHead();
        while (temp != nullptr) {
            nodes.push_back(temp);
            temp = temp->nextNode;
        }
        
        // Shuffle the vector
        std::random_device rd;  // a randm seed for the generator
        std::mt19937 gen(rd()); // a random number generator
        std::shuffle(nodes.begin(), nodes.end(), gen); // shuffle using the random generated number

        // Traverse the shuffled vector
        for (node<tracks>* node : nodes) {
            cout << "Playing: " << fs::path(node->data.getName().c_str()).filename() << "..." << endl;
            int size_needed = MultiByteToWideChar(CP_UTF8, 0, node->data.getName().c_str(), -1, NULL, 0);
            wstring widePath(size_needed, 0);
            MultiByteToWideChar(CP_UTF8, 0, node->data.getName().c_str(), -1, &widePath[0], size_needed);
            PlaySoundW(widePath.c_str(), NULL, SND_FILENAME);
        }
        cout << endl;
    }

    playlist(const playlist& l)
    {
        songs = l.songs;
        history = l.history;
        name = l.name;
    }

    bool isEmpty()
    {
        if (songs.getTail() == nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    string getName()
    {
        return name;
    }

    void DisplayHistory()
    {
        history.display();
    }
};

class AudioPlayer {
    string* songs = new string[10000]();
    int count = 0;
    int size = 0;
    fs::path dirSelected;

    public:
    playlist allPlaylists[5];
    AudioPlayer() {
        dirSelected = "C://";
    }

    void listenToPlaylist(int index) {
        if ((index < 1) || (index > 5))
        {
            cout << "Invalid playlist number!" << endl;
            return;
        }

        allPlaylists[index - 1].songs.forwardDisplay();

    }

    void ShuffleList()
    {
        allPlaylists[0].shuffle();
    }

    void displayHistory()
    {
        allPlaylists[0].DisplayHistory();
    }

    int getSize() {
        return size;
    }
    string* getSongs() {
        return songs;
    }
    int getCount() {
        return count;
    }

    playlist createPlaylist() {
            if (size == 0)
            {
                cout << "No songs available to create a playlist!" << endl;
                return playlist(); // Return an empty playlist
            }
        
            // Create new playlist
            string name;
            cout << "\nEnter name of your playlist: ";
            cin >> name;
        
            int* songChoices = new int[10000];
            cout << "\nWhich songs do you want to select?" << endl;
            cout << "Enter the corresponding number. Enter -1 to stop selecting." << endl;
        
            // Prompt user to select songs for playlist
            playlist p1(name);
            for (int i = 0;; i++) {
                cout << "Enter song number: ";
                cin >> songChoices[i];
        
                if (songChoices[i] == -1) {
                    break;
                }
                else if (songChoices[i] < 0 || songChoices[i] >= size) {
                    cout << "Invalid song number. Please try again." << endl;
                    i--;
                }
                else {
                    p1.insertAtTail(tracks(songs[songChoices[i]]));
                }
            }
            allPlaylists[count] = p1;
            count++;
        
            // Display playlist
            //while (true) {
                cout << "\nPlaylist created: " << name << endl;
                cout << "Songs in playlist: " << endl;
                node<tracks>* temp = p1.songs.getHead();
                int index = 1;
                while (temp != nullptr) {
                    cout << index << ". " << temp->data.getName() << endl;
                    temp = temp->nextNode;
                    index++;
                }
               cout << "\nShuffling the playlist:" << endl;
               ShuffleList();
               cout << "\nDisplaying the history of the playlist:" << endl;
               displayHistory();
               cout << "\nPlaying the playlist:" << endl;
               listenToPlaylist(1);

                // Prompting user to delete songs
                /*cout << "\nDo you want to delete any song from the playlist? (y/n): ";
                char choice;
                cin >> choice;
                if (choice == 'n') {
                    break;
                }
                else if (choice == 'y') {
                    cout << "Enter song number to delete: ";
                    int songToDelete;
                    cin >> songToDelete;
                    if (songToDelete < 1 || songToDelete > index - 1) {
                        cout << "Invalid song number. Please try again." << endl;
                    }
                    else {
                        p1.deleteAtAnyPosition(songToDelete - 1);
                    }
                }
                else {
                    cout << "Invalid choice. Please try again." << endl;
                }
            }*/
        
            delete[] songChoices;
        
            return p1;
        }

    void selectDirectory() {
        // Display dialog box to select a directory
        TCHAR szDir[MAX_PATH];
        BROWSEINFO bInfo = { 0 };
        bInfo.hwndOwner = NULL;
        bInfo.pidlRoot = NULL;
        bInfo.pszDisplayName = szDir;
        bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
        LPITEMIDLIST lpItem = SHBrowseForFolder(&bInfo);

        if (lpItem != NULL) {
            SHGetPathFromIDList(lpItem, szDir);

            // If TCHAR is wchar_t, convert to std::string
            #ifdef UNICODE
            wstring wselectedDir(szDir);
            string selectedDir(wselectedDir.begin(), wselectedDir.end()); // Convert wstring to string
            #else
            string selectedDir(szDir);
            #endif
            cout << "Selected Directory: " << selectedDir << endl;
            dirSelected = selectedDir; // Update the selected directory
        }
    }

    void countSongs() {
        if (dirSelected.empty()) {
            cout << "No directory selected!" << endl;
            return; // Exit if no directory is selected
        }

        size = 0; // Reset size before counting
        for (const auto& entry : std::filesystem::directory_iterator(dirSelected)) {
            size++;
        }

        // Create array of file paths
        int i = 0;
        for (const auto& entry : std::filesystem::directory_iterator(dirSelected)) {
            string pathStr = entry.path().string();
            songs[i] = pathStr;
            i++;
        }
    }

    void showSongs() {
        cout << "\nListing songs in directory:" << endl;
        for (int i = 0; i < size; i++) {
            cout << i + 1 << ". " << fs::path(songs[i]).filename() << endl;
        }
    }
};

int menu()
{
    int choice;
    cout << "\nSelect the operation you want to perform. Press:" << endl;
    cout << "1. Select directory" << endl;
    cout << "2. Create playlist" << endl;
    cout << "3. Listen to a playlist" << endl;
    cout << "4. Change directory" << endl;
    cout << "5. Shuffle songs" << endl;
    cout << "6. Display history" << endl;
    cout << "7. Exit Audio Player" << endl;
    cout << "Your choice: ";
    cin >> choice;

    return choice;
}

int main()
{
    int choice = 0;
    AudioPlayer a;

    while (choice != 7) {
        if (a.getCount() >= 5) {
            cout << "Maximum playlist capacity reached!" << endl;
            break;
        }

        choice = menu(); // Ensured to return valid choice (1-7)

        switch (choice) {
        case 1:
            a.selectDirectory();
            a.countSongs();
            a.showSongs();
            break;

        case 2:
            a.createPlaylist();
            break;

        case 3:
            if (a.getCount() == 0) {
                cout << "No playlist available! Create one now!" << endl;
                a.createPlaylist(); // Ensure playlist creation
            }
            else if (a.getCount() > 0) {
                    a.listenToPlaylist(1);
                }
            break;

        case 4:
            a.selectDirectory();
            //a.allPlaylists.clear(); // Safer way to clear playlists if it's a vector
            cout << "Directory changed! All playlists deleted." << endl;
            break;

        case 5:
            a.ShuffleList();
            cout << "Current Playlist is shuffling!" << endl;
            break;

        case 6:
            cout << "History: " << endl;
            a.displayHistory();
            break;

        case 7:
            cout << "Exiting program..." << endl;
            return 0; // Directly exit instead of breaking

        default:
            cout << "Invalid choice, try again.\n";
            break;
            }
        }

    return 0;
}
