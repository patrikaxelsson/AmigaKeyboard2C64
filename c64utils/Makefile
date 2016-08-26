# ReadKey Makefile
# Keeping it simple!

AS=ca65
LN=ld65
NAME=testkbd

testkbd: testkbd.asm
	$(AS) -o $(NAME).o -t c64 $(NAME).asm
	$(LN) -o $(NAME).prg -C c64.cfg $(NAME).o

clean:
	$(RM) *.o
	$(RM) *.prg
