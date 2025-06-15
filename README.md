Repo for our Team & Group training exercises

## Getting Started

### Prerequisites
- Python 3.8+
- Git
- Markdown editor (e.g., Visual Studio Code)

### Installation

1. **Install Python and Pip**
   ```sh
   sudo apt-get update
   sudo apt-get install python3.8 python3-pip
   ```

2. **Install Requirements**
   ```sh
   pip3 install -r requirements.txt
   ```

3. **Start the Documentation Server**
   ```sh
   mkdocs serve
   ```

4. **Open Your Web Browser and Navigate to**
   ```
   http://127.0.0.1:8000
   ```

### Additional Notes
- Ensure you have the `mkdocs` and `mkdocs-material` packages installed.
- The documentation is structured as follows:
  - **Machine Setup**
    - Prerequisites: `machine_setup/prerequisites.md`
    - Installation: `machine_setup/installation.md`
  - **Linux UM**
    - Introduction: `linux_um/shtrudel.md`
    - User Management: `linux_um/tcpdump.md`
