
#./tests/test.sh <test name> <module name>

g++ -std=c++2a -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./tests/$1 -o $2.test.run && \
./$2.test.run --log_level=all --report_level=detailed