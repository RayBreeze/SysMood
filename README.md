# SysMood
[![CI](https://github.com/RayBreeze/SysMood/actions/workflows/ci.yml/badge.svg)](https://github.com/RayBreeze/SysMood/actions/workflows/ci.yml)

Hey there, the lone wonderer here!  
**SysMood** is a fun little console program for Windows that checks your CPU and memory usage, then tells you how your system is "feeling"—with some quirky moods and a splash of ASCII art.

---

## What’s Inside?

- **CPU Usage:**  
  Measures how hard your CPU is working (with a 1-second check).

- **Memory Usage:**  
  Tells you how much memory you’re using, what’s free, and what’s left.

- **System Mood:**  
  Gives you playful, sometimes sassy feedback based on your system’s stats.

- **ASCII Art Banner:**  
  Because every program deserves a cool entrance.

  and many more....

---

## How to Use

### 1. Clone This Repo

```sh
git clone https://github.com/RayBreeze/SysMood.git
cd SysMood
```

### 2. Build It

You’ll need a C++ compiler (like `g++`).  
From the project root, run:

```sh
g++ -Iinclude src/main.cpp -o SysMood.exe
```

### 3. Run It

```sh
./SysMood.exe
```

---

### Install via Winget

```sh
winget install SysMood
```

## What You’ll See [This is Demo of Release 1.0.0, future versions might differ...]

```sh
________       ___    ___ ________  _____ ______   ________  ________  ________     
|\   ____\     |\  \  /  /|\   ____\|\   _ \  _   \|\   __  \|\   __  \|\   ___ \    
\ \  \___|_    \ \  \/  / | \  \___|\ \  \\\__\ \  \ \  \|\  \ \  \|\  \ \  \_|\ \   
 \ \_____  \    \ \    / / \ \_____  \ \  \\|__| \  \ \  \\\  \ \  \\\  \ \  \ \\ \  
  \|____|\  \    \/  /  /   \|____|\  \ \  \    \ \  \ \  \\\  \ \  \\\  \ \  \_\\ \ 
    ____\_\  \ __/  / /       ____\_\  \ \__\    \ \__\ \_______\ \_______\ \_______\
   |\_________\\___/ /       |\_________\|__|     \|__|\|_______|\|_______|\|_______|
   \|_________\|___|/        \|_________|                                            

Sysmood: Your system is feeling. It also has moods.
========================System Stats======================= 
CPU Usage: 12%
Memory Usage: 45%
Memory Available: 8000 MB
Memory Total: 16000 MB
Memory Used: 7200 MB
=========================================================== 
========================System Mood======================== 
Yeah I am chilling here, toally chillll ;)
So much free memory! I could host a party in here! 
=========================================================== 
```

---

## Wanna Tweak It?

- **Change the ASCII Art:**  
  Go wild in `main.cpp`—make it yours!

- **Edit the Mood Messages:**  
  The mood lines for CPU and memory are all in `main.cpp`. Make them as weird or wise as you want.

---

## What You Need

- OS [ Windows or Linux ]
- C++17 or newer (tested with g++ and MSVC)

---

## License

MIT License. See [LICENSE](LICENSE) for the legal stuff.

---

## Who Made This?

Made with ☕ and curiosity by [Samman Das (RayBreeze)](https://github.com/RayBreeze) and with the help of fellow Contributors across the Globe

---

## Contributors

<a href="https://github.com/RayBreeze/SysMood/graphs/contributors">
  <img src="https://contrib.rocks/image?repo=RayBreeze/SysMood" />
</a>

Made with [contrib.rocks](https://contrib.rocks).

---

## Found a Bug? Want to Help? Contributions are Welcome

Open an issue or PR! I’d love to see what you come up with. See the [CONTRIBUTING.md](CONTRIBUTING.md) for more details.
