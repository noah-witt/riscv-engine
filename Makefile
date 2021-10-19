build:
	g++ -std=c++20 -lboost_log -lboost_program_options -lboost_unit_test_framework -DBOOST_LOG_DYN_LINK -pthread -g ./*.cpp ./control/*.cpp -o riscv.run 

test:
	@echo "testing"
	bash ./tests/test.sh start start
	bash ./tests/test.sh alu alu
	bash ./tests/test.sh util util
	bash ./tests/test.sh assembler assembler
	bash ./tests/test.sh memory	memorys
	bash ./tests/test.sh registers registers
