add_test([=[Operations.Multiplication]=]  /home/niten/dev/cpp-test/build-qt/BigNum_test [==[--gtest_filter=Operations.Multiplication]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[Operations.Multiplication]=]  PROPERTIES WORKING_DIRECTORY /home/niten/dev/cpp-test/build-qt SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  BigNum_test_TESTS Operations.Multiplication)
