#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
ln -s "$DIR/../typegrind/clangTypegrind/src/typegrind" "$DIR/../vendor/llvm/tools/clang/lib/typegrind" 
ln -s "$DIR/../typegrind/clangTypegrind/include/typegrind" "$DIR/../vendor/llvm/tools/clang/include/typegrind" 