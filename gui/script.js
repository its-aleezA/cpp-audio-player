class DoublyLinkedList {
    constructor() {
        this.head = null;
        this.tail = null;
        this.length = 0;
    }

    isEmpty() {
        return this.length === 0;
    }

    insertAtEnd(data) {
        const newNode = { data, next: null, prev: null };
        
        if (this.isEmpty()) {
            this.head = newNode;
            this.tail = newNode;
        } else {
            newNode.prev = this.tail;
            this.tail.next = newNode;
            this.tail = newNode;
        }
        
        this.length++;
        return newNode;
    }

    insertAtStart(data) {
        const newNode = { data, next: null, prev: null };
        
        if (this.isEmpty()) {
            this.head = newNode;
            this.tail = newNode;
        } else {
            newNode.next = this.head;
            this.head.prev = newNode;
            this.head = newNode;
        }
        
        this.length++;
        return newNode;
    }

    deleteFromStart() {
        if (this.isEmpty()) return null;
        
        const removedNode = this.head;
        
        if (this.length === 1) {
            this.head = null;
            this.tail = null;
        } else {
            this.head = this.head.next;
            this.head.prev = null;
        }
        
        this.length--;
        return removedNode.data;
    }

    deleteFromEnd() {
        if (this.isEmpty()) return null;
        
        const removedNode = this.tail;
        
        if (this.length === 1) {
            this.head = null;
            this.tail = null;
        } else {
            this.tail = this.tail.prev;
            this.tail.next = null;
        }
        
        this.length--;
        return removedNode.data;
    }

    deleteFromPosition(position) {
        if (position < 0 || position >= this.length) return null;
        
        if (position === 0) return this.deleteFromStart();
        if (position === this.length - 1) return this.deleteFromEnd();
        
        let currentNode = this.head;
        for (let i = 0; i < position; i++) {
            currentNode = currentNode.next;
        }
        
        currentNode.prev.next = currentNode.next;
        currentNode.next.prev = currentNode.prev;
        this.length--;
        
        return currentNode.data;
    }

    traverse() {
        const items = [];
        let currentNode = this.head;
        
        while (currentNode) {
            items.push(currentNode.data);
            currentNode = currentNode.next;
        }
        
        return items;
    }

    shuffle() {
        const nodes = [];
        let currentNode = this.head;
        
        while (currentNode) {
            nodes.push(currentNode);
            currentNode = currentNode.next;
        }
        
        // Fisher-Yates shuffle algorithm
        for (let i = nodes.length - 1; i > 0; i--) {
            const j = Math.floor(Math.random() * (i + 1));
            [nodes[i], nodes[j]] = [nodes[j], nodes[i]];
        }
        
        // Rebuild the list
        this.head = nodes[0];
        this.head.prev = null;
        
        for (let i = 1; i < nodes.length; i++) {
            nodes[i].prev = nodes[i - 1];
            nodes[i - 1].next = nodes[i];
            nodes[i].next = null;
        }
        
        this.tail = nodes[nodes.length - 1];
    }
}

class StaticQueue {
    constructor(capacity = 5) {
        this.capacity = capacity;
        this.queue = [];
        this.front = 0;
        this.rear = -1;
        this.size = 0;
    }

    enqueue(item) {
        if (this.isFull()) {
            this.dequeue();
        }
        this.rear = (this.rear + 1) % this.capacity;
        this.queue[this.rear] = item;
        if (this.size < this.capacity) this.size++;
    }

    dequeue() {
        if (this.isEmpty()) return null;
        const item = this.queue[this.front];
        this.front = (this.front + 1) % this.capacity;
        this.size--;
        return item;
    }

    isEmpty() {
        return this.size === 0;
    }

    isFull() {
        return this.size === this.capacity;
    }

    getItems() {
        const items = [];
        for (let i = 0; i < this.size; i++) {
            const index = (this.front + i) % this.capacity;
            items.push(this.queue[index]);
        }
        return items;
    }
}

class Playlist {
    constructor(name) {
        this.name = name;
        this.songs = new DoublyLinkedList();
        this.history = new StaticQueue(5);
    }

    addSong(song) {
        this.songs.insertAtEnd(song);
        this.history.enqueue(song);
    }

    play() {
        return this.songs.traverse();
    }

    shuffle() {
        this.songs.shuffle();
        return this.songs.traverse();
    }

    getHistory() {
        return this.history.getItems();
    }
}

class AudioPlayer {
    constructor() {
        this.songs = [];
        this.playlists = [];
        this.currentPlaylist = null;
        this.currentSongIndex = -1;
        this.audio = new Audio();
        this.isPlaying = false;
        this.isShuffled = false;
        this.isRepeating = false;
        this.originalOrder = [];
    }

    loadSongs(files) {
        this.songs = files
            .filter(file => {
                const ext = file.name.split('.').pop().toLowerCase();
                return ['mp3', 'wav', 'ogg', 'flac'].includes(ext);
            })
            .map(file => ({
                name: file.name,
                path: URL.createObjectURL(file),
                artist: "Unknown Artist",
                duration: 0 // Will be updated when loaded
            }));
    }

    createPlaylist(name, songIndices) {
        const playlist = new Playlist(name);
        
        songIndices.forEach(index => {
            if (index >= 0 && index < this.songs.length) {
                playlist.addSong(this.songs[index]);
            }
        });
        
        this.playlists.push(playlist);
        return playlist;
    }

    playPlaylist(playlistIndex, shuffle = false) {
        if (playlistIndex < 0 || playlistIndex >= this.playlists.length) return;
        
        this.currentPlaylist = this.playlists[playlistIndex];
        this.isShuffled = shuffle;
        
        if (shuffle) {
            this.originalOrder = [...this.currentPlaylist.songs.traverse()];
            this.currentPlaylist.shuffle();
        }
        
        this.playSong(0);
    }

    playSong(index) {
        if (!this.currentPlaylist || index < 0 || index >= this.currentPlaylist.songs.length) return;
        
        this.currentSongIndex = index;
        const songs = this.currentPlaylist.songs.traverse();
        const song = songs[index];
        
        this.audio.src = song.path;
        this.audio.load();
        this.audio.play()
            .then(() => {
                this.isPlaying = true;
                this.updatePlayerUI(song);
            })
            .catch(error => {
                console.error("Playback failed:", error);
            });
    }

    playNext() {
        if (!this.currentPlaylist) return;
        
        const songs = this.currentPlaylist.songs.traverse();
        let nextIndex = this.currentSongIndex + 1;
        
        if (nextIndex >= songs.length) {
            if (this.isRepeating) {
                nextIndex = 0;
            } else {
                this.stop();
                return;
            }
        }
        
        this.playSong(nextIndex);
    }

    playPrev() {
        if (!this.currentPlaylist) return;
        
        let prevIndex = this.currentSongIndex - 1;
        if (prevIndex < 0) prevIndex = 0;
        
        this.playSong(prevIndex);
    }

    togglePlayPause() {
        if (!this.currentPlaylist) return;
        
        if (this.isPlaying) {
            this.audio.pause();
            this.isPlaying = false;
        } else {
            this.audio.play()
                .then(() => {
                    this.isPlaying = true;
                })
                .catch(error => {
                    console.error("Playback failed:", error);
                });
        }
    }

    stop() {
        this.audio.pause();
        this.audio.currentTime = 0;
        this.isPlaying = false;
    }

    toggleShuffle() {
        this.isShuffled = !this.isShuffled;
        
        if (this.currentPlaylist) {
            if (this.isShuffled) {
                this.originalOrder = [...this.currentPlaylist.songs.traverse()];
                this.currentPlaylist.shuffle();
            } else {
                // Restore original order
                this.currentPlaylist.songs = new DoublyLinkedList();
                this.originalOrder.forEach(song => {
                    this.currentPlaylist.songs.insertAtEnd(song);
                });
            }
        }
    }

    toggleRepeat() {
        this.isRepeating = !this.isRepeating;
    }

    setVolume(volume) {
        this.audio.volume = volume;
    }

    seek(time) {
        this.audio.currentTime = time;
    }

    updatePlayerUI(song) {
        // This will be implemented in the UI section
    }
}

// UI Implementation
document.addEventListener('DOMContentLoaded', () => {
    const player = new AudioPlayer();
    
    // DOM Elements
    const selectDirBtn = document.getElementById('selectDirBtn');
    const directoryInput = document.getElementById('directoryInput');
    const createPlaylistBtn = document.getElementById('createPlaylistBtn');
    const playlistsContainer = document.getElementById('playlistsContainer');
    const songsContainer = document.getElementById('songsContainer');
    const currentPlaylistName = document.getElementById('currentPlaylistName');
    const historyContainer = document.getElementById('historyContainer');
    const availableSongs = document.getElementById('availableSongs');
    const playlistModal = document.getElementById('playlistModal');
    const closeModal = document.querySelector('.close');
    const savePlaylistBtn = document.getElementById('savePlaylistBtn');
    const playlistNameInput = document.getElementById('playlistName');
    const currentSongTitle = document.getElementById('currentSongTitle');
    const currentSongArtist = document.getElementById('currentSongArtist');
    const albumArt = document.getElementById('albumArt');
    const progress = document.getElementById('progress');
    const currentTime = document.getElementById('currentTime');
    const duration = document.getElementById('duration');
    const playPauseBtn = document.getElementById('playPauseBtn');
    const prevBtn = document.getElementById('prevBtn');
    const nextBtn = document.getElementById('nextBtn');
    const shuffleBtn = document.getElementById('shuffleBtn');
    const repeatBtn = document.getElementById('repeatBtn');
    const volume = document.getElementById('volume');
    
    // Event Listeners
    selectDirBtn.addEventListener('click', () => directoryInput.click());
    directoryInput.addEventListener('change', handleDirectorySelect);
    createPlaylistBtn.addEventListener('click', openCreatePlaylistModal);
    closeModal.addEventListener('click', () => playlistModal.style.display = 'none');
    savePlaylistBtn.addEventListener('click', savePlaylist);
    playPauseBtn.addEventListener('click', togglePlayPause);
    prevBtn.addEventListener('click', playPrevious);
    nextBtn.addEventListener('click', playNext);
    shuffleBtn.addEventListener('click', toggleShuffle);
    repeatBtn.addEventListener('click', toggleRepeat);
    volume.addEventListener('input', adjustVolume);
    
    // Audio event listeners
    player.audio.addEventListener('timeupdate', updateProgress);
    player.audio.addEventListener('loadedmetadata', updateDuration);
    player.audio.addEventListener('ended', handleSongEnd);
    player.audio.addEventListener('play', updatePlayButton);
    player.audio.addEventListener('pause', updatePlayButton);
    progress.addEventListener('input', seekAudio);
    
    // Functions
    function handleDirectorySelect(event) {
        const files = Array.from(event.target.files);
        player.loadSongs(files);
        renderAvailableSongs();
    }
    
    function openCreatePlaylistModal() {
        if (player.songs.length === 0) {
            alert('Please select a directory with music files first!');
            return;
        }
        playlistModal.style.display = 'block';
    }
    
    function renderAvailableSongs() {
        availableSongs.innerHTML = '';
        player.songs.forEach((song, index) => {
            const songElement = document.createElement('div');
            songElement.className = 'available-song';
            songElement.textContent = song.name;
            songElement.addEventListener('click', () => {
                songElement.classList.toggle('selected');
            });
            availableSongs.appendChild(songElement);
        });
    }
    
    function savePlaylist() {
        const name = playlistNameInput.value.trim();
        if (!name) {
            alert('Please enter a playlist name');
            return;
        }
        
        const selectedSongs = Array.from(document.querySelectorAll('.available-song.selected'))
            .map(el => Array.from(availableSongs.children).indexOf(el));
        
        if (selectedSongs.length === 0) {
            alert('Please select at least one song');
            return;
        }
        
        player.createPlaylist(name, selectedSongs);
        renderPlaylists();
        playlistModal.style.display = 'none';
        playlistNameInput.value = '';
        document.querySelectorAll('.available-song.selected').forEach(el => el.classList.remove('selected'));
    }
    
    function renderPlaylists() {
        playlistsContainer.innerHTML = '';
        player.playlists.forEach((playlist, index) => {
            const playlistElement = document.createElement('div');
            playlistElement.className = 'playlist-item';
            playlistElement.textContent = playlist.name;
            playlistElement.addEventListener('click', () => {
                // Remove active class from all playlists
                document.querySelectorAll('.playlist-item').forEach(el => el.classList.remove('active'));
                // Add active class to clicked playlist
                playlistElement.classList.add('active');
                
                player.playPlaylist(index);
                renderSongs(playlist);
                currentPlaylistName.textContent = playlist.name;
            });
            playlistsContainer.appendChild(playlistElement);
        });
    }
    
    function renderSongs(playlist) {
        songsContainer.innerHTML = '';
        const songs = playlist.songs.traverse();
        
        songs.forEach((song, index) => {
            const songElement = document.createElement('div');
            songElement.className = 'song-item';
            if (index === player.currentSongIndex) {
                songElement.classList.add('active');
            }
            
            const songInfo = document.createElement('div');
            songInfo.className = 'song-info';
            songInfo.innerHTML = `
                <div>${song.name}</div>
                <small>${song.artist}</small>
            `;
            
            const songDuration = document.createElement('div');
            songDuration.className = 'song-duration';
            songDuration.textContent = formatTime(song.duration);
            
            songElement.appendChild(songInfo);
            songElement.appendChild(songDuration);
            
            songElement.addEventListener('click', () => {
                player.playSong(index);
            });
            
            songsContainer.appendChild(songElement);
        });
    }
    
    function updatePlayerUI(song) {
        currentSongTitle.textContent = song.name;
        currentSongArtist.textContent = song.artist;
        albumArt.src = 'https://via.placeholder.com/150'; // Replace with actual album art if available
        
        // Update active song in playlist
        const songElements = document.querySelectorAll('.song-item');
        songElements.forEach((el, index) => {
            if (index === player.currentSongIndex) {
                el.classList.add('active');
            } else {
                el.classList.remove('active');
            }
        });
    }
    
    function togglePlayPause() {
        player.togglePlayPause();
    }
    
    function playPrevious() {
        player.playPrev();
    }
    
    function playNext() {
        player.playNext();
    }
    
    function toggleShuffle() {
        player.toggleShuffle();
        shuffleBtn.classList.toggle('active', player.isShuffled);
        
        if (player.currentPlaylist) {
            renderSongs(player.currentPlaylist);
        }
    }
    
    function toggleRepeat() {
        player.toggleRepeat();
        repeatBtn.classList.toggle('active', player.isRepeating);
    }
    
    function adjustVolume() {
        player.setVolume(volume.value);
    }
    
    function updateProgress() {
        if (!isNaN(player.audio.duration)) {
            progress.value = player.audio.currentTime;
            progress.max = player.audio.duration;
            currentTime.textContent = formatTime(player.audio.currentTime);
        }
    }
    
    function updateDuration() {
        if (!isNaN(player.audio.duration)) {
            duration.textContent = formatTime(player.audio.duration);
            
            // Update song duration in the playlist
            if (player.currentPlaylist && player.currentSongIndex >= 0) {
                const songs = player.currentPlaylist.songs.traverse();
                songs[player.currentSongIndex].duration = player.audio.duration;
                
                const songElements = document.querySelectorAll('.song-item .song-duration');
                if (songElements[player.currentSongIndex]) {
                    songElements[player.currentSongIndex].textContent = formatTime(player.audio.duration);
                }
            }
        }
    }
    
    function handleSongEnd() {
        if (player.isRepeating) {
            player.playSong(player.currentSongIndex);
        } else {
            player.playNext();
        }
    }
    
    function updatePlayButton() {
        const icon = playPauseBtn.querySelector('i');
        if (player.isPlaying) {
            icon.classList.remove('fa-play');
            icon.classList.add('fa-pause');
        } else {
            icon.classList.remove('fa-pause');
            icon.classList.add('fa-play');
        }
    }
    
    function seekAudio() {
        player.seek(progress.value);
    }
    
    function formatTime(seconds) {
        const mins = Math.floor(seconds / 60);
        const secs = Math.floor(seconds % 60);
        return `${mins}:${secs < 10 ? '0' : ''}${secs}`;
    }
    
    // Close modal when clicking outside of it
    window.addEventListener('click', (event) => {
        if (event.target === playlistModal) {
            playlistModal.style.display = 'none';
        }
    });
});
