build:
	g++ -std=c++2a  -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread ./*.cpp ./control/*.cpp  -march=native -O3 -o riscv.run 

dist:
	g++ -std=c++2a -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread ./*.cpp ./control/*.cpp -O3 -o riscv_dist.run 



test:
	g++ -std=c++2a -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./tests/*.cpp -o test.run
	./test.run

ci:
	g++ -std=c++2a -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./tests/*.cpp -o test.run
	./test.run --report_format=XML --report_sink=./test_report.xml --report_level=detailed --log_level=all

build_tests:
	g++ -std=c++2a -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./tests/*.cpp -o test.run

build_tests_optimal:
	g++ -std=c++2a -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread ./*.cpp ./tests/*.cpp -O3 -march=native -o test_optimized.run
