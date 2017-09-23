.PHONY : all

all :
	@echo action is $(action)
	make -C src $(action)
	make -C Tests $(action)