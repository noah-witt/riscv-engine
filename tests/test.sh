
#./tests/test.sh <test name> <module name>

g++ -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -O0 ./*.cpp ./tests/$1.cpp -o $1.test.run && \
./$1.test.run --log_level=all --report_level=detailed