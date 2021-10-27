build:
	g++ -std=c++20 -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp  -march=native -O3 -o riscv.run 

dist:
	g++ -std=c++20 -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp -O3 -o riscv_dist.run 

run:
	g++ -std=c++20 -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp -O0 -o riscv_dist.run
	./riscv_dist.run

test:
	@echo "testing"
	bash ./tests/test.sh start start
	bash ./tests/test.sh alu alu
	bash ./tests/test.sh util util
	bash ./tests/test.sh assembler assembler
	bash ./tests/test.sh memory	memorys
	bash ./tests/test.sh registers registers
ci:
	bash ./tests/test.sh * all
