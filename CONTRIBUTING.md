# Contributing to SysMood

Hey there, Welcome to SysMood, one of my summer project after learning C and C++ in College and scoring a B in the exam 🥲  
Thanks for considering contributing to **SysMood**! Whether you're here to fix bugs, add features, improve documentation, or just make the system moods even quirkier, we welcome all contributions.

---

## How to Contribute

### 1. Fork & Clone

```bash
# Fork the repository on GitHub first, then:
git clone https://github.com/RayBreeze/SysMood.git
cd SysMood
```

### 2. Create a Branch

```bash
git checkout -b feature/your-awesome-feature
# or
git checkout -b fix/bug-description
```

### 3. Make Your Changes

- Follow the existing code style and structure
- Add comments where necessary (we all love clear communication isn't it ?)
- Test your changes thoroughly

### 4. Commit Your Changes

```bash
git add .
git commit -m "Add: Brief description of your changes"
```

Use clear, descriptive commit messages:

- `Add: New feature description`
- `Fix: Bug description`
- `Update: What was updated`
- `Refactor: What was refactored`

### 5. Push & Create Pull Request

```bash
git push origin feature/your-awesome-feature
```

Then create a Pull Request on GitHub with a clear description of your changes.

---

## What We'd Love Help With

### Bug Fixes

- Memory leaks or performance issues
- Incorrect system readings
- Cross-platform compatibility issues
- Any crashes or unexpected behavior

### New Features

- **Linux and macOS Support**: Help us make SysMood cross-platform!
- **New System Metrics**: Disk usage, network stats, temperature monitoring
- **More Moods**: Creative and funny system mood messages
- **Configuration Options**: Config files for customizing moods and thresholds
- **Different Output Formats**: JSON output, logging capabilities [However, I am a bit      sceptical about this one]

### Documentation

- Improve README.md
- Add code comments
- Create usage examples
- Write installation guides for different platforms

###  Fun Stuff

- New ASCII art banners [Don't break the existing one if your's are not good 🥲]
- More creative mood messages
- Color output support
- Emoji support for moods

---

## Development Setup

### Prerequisites

- **Windows**: Currently the primary supported platform
- **C++17 or newer**: Tested with g++ and MSVC
- **Git**: For version control

### Building the Project

```bash
# From the project root:
g++ -Iinclude src/main.cpp -o SysMood.exe

# Or use the VS Code build task if you're using VS Code
```

### Project Structure

```text
SysMood/
├── include/           # Header files
│   ├── cpu_monitor.h     # CPU usage monitoring
│   ── memory_monitor.h  # Memory usage monitoring
├── src/
│   └── main.cpp          # Main program logic
├── CONTRIBUTING.md       # This file
├── README.md            # Project overview
└── LICENSE              # MIT License
```

---

## Code Style Guidelines

### General Principles

- **Keep it simple**: SysMood is meant to be lightweight and fun
- **Comment your code**: Especially complex logic or Windows API calls
- **Use descriptive names**: Variables and functions should be self-explanatory enough to understand
- **Follow existing patterns**: Look at the current code structure

### C++ Style

- Use `snake_case` for variables and functions
- Use `PascalCase` for classes
- Include proper headers and avoid unnecessary includes
- Use const where appropriate
- Handle errors gracefully

### Example

```cpp
// Good ✅
int cpu_usage_percent = cpu.now();
if (cpu_usage_percent > 80) {
    std::cout << "CPU is working hard! Maybe give it a break? 😅" << std::endl;
}

// Not so good ❌
int x = cpu.now();
std::cout << "high cpu" << std::endl;
```

---

## Testing Your Changes

### Manual Testing

1. Build the project: `g++ -Iinclude src/main.cpp -o SysMood.exe`
2. Run the executable: `./SysMood.exe`
3. Check that:
   - CPU and memory readings seem accurate
   - Mood messages display correctly
   - ASCII art renders properly
   - No crashes or errors occur

### Test Different Scenarios

- Test with high CPU usage (run some intensive tasks)
- Test with high memory usage
- Test with low resource usage
- Test on different Windows versions if possible [Remember, It works only on Windows 10 and later versions]

---

## Pull Request Guidelines

### Before Submitting

- [ ] Code builds without errors or warnings
- [ ] Changes have been tested manually
- [ ] Commit messages are clear and descriptive
- [ ] No unnecessary files are included (build artifacts, IDE files, etc.)

### PR Description Should Include

- **What**: Brief description of changes
- **Why**: Reason for the changes
- **How**: How you implemented it
- **Testing**: How you tested the changes

### Example PR Description

```markdown
## Add Linux Support for CPU Monitoring

**What**: Implemented CPU usage monitoring for Linux systems using /proc/stat

**Why**: Users have been requesting Linux support (#issue-number)

**How**: 
- Added conditional compilation for Linux vs Windows
- Used /proc/stat parsing for Linux CPU usage
- Maintained existing Windows functionality

**Testing**: 
- Tested on Ubuntu 20.04 and 22.04
- Verified Windows functionality still works
- CPU readings match system monitor tools
```

---

## What Not to Contribute

- **Malicious code**: Obviously you don't wanna see the world burn 😶‍🌫️🙏🏻
- **Copyrighted code**: Only contribute code you own or have permission to use. Please I am a independent developer, can't afford legal troubles
- **Breaking changes**: Avoid changes that break existing functionality without good reason
- **Overly complex solutions**: Keep it simple and maintainable

---

## Recognition

All contributors will be:

- Listed in the README.md contributors section
- Mentioned in release notes for their contributions
- Author will be personally thankful to all contributors 🫠

---

## Getting Help

- **Issues**: Open an issue on GitHub for bugs or feature requests
- **Discussions**: Use GitHub Discussions for general questions
- **Contact**: Reach out to [@RayBreeze](https://github.com/RayBreeze) for major contributions
    also can email me at sammandas234@gmail.com

---

## Code of Conduct

### Be Nice

- Be respectful and constructive in all interactions
- Help others learn and grow
- Keep discussions focused and productive
- Have fun! SysMood is meant to be a fun project 😊

### Not Tolerated

- Harassment, discrimination, or offensive behavior
- Spam or off-topic discussions
- Plagiarism or copyright violations

---

## First-Time Contributors

New to open source? No worries! Here are some good first issues to start with:

### Easy Issues

- Add new mood messages for different CPU/memory scenarios
- Fix typos in documentation or comments
- Add more ASCII art options
- Improve error messages

### Medium Issues

- Add color output support
- Implement configuration file support
- Add new system metrics (disk usage, etc.)
- Improve cross-platform compatibility

---

**Happy Contributing!** 🎉  
Let's make systems everywhere feel a little more understood just like humans do! 💻❤️

---

*This contributing guide is a living document. If you have suggestions for improvements, please open an issue or submit a PR!*
