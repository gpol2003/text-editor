# Text Editor
## Table of Contents
  - [Overview](#overview)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Contact](#contact)
## Overview 
This c program simulates a text editor with an integrated clipboard manager which can handle the following operations:
- TYPE <text>
- SELECT <start_index> <end_index>
- MOVE_CURSOR <offset>
- COPY
- PASTE or PASTE <steps_back>
- EXIT

The project contains also a custom implementation of a queue.


## Installation
Project can be downloaded from the attached folder or by clonning my git repository:


```bash
git clone https://github.com/gpol2003/text-editor.git
```

## Usage

To compile main.c and the queue module:
```bash
cd c-project
make
```

To execute the editor program:
```bash
make test
```

To execute editor program with Valgrind memory error detector:
```bash
make check
```
## Contact
Pol Guarch Bosom - guarchp@gmail.com - www.linkedin.com/in/pol-guarch

Project Link: https://github.com/gpol2003/text-editor.git
