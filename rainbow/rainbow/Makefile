LIBPS4 := ../libPS4
TEXT   := 0x926200000
DATA   := 0x926300000

CC     := gcc
AS     := gcc
OBJCOPY:= objcopy
ODIR   := build
SDIR   := .
IDIRS  := -I$(LIBPS4)/include -I.
LDIRS  := -L$(LIBPS4) -L.
CFLAGS := $(IDIRS) -O2 -std=c11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fPIE
SFLAGS := -nostartfiles -nostdlib -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fPIE
LFLAGS := $(LDIRS) -Xlinker -T $(LIBPS4)/linker.x -Wl,--build-id=none -Wl,--gc-sections
LIBS   := -lPS4

CFILES := $(wildcard $(SDIR)/*.c)
SFILES := $(wildcard $(SDIR)/*.s)
OBJS   := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))

TARGET = payload.bin

$(TARGET): $(ODIR) $(OBJS)
	$(CC) $(LIBPS4)/crt0.s $(ODIR)/*.o -o temp.t $(CFLAGS) $(LFLAGS) $(LIBS)
	$(OBJCOPY) -O binary temp.t $(TARGET)
	rm -f temp.t

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir $@

.PHONY: clean
clean:
	rm -f $(TARGET) $(ODIR)/*.o
