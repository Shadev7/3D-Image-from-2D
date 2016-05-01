.PHONY = all clean distclean


# Main entry point
#
all:
	@$(MAKE) -f Makefile-render
	@$(MAKE) -f Makefile-segment

clean:
	@$(MAKE) clean -f Makefile-render
	@$(MAKE) clean -f Makefile-segment

