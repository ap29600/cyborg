# Tango

A file tangler for literate programming

tango parses its stdin and writes any block that looks like the following
snippet to the appropriate file.

````
```haskell { file=Main.hs }
module Main where

main :: IO ()
main = putStrLn "Hello, World!"
```
````

you can also enter multiple snippets, and they will be joined into a single file, like so:

````

First we include the necessary headers:

```c { file=factorial.c }
#include <stdio.h>
```

We can now declare and define a factorial function:

```c { file=factorial.c }
unsigned long factorial (unsigned int a ) {
```

the accumulator starts at 1.

```c { file=factorial.c }
    unsigned long acc = 1;
```

The loop multiplies the return value by every integer up to `a`.

```c { file=factorial.c }
    for (unsigned int i = 1; i<=a ; i++)
        acc *= i;
```

finally, we return the accumulator.

```c { file=factorial.c }
    return ret;
}
```
We can now use this function in our main.

```c { file=factorial.c }
int main () {
    printf ("%lu\n", factorial(10));
}
```

````
