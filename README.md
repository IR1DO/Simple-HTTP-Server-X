# Simple-HTTP-Server-X

This is the repository for my simple HTTP server project.

## 1. Running Steps

### 1.1 Clone the Project

`git clone https://github.com/IR1DO/Simple-HTTP-Server-X`

### 2.2 Navigate to the Root Directory

`cd Simple-HTTP-Server-X `

### 2.3 Compile and Run

`make`

`cd build/bin`

`./httpServer`

## 2. Testing

`make test`

## 3. View Test Coverage

`make coverage`

## 4. Clean Test Files

`make clean`

## 5. Notes

### 5.1 Static Files

Static files are not included in the GitHub repository and need to be added
manually.

To handle static files for client requests, create a folder named 'static' in
the project's root directory and place the required static files in it. In the
absence of a explicitly specified request file, the server will default to
retrieving the 'index.html' file. Ensure that the 'static' directory contains
the necessary files.

### 5.2 Dependency Tools

Before using `make coverage`, ensure that the following tools are installed on
your system:

- lcov: Used for collecting code coverage information.

  - Install on Ubuntu: `sudo apt-get install lcov`

- xdg-open: Used to open the generated HTML report in the default web browser.
  - Install on Ubuntu: `sudo apt-get install xdg-utils`

On other Linux distributions or operating systems, use the respective package
manager for installation.
