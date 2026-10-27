# 🔍 Lexical Analyzer in C

A modular **C-based Lexical Analyzer** that performs the first phase of compiler design. It reads a source file, scans its contents, and identifies lexemes such as keywords, identifiers, operators, constants, comments, and delimiters.

The analyzer prints each token along with its category and source line number, helping demonstrate how a compiler breaks source code into meaningful units.

## ✨ Features

- Recognizes C keywords and identifiers.
- Identifies integer and simple decimal constants.
- Detects string and character literals, including escaped characters.
- Recognizes single-line and multi-line comments.
- Identifies common single-character and multi-character operators.
- Detects delimiters and preprocessor lines.
- Displays token categories, lexemes, and line numbers.
- Reports lexical-error labels for unterminated strings and comments.
- Accepts a C source file as a command-line argument.

## 🧰 Built With

- **Language:** C
- **Compiler:** GCC / MinGW
- **Build tool:** Make (optional)

## 📁 Project Structure

```text
lexical_analyzer/
├── main.c       # Program entry point and token output
├── lexer.c     # Token scanning and classification logic
├── lexer.h     # Token definitions and lexer declarations
├── sample.c    # Sample C source file for testing
├── Makefile    # Build and clean commands
└── README.md
```

## ⚙️ Build and Run

### Windows (GCC / MinGW)

Open a terminal in the project directory and compile:

```bash
gcc -Wall -Wextra -Wpedantic -std=c11 main.c lexer.c -o lexer.exe
```

Run the analyzer on the included sample:

```powershell
.\lexer.exe sample.c
```

### Linux / macOS

Build using the included Makefile:

```bash
make
```

Run:

```bash
./lexer sample.c
```

To remove the generated executable:

```bash
make clean
```

## ▶️ Analyze Your Own C File

Pass the path of a C source file as an argument:

```bash
./lexer path/to/your_file.c
```

On Windows:

```powershell
.\lexer.exe path\to\your_file.c
```

## 🖥️ Sample Output

The program displays a table similar to the following:

```text
LINE   | TOKEN TYPE             | LEXEME
-------+------------------------+------------------------------
1      | PREPROCESSOR          | #include <stdio.h>
4      | KEYWORD              | int
4      | IDENTIFIER           | main
4      | DELIMITER            | (
```

The exact output depends on the source file being analyzed.

## 📚 What I Learned

- The role of lexical analysis in compiler design.
- How source code is scanned and separated into tokens.
- Token classification using C programming.
- File handling and command-line arguments.
- Modular programming with separate source and header files.

## ⚠️ Scope and Limitations

This is a learning project, not a complete C-standard lexer or compiler. It supports common C token patterns but does not implement every C numeric-literal rule or all preprocessor continuation rules. It identifies tokens; it does not validate C syntax or compile the source program.

## 👨‍💻 Author

**Vishnu Kalikal**
