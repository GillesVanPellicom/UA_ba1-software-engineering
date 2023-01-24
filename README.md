# Traffic simulation
Archival upload

Made by Jonas Caluwé & Gilles Van pellicom

## Using the simulation

A linux environment is required.

### Compile project
```bash
cmake -Wall -Werror -B build -S . -G "Unix Makefiles"
cmake --build ./build --config Debug --target all -j256
```

### Run project
Run the project: `./build/sim`

Test the project: `./build/sim_test`

### Generate documentation
**Run this from inside the `doc` directory**

```bash
doxygen ./Doxyfile
```

### Format source code
**Run this from inside the `src` directory**

```bash
find . -type f \( -name "*.h" -o -name "*.cpp" \) -not \( -path "./lib/gtest/*" -or -path "./lib/mini/*" -or -path "./lib/nlohmann-json/*" -or -path "./lib/pugixml/*" -or -path "./lib/visualizer/*" \) -exec clang-format -i -style=file {} \;
```

### Visualizing
The simulation can be visualized usin Jonathan's excellent visualizer.

https://github.com/jonatcln/pse-traffic-simulation-visualizer
