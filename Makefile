BUILD_DIR=build
SOURCE_DIR=src
include $(N64_INST)/include/n64.mk

ROMNAME=ultrabrew

$(ROMNAME).z64: CXXFLAGS+=-std=c++14 -I$(N64_ROOTDIR)/mips64-elf/include/bullet
$(ROMNAME).z64: MAPCMD=-Wl,-Map
$(ROMNAME).z64: LD=$(N64_CXX)
$(ROMNAME).z64: LDFLAGS=-L$(N64_ROOTDIR)/mips64-elf/lib -ldragon -lc -lm -ldragonsys -Wl,--start-group -lBullet3Common -lBulletDynamics -lBulletCollision -lBulletInverseDynamics -lBulletSoftBody -lLinearMath -Wl,--end-group -Wl,-Tn64.ld -Wl,--gc-sections

all: $(ROMNAME).z64

$(BUILD_DIR)/$(ROMNAME).elf: \
	$(BUILD_DIR)/test_level.o \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/model.o \

$(ROMNAME).z64: N64_ROM_TITLE="${ROMNAME}"

clean:
	rm -rf $(BUILD_DIR) ${ROMNAME}.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
