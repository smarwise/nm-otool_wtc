#include "../includes/nm.h"

void    get_info32(struct symtab_command *sym, t_file *file)
{
    uint64_t i;
    struct nlist *array;
    char *strtable;
    t_symbol32 *symbols;
    char *str;

    array = file->ptr + sym->symoff;
    strtable = file->ptr + sym->stroff;
    symbols = (t_symbol32*)malloc(sizeof(t_symbol32));
    file->head32 = symbols;
    for (i = 0; i < sym->nsyms; i++)
    {
        str = strtable + array[i].n_un.n_strx;
        if (ft_strlen(str) > 0)
        {
            symbols->type = array[i].n_type & N_TYPE;
            symbols->ext = array[i].n_type & N_EXT;
            symbols->sect = array[i].n_sect;
            symbols->name = ft_strdup(str);
            symbols->value = array[i].n_value;
            if (i + 1 == sym->nsyms)
                symbols->next = NULL;
            symbols->next = (t_symbol32 *)malloc(sizeof(t_symbol32));
            symbols = symbols->next;
        }
    }
}

void        get_sect_info32(struct load_command *lc, t_file *file)
{
    struct segment_command	*seg;
	struct section			*sect;
	uint32_t					i;

	seg = (struct segment_command *)lc;
	sect = (struct section *)((void*)seg + sizeof(*seg));
	i = -1;
	while (++i < ppc_64(seg->nsects))
	{
		if (!ft_strcmp((sect + i)->sectname, SECT_TEXT) \
			&& !ft_strcmp((sect + i)->segname, SEG_TEXT))
			file->sect->text = file->sect->index + 1;
		else if (!ft_strcmp((sect + i)->sectname, SECT_DATA) \
			&& !ft_strcmp((sect + i)->segname, SEG_DATA))
			file->sect->data = file->sect->index + 1;
		else if (!ft_strcmp((sect + i)->sectname, SECT_BSS) \
			&& !ft_strcmp((sect + i)->segname, SEG_DATA))
			file->sect->bss = file->sect->index + 1;
		++(file->sect->index);
	}
}

void    handle_32(t_file *file)
{
    int nmcmds;
    struct mach_header *header;
    struct load_command *lc;
    struct symtab_command *sym;
    int     i;

    header = (struct mach_header *)file->ptr;
    nmcmds = header->ncmds;
    lc = (void*)file->ptr + sizeof(*header);
    file->sect = (t_section *)malloc(sizeof(t_section));
    file->sect->bss = 0;
    file->sect->data = 0;
    file->sect->text = 0;
    file->sect->index = 0;
    for (i = 0; i < nmcmds; i++)
    {
        if (lc->cmd == LC_SEGMENT)
            get_sect_info32(lc, file);
        if (lc->cmd == LC_SYMTAB)
        {
            sym = (struct symtab_command *)lc;
            get_info32(sym, file);
            print_symbols32(file);
        }
        lc = (void *)lc + lc->cmdsize;
    }
}