# Test case naming convention
There are currently 3 types of naming conventions.

# V1
`<test case number>-<simple|xml>.txt`
- e.g `1-simple.txt` - contains SIMPLE source
- e.g `1-xml.txt` - contains corresponding "xml" for same-numbered SIMPLE source

# V2
**Incorrect tests start with a `x-` !**

`[x-]<test case description>.<simple|x>`
- e.g `multi-procedures.simple` - contains SIMPLE source for multiple procedures
- e.g `mult-procedures.x` - corresponding "xml"
- e.g `x-leading-zeroes.simple` - incorrect SIMPLE



# V3
Only relevant to "program" test cases

1. Nesting level - 0/1 (easy), 2 (medium), >=3 (hard)
   1. `N0`, `N1`, `N2`, `N3`
2. Order of nesting - while/while, if/if, if/while, while/if 
   1. `WW`, `II`, `IW`, `WI`
3. Position of nested container - first, last, middle, only statement 
   1. `F`, `M`, `L`

- e.g `n1iim.simple` 
  - one nesting level
  - if-if
  - if-if in middle (sandwiched between other statements).