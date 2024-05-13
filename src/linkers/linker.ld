OUTPUT_FORMAT(elf64-x86-64)
ENTRY(start)
PHDRS 
{
  text PT_LOAD FLAGS(5);
  data PT_LOAD FLAGS(6);
  bss PT_LOAD FLAGS(6);
}
SECTIONS
 {
   . = 1M;
   .text BLOCK(4K) : ALIGN(4K)
   {
       *(.multiboot)
       *(.text)
   } :text
   .data : { *(.data) } :data
   .bss  : { *(.bss)  } :bss
 }
