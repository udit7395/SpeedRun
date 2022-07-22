# Conan Setup Steps

1. Create conanfile.txt (like requirements.txt)
2. Add `[requires]` and that add libraries with their version number. Use [conan center](https://conan.io/center/) to search for libraries, receipes, etc. Example, adding [{fmt}](https://fmt.dev/latest/index.html) an open-source formatting library.
    ```
    [requires]
    fmt/9.0.0
    ```
3. Add `[generators]` and we will be using `cmake` (For more info on [generators](https://docs.conan.io/en/1.46/reference/generators.html#generators-reference))
    ```
    [generators]
    cmake
    ```
4. In your terminal execute the following command 
    ```bash
    conan profile new default --detect` #(this will use gcc by default on linux)
    conan profile update settings.compiler.libcxx=libstdc++11 default` #(use c++11 by default)
    mkdir build && cd build
    conan install ..
    ``` 
5. This step might fail and you might get an error such as 
    ```bash
    <your-missing-dependency>: ERROR: while executing system_requirements(): Aborted due to CONAN_SYSREQUIRES_MODE=verify
    ```
    By default conan will not automatically install dependencies that need to be installed using `sudo apt-get install <your-missing-dependency>`. Run the following command  
    ```bash
    export CONAN_SYSREQUIRES_MODE=enabled
    ``` 
    and then execute command 
    ```bash
    conan install ..
    ```
    This way conan will also install the missing dependencies. **WARNING** Please check before using this, there is a possibility of messing stuff up.
   
# CMakeLists Setup steps with Conan
1. Once installed successfully, create your CMakeLists.txt 
2. Add the following lines to your CMakeLists
    ```cmake
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
    
    add_executable(<your-executable-name> <files>.cpp)
    target_link_libraries(<your-executable-name> ${CONAN_LIBS})
    ```
3. Execute
    ```bash
    cmake .. && make
    ```

# Bonus - Conan integration with VSCode
1. Even though the code compiles, VSCode would not know where to look for the libraries installed by conan and you could get lint errors such as 
    ```
    #include errors detected. Please update your includePath. Squiggles are disabled for this translation unit
    ```
2. To fix, this add `set(CMAKE_EXPORT_COMPILE_COMMANDS ON)` to your CMakeLists.txt
3. Run cmake again
4. Under your `c_cpp_porperties.json' add the following under `configurations`
    ```json
    "compileCommands": "${workspaceFolder}/build/compile_commands.json"
    ```
5. If you do not `c_cpp_porperties.json`, then open the command pallete `Ctrl+Shift+P` and then click `C/C++: Edit Configurations (UI)`. More information can be on [this stackoverflow page](https://stackoverflow.com/questions/62036568/dont-have-c-cpp-properties-json-file-in-vscode)