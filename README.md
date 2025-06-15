Repo for our Team & Group training exercises

## Getting Started

### Installation

1. **Setup Python and Pip**
   ```sh
   sudo apt update
   sudo apt install python3.12 python3-pip  # python3.8 also works
   ```

2. **Setup A Virtualenv for MkDocs**
   ```
   mkdir ~/virtualenvs
   python -m venv ~/virtualenvs/hafifot
   source ~/virtualenvs/hafifot/bin/activate.fish

   pip install -r requirements.txt
   ```

3. **Start the Documentation Server**
   ```sh
   mkdocs serve
   ```

4. **Open Your Web Browser and Navigate to `http://127.0.0.1:8000`**
