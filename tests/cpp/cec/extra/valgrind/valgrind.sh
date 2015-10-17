R -d "valgrind --tool=memcheck --leak-check=full" --vanilla < test_\demo.R > log_demo.txt 2>&1
