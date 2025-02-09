# LinuxFastTask
### An task optimizer for Linux written by Gemini 2 (beta)

This project is a dynamic process manager for Linux systems (with experimental macOS support) that aims to improve system responsiveness by automatically pausing and resuming processes based on their resource usage (CPU, RAM) and whether they are actively being used (GUI windows in focus).

## Features

*   **Automatic Process Monitoring:** Continuously monitors running processes, collecting information about CPU usage, memory usage, and GUI window status.
*   **Intelligent Pausing/Resuming:** Pauses non-critical processes (those not actively used and exceeding resource thresholds) to free up CPU and RAM. Resumes processes when they are needed again (e.g., when their GUI window comes into focus).
*   **Priority-Based Management:** Allows assigning priorities to processes, influencing which processes are more likely to be paused.
*   **(Future) Persistence:**  Will save and load process usage history to make more informed decisions over time ("learning" from past behavior).
*   **(Future) User Interface:**  Plans for a command-line interface (CLI) or graphical user interface (GUI) for user interaction and configuration.

## Compilation (Linux)

This project is primarily designed for Linux systems and uses standard Linux libraries.

**Prerequisites:**

*   **GCC (or Clang):** A C compiler.  Most Linux distributions come with GCC pre-installed.
*   **libprocps:**  Provides access to process information from the `/proc` filesystem.  Install it using your distribution's package manager:
    *   Debian/Ubuntu: `sudo apt-get install libprocps-dev`
    *   Fedora/CentOS/RHEL: `sudo yum install procps-ng-devel`
    *   Arch Linux: `sudo pacman -S procps-ng`
*   **X11 (Xlib):**  Required for detecting active GUI windows.  Install the development headers:
    *   Debian/Ubuntu: `sudo apt-get install libx11-dev`
    *   Fedora/CentOS/RHEL: `sudo yum install libX11-devel`
    *   Arch Linux: `sudo pacman -S libx11`

**Building:**

1.  **Clone the repository:**
    ```bash
    git clone <repository_url>
    cd <repository_directory>
    ```

2.  **Compile using the provided Makefile:**
    ```bash
    make
    ```
    This will create an executable file named `process_manager`.

## Usage (Linux)

1. **Run the executable**:  ` $ ./process_manager `
2. **Observe output**: The program will print messages to the console indicating when processes are paused and resumed.
3. **Test**: Open several programs, including some resource-intensive ones (e.g. videos, games, while loops). Use tools such as top and htop to observe the pausing.
4. Use programs and change windows focus.

The program currently uses default thresholds for CPU and memory usage.  These will be configurable in future versions.

## macOS Support (Experimental)

macOS support is experimental and relies on XQuartz, an X11 implementation for macOS.  Native macOS window management (using Quartz) is planned for future development.

**Prerequisites (macOS):**

*   **Xcode Command Line Tools:** Install them by running `xcode-select --install` in the terminal.
*   **XQuartz:** Download and install XQuartz from [https://www.xquartz.org/](https://www.xquartz.org/).
*  **Homebrew**: A package manager. Install it following the instructions here:  [https://brew.sh/](https://brew.sh/)

**Building (macOS):**

1.  **Install `libprocps` and `X11` libraries (using Homebrew):**

 ```bash
  brew install procps xorg-server
 ```
 *Note: `procps` on macOS provides a limited subset of the functionality of the Linux `libprocps`. This may cause some features not working properly.*

2.  **Adjust the `Makefile` (if necessary):**  You might need to manually specify the include and library paths for XQuartz in the `Makefile`:

 ```makefile
 CFLAGS = -Wall -Wextra -g -I/opt/X11/include
 LDFLAGS = -L/opt/X11/lib -lX11
 ```
 Check in `/opt/X11/include` and `/opt/X11/lib` to see if they are the correct path.

3.  **Compile:**
 ```bash
 make
 ```

**Usage (macOS):**

The usage is the same as on Linux, but keep in mind that the behavior might be different due to the limitations of `procps` and XQuartz on macOS.

## Future Development

*   **Improved macOS Support:**  Implement native macOS window management using Quartz for better performance and reliability.
*   **Persistence:**  Save and load process usage data.
*   **User Interface:**  Develop a CLI or GUI for easier interaction.
*   **Configuration:**  Allow users to customize thresholds and other settings.
*   **Signal Handling:**  Implement more robust signal handling.
*   **Error Handling:**  Improve error handling throughout the code.
*   **Testing:**  Add unit and integration tests.
*   **GPU Usage Monitoring:** Extend monitoring to include GPU usage (if possible).
* **Logging**: Add a logging system.
* **User configuration file**: Support a config file.

## Contributing

Contributions are welcome!  Please feel free to submit pull requests or open issues to discuss improvements or bug fixes.

## License
This project is licensed under the MIT. See the LICENSE file for more details.