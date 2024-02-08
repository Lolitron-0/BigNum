cd build
sudo cmake -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Release -G Ninja .. && sudo cmake --build . --parallel 5 && cp compile_commands.json ../compile_commands.json
# sudo cmake -DCMAKE_CXX_COMPILER=g++ -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug -G Ninja .. && sudo cmake --build . --parallel 5 && cp compile_commands.json ../compile_commands.json
./BigNum_pi $1
cd ..
