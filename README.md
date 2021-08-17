# Cyborg

A collection of tools that make VIM feel like Org-Mode:

Each tool is (preferably) a standalone executable in order to integrate in as
many setups as possible, but some basic remaps are provided to get you started.

## Cyborg_Tablify

formats a markdown table and makes computations:

```vim
nnoremap <leader>tf vip:!cyborg_tablify<cr>
nnoremap <leader>tc vip:!cyborg_tablify --compute=1<cr>
```

for example: (making sure there is an empty line before and after the table)

```
| A | B | C |
| - 
| 123 | =A1 | = A1 +  0.5 * B1 |
```

`*press <leader>tf*` and the table gets formatted

```
|  A  |  B  |        C         |
|-----|-----|------------------|
| 123 | =A1 | = A1 +  0.5 * B1 |
```

or `*press <leader>tc*` and results appear next to the expressions.

```
|  A  |     B     |            C             |
|-----|-----------|--------------------------|
| 123 | =A1 # 123 | = A1 +  0.5 * B1 # 184.5 |
```

if the result is too long it will be printed on a separate line and referenced
in the cell, this way subsequent cells are not pushed too far out of the
screen.

```
|              A               |        B        |       C        |
|------------------------------|-----------------|----------------|
| =range(1, 15) # range(1, 15) | =list(A1) # <1> | = max(B1) # 14 |
| =range(1, 10) # range(1, 10) | =list(A2) # <2> | = max(B2) # 9  |
<1>: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14]
<2>: [1, 2, 3, 4, 5, 6, 7, 8, 9]
```

You can find out more about this tool in its own folder.

## To be continued ...

that's it for now, but I will be adding more tools as I make them.
