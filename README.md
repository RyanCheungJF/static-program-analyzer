# Team 06

## Target Environment

| Item         | Version                                                 |
| ------------ | ------------------------------------------------------- |
| OS           | Windows 10                                              |
| Toolchain    | Microsoft Visual Studio Enterprise 2019 Version 16.11.0 |
| C++ Standard | C++17                                                   |

### Additional Build Instructions

1. To run the system tests, do check out the readme under the `Team06/Tests06/systemtests/` directory on the instructions to run the relevant files based on your operating system.
2. To run your own custom test cases, please enter the directory where your AutoTester.exe resides in. Then, run the command `.\AutoTester.exe ..\Tests\Sample_source.txt ..\Tests\Sample_queries.txt ..\Tests\out.xml` to check that it is compiling where an <b>out.xml</b> will be produced. To have your own custom input, please feel free to edit the <i>Sample_queries.txt</i> and <i>Sample_source.txt</i>
3. To view your results of the out.xml, run `python -m http.server {PORT}` where PORT can be any port number of your liking, such as `8000`. Go to your `localhost:{PORT}` in your browser, navigate and open the out.xml file.

### Running Checkstyle

To run checkstyle, first install the `clang-format` package.

```bash
sudo apt-get install clang-format
```

Ensure that your `style.sh` script has executable permissions,
then run it to automatically style your code.

```bash
chmod u+x style.sh
./style.sh
```

The checkstyle runs on all directories, and targets `.cpp` and `.h` files.

To configure how the checkstyle looks, change the properties in `.clang-format`.

#### _Todo_

Optimize with husky pre commit hooks? 🥴

# Team Members

|                         Name |  Mobile  | Email              | Development OS/Toolchain      |
| ---------------------------: | :------: | :----------------- | ----------------------------- |
|                  Hoon Darren | 96353534 | e0523347@u.nus.edu | MacOS/CLion                   |
|                 Cui Langyuan | 98225819 | e0148779@u.nus.edu | Windows 11/Visual Studio 2019 |
| Muhammad Faruq Bin Jahangeer | 81268656 | e0544096@u.nus.edu | MacOS/CLion                   |
|           Calvin Tan Wei Jie | 92207978 | e0544514@u.nus.edu | Windows 10/Visual Studio 2022 |
|                 Ong Xing Wei | 81852377 | e0543718@u.nus.edu | Windows 10/Visual Studio 2022 |
|        Ryan Cheung Jing Feng | 83213902 | e0543623@u.nus.edu | Windows 10/Visual Studio 2019 |
