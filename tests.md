# Compilar el programa
```sh
mkdir -p build
cd build
cmake ..
make
```

# Correr tests
```sh
cd tests/
-- rm -rf build
mkdir -p build
cd build
cmake ..
make runTests
./runTests
```
