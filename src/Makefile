

SRCPATH=$(shell pwd)

CFLAGS=
COMMON_MAKEFILE=common.mk
PREPARE_LIST=common tcpConnect tcpServer tcpClient udpConnect udpServer udpClient

TARGET=$(LIBPATH)/$(LIBARCH)/$(LIBNAME)


.PHONY:prepare pureclean clean


all: prepare $(TARGET)

prepare:
	@for path in $(PREPARE_LIST); \
		do \
		cp $(SRCPATH)/$(COMMON_MAKEFILE) $(SRCPATH)/$$path/Makefile; \
		cd $(SRCPATH)/$$path && $(MAKEARCH); \
	done

	@echo 
	@echo "generate lib    file in" $(TARGET)
	@echo "generate header file in" $(INCLUDEPATH)
	@echo 


$(TARGET): 
	$(shell [ -d "$(LIBPATH)/$(LIBARCH)" ] || mkdir $(LIBPATH)/$(LIBARCH))
	@$(CC) -shared -fPIC -o $@ $(OBJPATH)/*.o $(CFLAGS)


pureclean:
	@for path in $(PREPARE_LIST); \
		do \
		cd $(SRCPATH)/$$path && $(MAKEARCH) pureclean; \
	done

clean:
	-@rm -rf $(LIBPATH)/$(LIBARCH)
	@for path in $(PREPARE_LIST); \
		do \
		cd $(SRCPATH)/$$path && $(MAKEARCH) clean; \
	done



