# Team 41

## SIMPLE Testing Plan
1. Nesting level - 0/1 (easy), 2 (medium), >=3 (hard)
2. Order of nesting - while/while, if/if, if/while, while/if
3. Position of nested container - first, last, middle, only statement
4. Types of statements - `read` `print` `call` `while` `if` `assign`

## Local Testing
If you need a main function with access to the `spa` library but want to bypass autotester, use the `src/local_testing` project.
1. Create `main.cpp` in directory `src/local_testing/src`
2. Create a main function in `main.cpp`, for example:
```c++
#include <iostream>
using namespace std;
int main() {
    cout << "hello world!" << endl;
}
```
3. Click the green run arrow next to "int main" (Clion only)

## Target Environment

Item | Version
-|-
OS | Windows 10
Toolchain | Cross-platform Startup SPA Solution
C++ Standard | C++17

### Additional Build Instructions

No additional instructions required.

# Team Members

Name | Mobile | Email | Development OS/Toolchain
-:|:-:|:-|-|
Choi Min Suk | 92277054 | e0310270@u.nus.edu | Windows 10/CLion
Hardy Shein | 97578119 | e0311086@u.nus.edu | Windows 10/CLion
Lim Jin Hao | 84980142 | e0425801@u.nus.edu | Windows 10/CLion
Ang Wei Heng Kendrick | 83606608 | kendrick.ang@u.nus.edu | Windows 10/CLion
Sean Lum | 87331587 | e0406739@u.nus.edu | macos /CLion
