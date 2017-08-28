
include config.mk

SOURCE=$(wildcard *.c)
INCLUDEFILE=$(wildcard *.h)
BUILD=$(patsubst %.c,$(OBJPATH)/%.o,$(SOURCE))

STARTLINE=$(shell sed -n "/\/\/start-private/=" $(INCLUDEFILE))
ENDLINE=$(shell sed -n "/\/\/end-private/=" $(INCLUDEFILE))


.PHONY: pureclean clean generate

all:generate

$(BUILD):$(SOURCE) $(INCLUDEFILE)
	@$(CC) -Wall -fPIC -c $(SOURCE) $(INCLUDE) $(CFLAGS)
	@mv *.o $(BUILD)

	
generate: $(BUILD)
	$(shell cp *.h $(INCLUDEPATH))
	$(shell [ -n "$(STARTLINE)" ] && \
		[ -n "$(ENDLINE)" ] && \
		sed -i '$(STARTLINE),$(ENDLINE)d' $(INCLUDEPATH)/$(INCLUDEFILE) )

pureclean:
	-@rm -f $(BUILD)
	-@rm -f Makefile


clean:
	-@rm -f $(BUILD)
	-@rm -f $(INCLUDEPATH)/$(INCLUDEFILE)
	-@rm -f Makefile


