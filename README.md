# suduku
This takes a suduku and solves the suduku

The values in the board class take a number 1-9, not 0-9.
This means that when the first element needs to be accesed
It is in location value[1][1] not value [0][0].

Also, when reading in the suduku, for every blank spot,
a -1 is entered into the value matrix. If it is not blank, then
the value of the digit at that location is entered in the matrix
