#!/usr/bin/bash
echo "Building project...";

mkdir -p build;
cd build; 
cmake ..;
cd ..;

echo "Project build!";
