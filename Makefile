test:
	@echo "testing"
	bash ./tests/test.sh assembler assembler
	bash ./tests/test.sh memory	memory
	bash ./tests/test.sh registers registers
