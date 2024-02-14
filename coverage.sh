cd $1
gcov BigNum.cpp.gcno 
lcov --capture --directory CMakeFiles/BigNum.dir/src --output-file gtest_coverage.info
genhtml gtest_coverage.info --output-directory code_coverage 
cd code_coverage
open index.html
