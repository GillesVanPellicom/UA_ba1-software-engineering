# Description of the test files

### test0.xml
- OutputValmapNotEmpty0: Tests that the returned map is not empty
- ExpectedOutputCompare0: Compares the outputted map against a predefined map

### test1.xml
- ExpectedOutputCompare1: Compares the outputted map against a predefined map

### test2.xml
- ExpectedEmptyValmapOnFail: Checks that the ValMap is empty when it encounters a non-valid xml
- ExpectedErrorMessageCompare0: Compares the error message output against a predefined output

### test3.xml
- ExpectedErrorMessageCompare1: Compares the error message output against a predefined output

### test4.xml
- ExpectedErrorMessageCompare2: Compares the error message output against a predefined output

### test5.xml
- ExpectedErrorMessageCompare3: Compares the error message output against a predefined output

### test6.xml
- ExpectedErrorMessageCompare4: Compares the error message output against a predefined output

### test7.xml
- ExpectedErrorMessageCompare5: Compares the error message output against a predefined output

### test8.xml
- ExpectedErrorMessageCompare6: Compares the error message output against a predefined output

### test9.xml
- ExpectedValmapNotEmpty1: Tests that the returned map is not empty (Checks that all vehicle and vehicle generator types are allowed)

### test10.xml
- ExpectedErrorMessageCompare7: Compares the error message output against a predefined output (type of vehicle and generator is not set)

### test11.xml
- ExpectedErrorMessageCompare8: Compares the error message output against a predefined output (different types of non existent vehicles are given)

### test12.xml
- OutputValmapNotEmpty2: Tests that the returned map is not empty (busstops)

### test13.xml
- OutputValmapNotEmpty3: Tests that the returned map is not empty (busstops with optional attributes)

### test14.xml
- ExpectedErrorMessageCompare9: Compares the error message output against a predefined output (multiple definitions of the same attribute)

### test15.xml
- ExpectedErrorMessageCompare10: Compares the error message output against a predefined output (incorrect attributes for busstops).

### test16.xml
- ExpectedErrorMessageCompare11: Compares the error message output against a predefined output (incomplete attributes for busstops).

### test17.xml
- ExpectedErrorMessageCompare12: Compares the error message output against a predefined output (types of the attributes of busstop)

### test18.xml
- ExpectedErrorMessageCompare13: Compares the error message output against a predefined output (incorrect attributes for crossroads)
 
### test19.xml
- OutputValmapNotEmpty4: Tests that the returned map is not empty (crossRoads)

### test20.xml
- ExpectedErrorMessageCompare14: Compares the error message output against a predefined output (incorrect types for attributes of crossroads)

### test21.xml
- ExpectedErrorMessageCompare15: Compares the error message output against a predefined output (same road twice)

### test22.xml
- ExpectedErrorMessageCompare16: Compares the error message output against a predefined output (road that does not exist and outside road positions)

### test23.xml
- ExpectedErrorMessageCompare17: Compares the error message output against a predefined output (lights inside eachothers brake distance)

### test24.xml
- OutputValmapNotEmpty5: Tests that the returned map is not empty (crossroads with lights)

### test25.xml
- ExpectedErrorMessageCompare18: Compares the error message output against a predefined output (lights at crossroads)