build:
	g++ -std=c++20  -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp  -march=native -O3 -o riscv.run 

dist:
	g++ -std=c++20  -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp -O3 -o riscv_dist.run 

run:
	g++ -std=c++20 -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp -O0 -o riscv_dist.run
	./riscv_dist.run

test:
	g++ -std=c++20 -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./tests/*.cpp -o test.run
	./test.run

ci:
	g++ -std=c++20 -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./tests/*.cpp -o test.run
	./test.run --report_format=XML --report_sink=./test_report.xml --report_level=detailed --log_level=all

build_tests:
	g++ -std=c++20 -lboost_log -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./tests/*.cpp -o test.run
