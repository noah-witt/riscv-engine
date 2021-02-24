
#./tests/test.sh <test name> <module name>

g++ -lboost_unit_test_framework ./$2.cpp ./tests/$1.cpp -o $1.test.run && \
./$1.test.run