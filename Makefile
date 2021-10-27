build:
	g++ -std=c++20  -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp  -march=native -O3 -o riscv.run 

dist:
	g++ -std=c++20  -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp -O3 -o riscv_dist.run 

run:
	g++ -std=c++20 -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp -O0 -o riscv_dist.run
	./riscv_dist.run

test:
	@echo "testing"
	bash ./tests/test.sh start.cpp start
	bash ./tests/test.sh alu.cpp alu
	bash ./tests/test.sh util.cpp util
	bash ./tests/test.sh assembler.cpp assembler
	bash ./tests/test.sh memory.cpp	memorys
	bash ./tests/test.sh registers.cpp registers
ci:
	bash ./tests/test.sh * all
