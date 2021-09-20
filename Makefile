test:
	@echo "testing"
	bash ./tests/test.sh util util
	bash ./tests/test.sh assembler assembler
	bash ./tests/test.sh memory	memorys
	bash ./tests/test.sh registers registers
