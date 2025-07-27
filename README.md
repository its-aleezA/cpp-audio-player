# 🎵 C++ Audio Player

This is a Windows-based console audio player written in C++. It allows users to browse local music directories, create and manage playlists, shuffle songs, and keep track of recently played tracks — all while demonstrating key data structures like doubly linked lists and queues.

A simple **HTML/CSS/JavaScript GUI** has also been added to visualize the functionalities with a user-friendly interface.

---

## 📌 Features

- Playlist creation from any selected folder
- Plays `.mp3`, `.wav`, `.ogg`, and `.flac` files using Windows API
- Shuffle mode and playback history support
- Custom **doubly linked list** for playlist handling
- Fixed-size **static queue** for playback history
- File system browsing using Windows Shell API
- Web-based GUI to mimic functionality in a visual environment

---

## 🏗 Technologies Used

- C++ STL
- Windows Multimedia API (`Winmm.lib`)
- Windows Shell Folder Picker
- `<filesystem>` for path traversal
- HTML, CSS, JavaScript (for GUI)

---

## 📁 Project Structure

| Path           | Description                                |
|----------------|--------------------------------------------|
| `code/`        | Contains the source code (`audio_player.cpp`) |
| `demo/`        | Video preview of the working of the audio player |
| `gui/`         | A web-based graphical interface (HTML, CSS, JS)               |
| `sample_music/`| Public domain audio for testing           |
| `LICENSE`      | MIT License for open-source use           |
| `README.md`    | Project overview and usage instructions   |

---

## 🌐 Graphical Interface

A simple browser-based GUI has been developed using HTML, CSS, and JavaScript to simulate how the audio player works in a graphical environment.  
This is a **static visual interface** and does not yet integrate directly with the C++ backend.

To try it out:
1. Open `gui/index.html` in any browser.
2. Browse the mock interface and explore playlist functionalities visually.

---

## 🤝 Collaborators

This project was written by **[Aleeza Rizwan](https://github.com/its-aleezA)**.  
Special thanks to **[Ibrahim Abdullah](https://github.com/Ibrahim5570)** for running and testing the program on his system due to platform limitations.

---

## ✅ License

This project is licensed under the **MIT License**.  
See the [LICENSE](LICENSE) file for details.
