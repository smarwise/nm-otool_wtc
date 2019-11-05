nm = ft_nm

otool = ft_otool

FLAGS = gcc -Wall -Wextra -Werror

nm_obj = obj/nm/main.o
     
otool_obj = obj/otool/main.o

$(nm) : $(nm_obj)
	make -C libft
	$(FLAGS) -o $(nm) $(nm_obj) libft/libft.a

$(otool) : $(otool_obj)
	make -C libft
	$(FLAGS) -o $(otool) $(otool_obj) libft/libft.a

all : $(nm) $(otool)

$(nm_obj): obj/nm/%.o: nm/%.c
	$(FLAGS) -c nm/$*.c -o $@

$(otool_obj): obj/otool/%.o: otool/%.c
	$(FLAGS) -c otool/$*.c -o $@

clean:
	make clean -C libft/
	rm -f $(nm_obj)
	rm -f $(otool_obj)

fclean: clean
	make fclean -C libft/
	rm -f $(nm)
	rm -rf $(otool)

re: fclean all