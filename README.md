# lib-memory-socket

## Retrospective

This project was a workshop with the idea to dive into the communication between processes and threads. The problem was simple: How could programs wrote in different languages communicate together on the same machine and what level of pain it is for developers?

So I take my C++ skills because this language is compatible with all others. I've spent few days finding existing libraries and testing them across Windows and Linux platforms. And as planned, no blocking issue was encountered. Just basic compilation things mainly, not even a segmentation fault 🤔.

In less than a week, a simple API was drafted and implemented. It was possible to establish a connection between 2 peers and send messages at lightning speed. And, a security system was designed based on asymmetric keys, backed by a robust solution.

**[SPOILER]**

The response to the initial problem: The communication between processes and threads is called `inter-process communication` can be achieved using a `named pipe`. It's supported for years on Windows and Unix systems including MacOS. The C++ implementation can be done with robust and maintained libraries like `boost` or `libuv` and the integration is very easy.

## Why ```lib-memory-socket```?

The communication is a complex task when it involve exchanging data between processes and threads. This statement is particulary true in a many-to-many scenario. And on top of that, doing it native adds a huge gap of complexity.

The idea is to tackle the complexity by exposing a simple API inspired by the web transport technologies **TCP** and **UDP**. An extra requirement is to get a fully secured solution by using some encryption methods.

## How?

Just don't reinvent the wheel. There's plenty of existing solution backed by strong actors that can match the technical requirements.

The all-in-one solution doesn't exist and the closer one is [libsharedmemory](https://github.com/kyr0/libsharedmemory). It's cross platform and use the shared memory for communication. But it miss the security layer, has no binary stream implemented and many more lacks. So, this project will be used as a strong reference and a fallback solution if things goes wrong.

### Requirements

- Cross platform: Windows, MacOS, Linux.
- Cross architecture: x86, x64, arm.
- Act as a sealed box: Encrypted data from peer to peer.

### Solutions

| Solution | Purpose |
|--|--|
| [boost](https://www.boost.org/) | shared memory management, buffering |
| [Libsodium](https://libsodium.gitbook.io/doc) | keys generator, encryption, decryption |
| [ObjectBox](https://cpp.objectbox.io/) | in-memory database |

## The API Draft

The API architecture is traditional, the server is capable of receiving client connections, consume their data and respond with new data. An endpoint is defined by an identifier similarly to the host part inside a **URL**.
 There's no mechanism similar to handle a path as it would imply to implement a parameter system which is time consuming. Finally, the encryption is fully transparent for a standard user.

⚠ The following code is not functional and may contain errors. This code is not intended to be compiled.

#### Server
```cpp
// Instanciate a server socket.
auto server = new ServerSocket();

// Handle socket errors.
server.OnError([] (std::string error) {
    // Consume the errors raised be the server socket.
});

// Handle socket data stream.
server.OnData([] (auto socket, void *data, size_t size) {
    // Consume data received from server socket. This is the data sent from a client socket.

    auto msg = "Hello from server";
    if (socket.Write(msg.c_str(), msg.length()) != 0) {
        // Handle write errors.
    }
});

// Listen for client connections.
server.Listen("my-unique-socket-identifier");

// Close the socket.
server.Close();
```

#### Client
```cpp
// Instanciate a client socket.
auto client = new ClientSocket();

// Handle socket errors.
client.OnError([] (std::string error) {
    // Consume the errors raised be the client socket.
});

// Handle socket data stream.
client.OnData([] (auto socket, void *data, size_t size) {
    // Consume data received from client socket. This is the data sent from a server socket.

    auto msg = "Hello from client";
    if (socket.Write(msg.c_str(), msg.length()) == 0) {
        // Handle write errors.
    }
});

// Connect the socket.
if (client.Connect("my-unique-socket-identifier") != 0) {
    // Handle connection error.
}

std::string msg("hello memory world");
if (client->Write(msg.c_str(), msg.length()) == 0) {
    // Handle connection error.
}

// Close the socket.
client.Close();
```

## How to build

1. Start a devcontainer. (Building instructions: [b3nNn/cpp-devcontainer](https://github.com/b3nNn/cpp-devcontainer)).
2. Open a terminal, then install `boost` and `libsodium`.
```shell
sudo apt-get install libsodium23 libsodium-dev libboost-all-dev
```
3. Compile the sources.
```shell
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
```
4. (Optional) Install the library `libmemorysocket` on the system.
```shell
make install
```

## Examples

### simple-program
A single producer - single consumer program.

1. Start the consumer.
```shell
./build/example/simple-program
```
2. Start the producer with ```-p``` argument.
```shell
./build/example/simple-program -p
```
The consumer's output should be similar to this:
```shell
consumed 1k messages
consumed 2k messages
consumed 3k messages
consumed 4k messages
consumed 5k messages
consumed 6k messages
consumed 7k messages
consumed 8k messages
consumed 9k messages
consumed 10k messages
[...]
```

## Todo

- [x] Boost integration.
- [x] Libsodium integration.
- [ ] Objectbox integration.
- [ ] Dedicated devcontainer image with dependencies included.
- [x] Linux support.
- [ ] MacOS support.
- [x] Windows support. _(documentation coming next)_

## Known issues
~~- CMake build on Windows fail.~~

## Licence

The licence is and will always remain MIT. The goal of this project is to enable anyone to do anything he wants with the code and it's usage.
