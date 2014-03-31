.PHONY: clean All

All:
	@echo ----------Building project:[ QTTest - Debug ]----------
	@"$(MAKE)" -f "QTTest.mk"
clean:
	@echo ----------Cleaning project:[ QTTest - Debug ]----------
	@"$(MAKE)" -f "QTTest.mk" clean
