# 🎵 C++ Audio Player

This is a Windows-based console audio player written in C++. It allows users to browse local music directories, create and manage playlists, shuffle songs, and keep track of recently played tracks — all while demonstrating key data structures like doubly linked lists and queues.

---

## 📌 Features

- Playlist creation from any selected folder
- Plays `.mp3`, `.wav`, `.ogg`, and `.flac` files using Windows API
- Shuffle mode and playback history support
- Custom **doubly linked list** for playlist handling
- Fixed-size **static queue** for playback history
- File system browsing using Windows Shell API

---

## 🏗 Technologies Used

- C++ STL
- Windows Multimedia API (`Winmm.lib`)
- Windows Shell Folder Picker
- `<filesystem>` for path traversal

---

## 📁 Project Structure

| Path           | Description                                |
|----------------|--------------------------------------------|
| `code/`        | Contains the source code (`audio_player.cpp`) |
| `demo/`        | Video preview of the working of the audio player |
| `sample_music/`| Public domain audio for testing           |
| `LICENSE`      | MIT License for open-source use           |
| `README.md`    | Project overview and usage instructions   |

---

## 🤝 Collaborators

This project was written by **[Aleeza Rizwan](https://github.com/its-aleezA)**.  
Special thanks to **[Ibrahim Abdullah](https://github.com/Ibrahim5570)** for running and testing the program on his system due to platform limitations.

---

## ✅ License

This project is licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for details.
