# Nodepp-Arduino

<img src="https://github.com/NodeppOficial/nodepp-arduino/assets/109326461/3d427678-9bea-4f71-93a4-37c349a01e22" 
     width="180px" align="left" style=" padding:10px; " >

**Nodepp** is an innovative open source project that aims to make it easier to create applications in **C++**. This project introduces a high-level abstraction layer that allows developers to write **C++** code in a way that is similar to how they would write code in **NodeJS**. With **Nodepp**, developers can leverage the advantages of the **C++** language while benefiting from the ease of use and flexibility that **NodeJS** offers. This unique combination of features makes **Nodepp** a powerful tool for creating high-performance and scalable applications. Additionally, since it is an open source project, anyone can contribute and improve **Nodepp** to fit their specific needs.

## Features

- Open source project, meaning anyone can contribute and improve it
- Allows writing **C++** code as if writing in **NodeJS** or **Javascript**
- Adds a high-level abstraction layer to simplify **C++** application development
- Includes an **Event Loop** that can handle multiple events and tasks on a single thread
- Supports **Coroutines**, which allows running multiple tasks concurrently on a single thread

## Hello world
```cpp
#include <nodepp.h>

using namespace nodepp;

void _main_() {
    console::log("Hello World!");
}
```
```
Hello World!
```

## Examples
- Examples : [here](https://github.com/NodeppOficial/Nodepp/tree/main/examples)

## FAQ
- reddit : [/r/Cplusplus/](https://www.reddit.com/r/Cplusplus/comments/19e2kw3/write_asynchronous_code_with_c_nodepp/) 
- reddit : [/r/cpp/](https://www.reddit.com/r/cpp/comments/19camli/write_asynchronously_code_with_c_nodepp/)

## Links

- **Nodepp:** compatible with Arduino [here](https://github.com/NodeppOficial/nodepp-arduino)
- **Nodepp:** compatible with PC      [here](https://github.com/NodeppOficial/Nodepp)
- **Nodepp:** compatible with ESPxxxx [here](https://github.com/NodeppOficial/Nodepp)
  
## Contribution

If you want to contribute to **Nodepp**, you are welcome to do so! You can contribute in several ways:

- Improving the documentation
- Reporting bugs and issues
- Adding new features or improving existing ones
- Writing tests and ensuring compatibility with different platforms
- Before submitting a pull request, make sure to read the contribution guidelines.

## License

**Nodepp** is distributed under the MIT License. See the LICENSE file for more details.
