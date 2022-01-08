TITLE_ID = PSREGPROF
TARGET   = RegistryProfiler
OBJS     = main.o
SOURCE_LIB = src/*.cc

LIBS = -lvita2d -lSceDisplay_stub -lSceGxm_stub -lSceSysmodule_stub \
	-lSceCtrl_stub -lScePgf_stub -lSceCommonDialog_stub -lfreetype -lpng -lz -lm -lc \
	-lSceRegistryMgr_stub -lSceIofilemgr_stub -lSceAppMgr_stub -lScePower_stub -lstdc++

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -fno-lto
CXXFLAGS = $(CFLAGS) -I$(PSP2SDK)/include/c++ -fno-exceptions
CXX      = $(PREFIX)-g++
ASFLAGS = $(CFLAGS)

all: $(TARGET).vpk

%.vpk: eboot.bin
	vita-mksfoex -s TITLE_ID=$(TITLE_ID) "Registry Profiler" param.sfo
	vita-pack-vpk -s param.sfo -b eboot.bin \
	--add sce_sys/icon0.png=sce_sys/icon0.png \
	--add sce_sys/livearea/contents/bg.png=sce_sys/livearea/contents/bg.png \
	--add sce_sys/livearea/contents/startup.png=sce_sys/livearea/contents/startup.png \
	--add sce_sys/livearea/contents/vita_docs.png=sce_sys/livearea/contents/vita_docs.png \
	--add sce_sys/livearea/contents/pstv_docs.png=sce_sys/livearea/contents/pstv_docs.png \
	--add sce_sys/livearea/contents/manual.png=sce_sys/livearea/contents/manual.png \
	--add sce_sys/livearea/contents/template.xml=sce_sys/livearea/contents/template.xml \
	--add src/menu_bg.png=data/menu_bg.png \
	--add src/menu_in.png=data/menu_in.png \
	--add src/menu_save.png=data/menu_save.png \
	--add src/menu_boot.png=data/menu_boot.png \
	--add src/menu_font.pgf=data/menu_font.pgf $@

eboot.bin: $(TARGET).velf
	vita-make-fself $< $@

%.velf: %.elf
	vita-elf-create $< $@

$(TARGET).elf: $(OBJS) $(SOURCE_LIB)
	$(CC) $(CXXFLAGS) $^ $(LIBS) -o $@

%.o: %.png
	$(PREFIX)-ld -r -b binary -o $@ $^

clean:
	@rm -rf $(TARGET).vpk $(TARGET).velf $(TARGET).elf $(OBJS) \
		eboot.bin param.sfo