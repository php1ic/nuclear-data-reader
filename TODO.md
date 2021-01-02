This file contains known bugs and a list of features and additions that may make the program better and/or more usable.

Additions are encouraged, you can either contact the author or implement the change and send a pull request via [github](https://github.com/php1ic/nuclear-data-reader)

## Known Bugs

- The 2012 and 2016 nubase files added isospin, but in a way that can overlap with the spin parity values and the error on half-life.
Two examples are:

```
007 0048   7Bei    26750       30     10980      30     RQ                     3/2-    T=3/2 03               p ?;3He ?;A ?
007 0050   7B      27677       25                            570     ys 140    (3/2-)        03 11Ch32t  1967 p=100
008 0020   8He     31609.68     0.09                         119.1   ms 1.2    0+            05          1965 B-=100;B-n=16 1;B-t=0.9 1
008 0030   8Li     20945.80     0.05                         839.40  ms 0.36   2+            05 10Fl01t  1935 B-=100;B-A=100
008 0038   8Lii    31768        5     10822       5     RQ                     0+      T=2   05
008 0040   8Be      4941.67     0.04                          81.9   as 3.7    0+            05          1932 A=100
008 0048W  8Bei    21568        3     16626       3                            2+  frg T=1      04Ti06e  2004 A~100
```
and
```
011 0050   11B      8667.9      0.4                          stbl              3/2-          12          1920 IS=80.1 7
011 0058   11Bxi   21228        9     12560       9     RQ              T=3/2  1/2+,(3/2+)   12          1963
011 0060   11C     10650.3      0.9                           20.364  m 0.014  3/2-          12          1934 B+=100
011 0068   11Cxi   22810       40     12160      40     RQ                     1/2+    T=3/2 12 71Wa21d  1971 p=?
```
There are isospin values for 195/5511 in 2012 and 205/5625 in 2016.
Currently I'm inclined to simply remove them from the line rather than extract all of the necessary situations in order to correctly parse.


## Things that need to be looked at (Not quite bugs)


- The population of the member [Nuclide::jpi](src/nuclide.cpp#L87) needs to be looked at in relation to isotopes that have many different values/possibilities

## Possible improvements/alterations (in no particular order)

- Use useful exit values if we quit out before completion, e.g. 10=no mass table 11=bad input file, etc...

