BUILD_DIR=build
SOURCE_DIR=src
include $(N64_INST)/include/n64.mk

ROMNAME=ultrabrew

$(ROMNAME).z64: CXXFLAGS+=-g -std=c++14 -I$(N64_ROOTDIR)/mips64-elf/include/bullet -faligned-new
$(ROMNAME).z64: MAPCMD=-Wl,-Map
$(ROMNAME).z64: LD=$(N64_CXX)
$(ROMNAME).z64: LDFLAGS=-g -L$(N64_ROOTDIR)/mips64-elf/lib -ldragon -lc -lm -ldragonsys -Wl,--start-group -lBullet3Common -lBulletDynamics -lBulletCollision -lBulletInverseDynamics -lBulletSoftBody -lLinearMath -Wl,--end-group -Wl,-Tn64.ld -Wl,--gc-sections

all: $(ROMNAME).z64

$(BUILD_DIR)/$(ROMNAME).dfs: \
	filesystem/CasioSA76_Vibraphone_FSharp4.wav64 \
	filesystem/CasioSA76_Vibraphone_C3.wav64 \
	filesystem/CasioSA76_Sax_C2.wav64 \
	filesystem/CasioSA76_Vibraphone_FSharp3.wav64 \
	filesystem/CasioSA76_HH_D2.wav64 \
	filesystem/CasioSA76_Sax_FSharp2.wav64 \
	filesystem/CasioSA76_Vibraphone_C4.wav64 \
	filesystem/CasioSA76_Bass_C2.wav64 \
	filesystem/CasioSA76_Tabla1_E2.wav64 \
	filesystem/CasioSA76_Tabla2_DSharp2.wav64 \
	filesystem/CasioSA76_Bass_FSharp2.wav64 \
	filesystem/CasioSA76_Vibraphone_C5.wav64 \
	filesystem/CasioSA76_Wood_C3.wav64 \
	filesystem/CasioSA76_Sax_C3.wav64 \
	filesystem/CasioSA76_Wood_E3.wav64 \
	filesystem/title.spr \
	
$(BUILD_DIR)/$(ROMNAME).elf: \
	$(BUILD_DIR)/test_level.o \
	$(BUILD_DIR)/test_level_2.o \
	$(BUILD_DIR)/sphere.o \
	$(BUILD_DIR)/snow.o \
	$(BUILD_DIR)/totem_mdl.o \
	$(BUILD_DIR)/gameobject.o \
	$(BUILD_DIR)/charactercontroller.o \
	$(BUILD_DIR)/totem.o \
	$(BUILD_DIR)/mushroom.o \
	$(BUILD_DIR)/centi.o \
	$(BUILD_DIR)/main.o \
	$(BUILD_DIR)/model.o \
	$(BUILD_DIR)/physmesh.o \

$(ROMNAME).z64: N64_ROM_TITLE="${ROMNAME}"
$(ROMNAME).z64: $(BUILD_DIR)/$(ROMNAME).dfs

clean:
	rm -rf $(BUILD_DIR) ${ROMNAME}.z64

-include $(wildcard $(BUILD_DIR)/*.d)

.PHONY: all clean
