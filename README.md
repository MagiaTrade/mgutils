# mgutils Library

`mgutils` is a C++ utility library designed to provide a set of common functionalities that can be reused across different projects. It includes utilities for logging, JSON handling, CSV parsing, event management, file operations, error handling, and job pooling, all with a focus on performance and ease of use.

## Features

### 1. Logger
- **Singleton Instance:** Ensures that only one instance of the logger exists throughout the application.
- **Log Levels:** Supports multiple log levels including Trace, Debug, Info, Warning, Error, and Critical.
- **File Logging:** Allows logging to files with options for rotating logs based on size.
- **Formatting Support:** Supports variadic formatting and streaming operators.
- **Macros:** Provides macros for easy logging of errors and critical messages.

### 2. JSON Handling
- **JSON Document Creation:** Supports creating JSON documents with different root types (object or array).
- **Parsing:** Allows parsing JSON strings into objects.
- **Manipulation:** Supports setting and getting values, including nested objects and arrays, with type safety.

### 3. CSV Parsing
- **Reading CSV Files:** Provides utilities to read CSV files and handle them easily within the application.
- **Data Manipulation:** Supports accessing CSV data through a simple interface.

### 4. Event Management
- **Event Handling:** Allows registering and triggering events with listeners for various types of events.
- **Boost Integration:** Utilizes Boost.Signals2 for managing signal-slot connections efficiently.

### 5. File Operations
- **File Utilities:** Provides various utilities for file operations, including reading and writing files, checking file existence, and getting file sizes.

### 6. Error Handling
- **Error Management:** Offers a structured way to handle errors across the application using a centralized error manager.
- **Custom Error Types:** Supports defining and using custom error types for more detailed error reporting.

### 7. Job Pooling
- **Thread Management:** Provides a job pool for managing and executing tasks concurrently, leveraging Intel TBB (Threading Building Blocks) for efficient multi-threading.

## Installation

> **⚠️ Warning:** This project has only been tested on macOS.

### Required Packages

To use `mgutils`, you need to install the following packages:

- [Boost (minimum version 1.83.0)](http://boost.org)

#### macOS Installation

Install the required packages using Homebrew:

```sh
brew install boost@1.83.0
```

#### Linux Installation

On Linux, you can install Boost and OpenSSL using your package manager. For example, on Ubuntu:

```sh
sudo apt-get update
sudo apt-get install -y libboost1.83-dev
```

If Boost 1.83.0 is not available in your distribution, you may need to download and build it manually from the [Boost website](https://www.boost.org/users/download/).

#### Windows Installation

On Windows, you can install Boost and OpenSSL using a package manager like vcpkg:

```sh
vcpkg install boost@1.83.0
```

Make sure to integrate vcpkg with your Visual Studio environment:

```sh
vcpkg integrate install
```

The CMake configuration for the project will resolve any additional dependencies automatically.

## Documentation and Tests
The test files included in this repository serve as living documentation. They provide concrete examples of how to use the various features of the mgutils library. By examining and running these tests, users can gain a better understanding of the library's functionality and intended use cases.

## Contributions

Contributions are welcome! Please feel free to submit a pull request or open an issue for any bugs, feature requests, or improvements.

## Authors

* **Tato Levicz** - [@tatolevicz](https://github.com/tatolevicz)
