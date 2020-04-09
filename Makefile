flags := -g -O2 -Wall

outdir := out
incdir := include
objdir := objects

target ?= avltree

objs := $(addprefix $(objdir)/, $(patsubst %.c, %.o, $(wildcard *.c)))

target_objs := $(addprefix $(objdir)/, $(patsubst %.c, %.o, \
	$(wildcard $(target)/*.c) \
))

$(objdir)/$(target)/%.o: $(target)/%.c $(incdir)/%.h
	@echo $@
	cc $(flags) -c $< -o $@

$(objdir)/%.o: %.c $(incdir)/%.h
	@echo $@
	cc $(flags) -c $< -o $@

$(outdir)/treeio: $(objs) $(target_objs)
	cc $(flags) $? -o $@

$(objs) ($target_objs): | $(objdir)
$(target_objs): | $(objdir) $(target)

$(objdir):
	mkdir $(objdir)
	mkdir $(objdir)/$(target)

.PHONY: clean
clean:
	rm -f -r $(objdir)
	rm -f $(outdir)/treeio

print: *.c
	lpr -p $?
	touch print
