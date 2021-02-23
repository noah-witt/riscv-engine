g++ -lboost_system -lboost_unit_test_framework $1.cpp ../$1.cpp -o $1.test.run
./$1.test.run