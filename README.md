# Text Editor
## Table of Contents
  - [Overview](#overview)
  - [Installation](#installation)
  - [Usage](#usage)
  - [Example](#example)
  - [Contact](#contact)
  - [Acknowledgments](#acknowledgments)

## Overview 
This c an go program simulates a text editor with an integrated clipboard manager which can handle the following operations:
- TYPE <text>
- SELECT <start_index> <end_index>
- MOVE_CURSOR <offset>
- COPY
- PASTE or PASTE <steps_back>
- EXIT

The project contains also a custom implementation of a queue in c language.


## Installation
Project can be downloaded from the attached folder or by clonning my git repository:


```bash
git clone https://github.com/gpol2003/text-editor.git
```

## Usage

### c
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
### go
To compile and execute main.go
```bash
cd go-project
go run main.go
```

## Example
```bash
Input: "TYPE We hate pointers", "MOVE_CURSOR -3", "SELECT 3 6", "TYPE love"
Output: We love pointers
Input: "EXIT"
Leaving text editor...
```

## Contact
Pol Guarch Bosom - guarchp@gmail.com - [www.linkedin.com/in/pol-guarch](www.linkedin.com/in/pol-guarch)

Project Link: [https://github.com/gpol2003/text-editor.git](https://github.com/gpol2003/text-editor.git)

## Acknowledgments
- [Go First Steps (video)](https://www.youtube.com/watch?v=AGiayASyp2Q)
- [Godoc: documenting Go code](https://go.dev/blog/godoc)
- [Defining GO structs](https://www.digitalocean.com/community/tutorials/defining-structs-in-go-es)
