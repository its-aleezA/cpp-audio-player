//program for an audio player
#include <iostream>
#include <Windows.h>
#include <filesystem>
#include <shlobj.h>
#include <algorithm>
#include <random>
#include <string>
#include <mmsystem.h>
#include <vector>
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

    node() : data(T()), nextNode(nullptr), previousNode(nullptr) {}
    node(T val) : data(val), nextNode(nullptr), previousNode(nullptr) {}
};

template <typename T>
class doublyLinked
{
    node<T>* tail, * head;
    int length;

public:
    doublyLinked() : tail(nullptr), head(nullptr), length(0) {}

    doublyLinked(const doublyLinked& dl) : tail(nullptr), head(nullptr), length(0)
    {
        node<T>* temp = dl.head;
        while (temp)
        {
            insertAtEnd(temp->data);
            temp = temp->nextNode;
        }
    }

    node<T>* getHead() const { return head; }
    node<T>* getTail() const { return tail; }
    int getLength() const { return length; }

    void insertAtStart(T val)
    {
        node<T>* newNode = new node<T>(val);
        if (isEmpty())
        {
            head = tail = newNode;
        }
        else
        {
            newNode->nextNode = head;
            head->previousNode = newNode;
            head = newNode;
        }
        length++;
    }

    void insertAtEnd(T val)
    {
        if (isEmpty())
        {
            insertAtStart(val);
            return;
        }

        node<T>* newNode = new node<T>(val);
        tail->nextNode = newNode;
        newNode->previousNode = tail;
        tail = newNode;
        length++;
    }

    void insertAtPosition(int n, T val)
    {
        if (n < 1 || n > length + 1)
        {
            cout << "Invalid position" << endl;
            return;
        }
        if (n == 1)
        {
            insertAtStart(val);
            return;
        }
        if (n == length + 1)
        {
            insertAtEnd(val);
            return;
        }

        node<T>* newNode = new node<T>(val);
        node<T>* temp = head;

        for (int i = 1; i < n - 1; i++)
        {
            temp = temp->nextNode;
        }
        newNode->nextNode = temp->nextNode;
        newNode->previousNode = temp;
        temp->nextNode->previousNode = newNode;
        temp->nextNode = newNode;
        length++;
    }

    void deleteFromStart()
    {
        if (isEmpty())
        {
            cout << "The list is empty" << endl;
            return;
        }

        node<T>* temp = head;
        head = head->nextNode;
        if (head)
            head->previousNode = nullptr;
        else
            tail = nullptr;
        delete temp;
        length--;
    }

    void deleteFromEnd()
    {
        if (isEmpty())
        {
            cout << "The list is empty" << endl;
            return;
        }

        node<T>* temp = tail;
        tail = tail->previousNode;
        if (tail)
            tail->nextNode = nullptr;
        else
            head = nullptr;
        delete temp;
        length--;
    }

    void deleteFromPosition(int n)
    {
        if (n < 1 || n > length)
        {
            cout << "Invalid position" << endl;
            return;
        }
        if (n == 1)
        {
            deleteFromStart();
            return;
        }
        if (n == length)
        {
            deleteFromEnd();
            return;
        }

        node<T>* temp = head;
        for (int i = 1; i < n; i++)
        {
            temp = temp->nextNode;
        }
        temp->previousNode->nextNode = temp->nextNode;
        temp->nextNode->previousNode = temp->previousNode;
        delete temp;
        length--;
    }

    void forwardDisplay() const
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

    void backwardDisplay() const
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

    bool isEmpty() const { return head == nullptr; }

    ~doublyLinked()
    {
        while (head)
        {
            deleteFromStart();
        }
    }
};

template <class T>
class staticQueue
{
    vector<T> arr;
    int front, rear, capacity, length;

public:
    staticQueue(int size = 5) : capacity(size), front(0), rear(-1), length(0)
    {
        arr.resize(capacity);
    }

    void enqueue(T data)
    {
        if (isFull())
        {
            dequeue();
        }
        rear = (rear + 1) % capacity;
        arr[rear] = data;
        if (length < capacity) length++;
    }

    void dequeue()
    {
        if (isEmpty()) return;
        front = (front + 1) % capacity;
        length--;
    }

    T getFront() const
    {
        if (isEmpty()) throw runtime_error("Queue is empty");
        return arr[front];
    }

    bool isEmpty() const { return length == 0; }
    bool isFull() const { return length == capacity; }

    void display() const
    {
        for (int i = 0; i < length; i++)
        {
            cout << i + 1 << ". " << arr[(front + i) % capacity] << endl;
        }
    }
};

class tracks
{
    string artist;
    string name;
    int duration;

public:
    tracks(string n = "", string a = "", int d = 0) : artist(a), name(n), duration(d) {}
    
    void display() const
    {
        cout << "Name: " << name << ", Duration: " << duration << ", Artist: " << artist << endl;
    }
    
    string getName() const { return name; }
};

ostream& operator<<(ostream& os, const tracks& t)
{
    string name = t.getName();
    cout << "Playing " << fs::path(name).filename() << "..." << endl;
    
    // Convert string to wide string for PlaySoundW
    wstring widePath(name.begin(), name.end());
    PlaySoundW(widePath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
    return os;
}

class playlist
{
    string name;
    doublyLinked<tracks> songs;
    staticQueue<string> history;

    public:
    playlist(string n = "") : name(n) {}

    void insertAtTail(const tracks& d)
    {
        songs.insertAtEnd(d);
        history.enqueue(fs::path(d.getName()).filename().string());
    }

    void deleteAtHead() { songs.deleteFromStart(); }
    void deleteAtTail() { songs.deleteFromEnd(); }
    void deleteAtPosition(int p) { songs.deleteFromPosition(p); }

    void traverse() const { songs.forwardDisplay(); }

    void shuffle() const
    {
        vector<node<tracks>*> nodes;
        node<tracks>* temp = songs.getHead();
        while (temp)
        {
            nodes.push_back(temp);
            temp = temp->nextNode;
        }

        random_device rd;
        mt19937 gen(rd());
        std::shuffle(nodes.begin(), nodes.end(), gen);

        for (const auto& node : nodes)
        {
            cout << node->data;
        }
    }

    bool isEmpty() const { return songs.isEmpty(); }
    string getName() const { return name; }
    void displayHistory() const { history.display(); }
};

class AudioPlayer
{
    vector<string> songs;
    vector<playlist> allPlaylists;
    fs::path dirSelected;

    public:
    AudioPlayer() : dirSelected("C://") {}

    size_t getPlaylistCount() const { return allPlaylists.size(); }

    void listenToPlaylist(int index) const
    {
        if (index < 1 || index > static_cast<int>(allPlaylists.size()))
        {
            cout << "Invalid playlist number!" << endl;
            return;
        }
        allPlaylists[index - 1].traverse();
    }

    void shufflePlaylist(int index) const
    {
        if (index < 1 || index > static_cast<int>(allPlaylists.size()))
        {
            cout << "Invalid playlist number!" << endl;
            return;
        }
        allPlaylists[index-1].shuffle();  // Call without arguments
    }

    void displayHistory(int index) const
    {
        if (index < 1 || index > static_cast<int>(allPlaylists.size()))
        {
            cout << "Invalid playlist number!" << endl;
            return;
        }
        allPlaylists[index - 1].displayHistory();
    }

    playlist createPlaylist()
    {
        if (songs.empty())
        {
            cout << "No songs available to create a playlist!" << endl;
            return playlist();
        }

        string name;
        cout << "\nEnter name of your playlist: ";
        cin >> name;

        playlist newPlaylist(name);
        
        cout << "\nAvailable songs:" << endl;
        for (size_t i = 0; i < songs.size(); i++)
        {
            cout << i + 1 << ". " << fs::path(songs[i]).filename() << endl;
        }

        cout << "\nEnter song numbers to add (0 to finish): ";
        int choice;
        while (cin >> choice && choice != 0)
        {
            if (choice > 0 && choice <= static_cast<int>(songs.size()))
            {
                newPlaylist.insertAtTail(tracks(songs[choice - 1]));
            }
            else
            {
                cout << "Invalid song number. Try again." << endl;
            }
        }

        allPlaylists.push_back(newPlaylist);
        return newPlaylist;
    }

    void selectDirectory()
{
    wchar_t szDir[MAX_PATH] = { 0 };
    BROWSEINFOW bInfo = { 0 };
    bInfo.hwndOwner = NULL;
    bInfo.pszDisplayName = szDir;
    bInfo.lpszTitle = L"Select a directory containing audio files";
    bInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

    LPITEMIDLIST lpItem = SHBrowseForFolderW(&bInfo);
    if (lpItem != NULL)
    {
        SHGetPathFromIDListW(lpItem, szDir);
        dirSelected = szDir;
        wcout << L"Selected Directory: " << szDir << endl;
        countSongs();
        CoTaskMemFree(lpItem);  // Added memory release
    }
} 

    void countSongs()
    {
        songs.clear();
        for (const auto& entry : fs::directory_iterator(dirSelected))
        {
            string ext = entry.path().extension().string();
            transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
            if (ext == ".mp3" || ext == ".wav" || ext == ".ogg" || ext == ".flac")
            {
                songs.push_back(entry.path().string());
            }
        }
    }

    void showSongs() const
    {
        cout << "\nAvailable songs:" << endl;
        for (size_t i = 0; i < songs.size(); i++)
        {
            cout << i + 1 << ". " << fs::path(songs[i]).filename() << endl;
        }
    }
};

int menu()
{
    int choice;
    cout << "\nAudio Player Menu:" << endl;
    cout << "1. Select directory" << endl;
    cout << "2. Create playlist" << endl;
    cout << "3. Listen to a playlist" << endl;
    cout << "4. Change directory" << endl;
    cout << "5. Shuffle playlist" << endl;
    cout << "6. Display history" << endl;
    cout << "7. Exit" << endl;
    cout << "Your choice: ";
    cin >> choice;
    return choice;
}

int main()
{
    AudioPlayer player;
    int choice = 0;

    while (choice != 7)
    {
        choice = menu();

        switch (choice)
        {
        case 1:
            player.selectDirectory();
            player.showSongs();
            break;

        case 2:
            if (player.createPlaylist().isEmpty())
            {
                cout << "Playlist creation failed!" << endl;
            }
            break;

        case 3:
            if (player.getPlaylistCount() == 0)
            {
                cout << "No playlists available!" << endl;
            }
            else
            {
                int index;
                cout << "Enter playlist number to play (1 to " << player.getPlaylistCount() << "): ";
                cin >> index;
                player.listenToPlaylist(index);
            }
            break;

        case 4:
            player.selectDirectory();
            break;

        case 5:
            if (player.getPlaylistCount() == 0)
            {
                cout << "No playlists available!" << endl;
            }
            else
            {
                int index;
                cout << "Enter playlist number to shuffle (1 to " << player.getPlaylistCount() << "): ";
                cin >> index;
                player.shufflePlaylist(index);
            }
            break;

        case 6:
            if (player.getPlaylistCount() == 0)
            {
                cout << "No playlists available!" << endl;
            }
            else
            {
                int index;
                cout << "Enter playlist number to show history (1 to " << player.getPlaylistCount() << "): ";
                cin >> index;
                player.displayHistory(index);
            }
            break;

        case 7:
            cout << "Exiting..." << endl;
            break;

        default:
            cout << "Invalid choice!" << endl;
            break;
        }
    }

    return 0;
}
