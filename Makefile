flags := -g -O2 -Wall 

outdir := out
incdir := include
objdir := objects

target ?= avltree
targetdir = $(objdir)/$(target)

src := $(wildcard *.c)
target_src := $(wildcard $(target)/*.c) 

objs := $(addprefix $(objdir)/, $(patsubst %.c, %.o, $(src)))
target_objs := $(addprefix $(objdir)/, $(patsubst %.c, %.o, $(target_src)))

$(targetdir)/%.o: $(target)/%.c $(incdir)/%.h
	@echo $@
	cc $(flags) -c $< -o $@

$(objdir)/%.o: %.c $(incdir)/%.h
	@echo $@
	cc $(flags) -c $< -o $@

$(outdir)/treeio: $(objs) $(target_objs)
	cc $(flags) $? -o $@

$(objs): | $(objdir)
$(target_objs): $(target) | $(targetdir)

$(objdir):
	mkdir $(objdir)

$(targetdir):
	mkdir $(targetdir)

.PHONY: clean
clean:
	rm -f -r $(objdir)
	rm -f $(outdir)/treeio

print: *.c
	lpr -p $?
	touch print
