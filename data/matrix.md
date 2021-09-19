# DATA SYSTEMS PROJECT PHASE 0 - REPORT


## Team 45
* Thota Gokul Vamsi (2019111009)
* Nirmal Manoj C (2019111011)



### Page layout to store matrices (Non-sparse matrices)

* Each matrix is viewed as a vector of elements. 

* Every page would store a part of a matrix as a vector, where each entry would correspond to a cell value.

* These cell values are stored across different pages inside a vector corresponding to each page, in a row-major order, i.e, all the elements of the matrix are initially listed in row-major order, and are stored chunk-by-chunk serially in pages (the size of this chunk is fixed, based on number of elements that can fit in a page).



### Compression technique used for sparse matrices and the corresponding page layout

* The implementation for storage of sparse matrices is done via an ordered map (from STL). 

* Each page contains a map, where the key corresponds to the current cell number (calculated based on listing in row-major order), and the value corresponds to its corresponding value in the matrix. Only cells with non-zero values are stored.

* The number of key-value pairs combined over all pages would be same as number of non-zero cells in the whole matrix.



### Compression ratio

* A matrix is considered to be sparse iff at least 60% of the matrix consists of 0s. Lets consider the percentage of number of zeroes in the matrix to be X %. Hence, we understand that X >= 60.

* Based on the compression technique used for storing matrices, we are storing only the non-zero cells of the matrix, and additionally storing the cell number which they occur in.

* Hence, the percentage of non-zero cells would amount to (100 - X) %. But, as we are also storing the cell number in which they occur, 2 times the above size is required for storing the entire matrix. Thus, we are effectively storing (2 * (100 - X)) % of the entire sparse matrix. 

* For bigger values of X, the above percentage would lead to a very significant amount of compression.



### In-place TRANSPOSE operation

* Initially, when we are storing the entire matrix in disk blocks, we keep track of the highest cell number among all the elements present in the current block. We store this value for each block in serial order, and thus, based on our above implementation, it is obvious that this vector of values would be sorted. This vector is not necessary for non-sparse matrices, but useful in case of sparse matrices.

* Note that we transpose only for cells where row number < column number, so that the swapping during transpose doesn't occur twice.


#### Non-sparse matrices


* We traverse through the entire matrix block by block in serial order.

* We load the block with current cell number into memory.

* For the current cell number, we compute the corresponding cell number with which this cell will swap its value when a transpose operation occurs. This block number in which this swap-cell is present can be efficiently computed in constant time as we are storing the tuples in serial order. Further the offset can be computed because of the same reason.

* The above computed block is also moved to main memory. As the required 2 blocks (for the current cell's transpose) are in the memory, we simply swap the elements in these 2 positions, and appropriately write the blocks back to disk (the first block is written to disk only when elements in it are processed).

* This traversal is done for all the cells with row number < column number, and thus we have obtained the transposed matrix, which was transposed in-place.


#### Sparse matrices


* We traverse through the entire matrix block by block in serial order.

* We load the block with current cell number into memory.

* For the current cell number, we compute the corresponding cell number with which this cell will swap its value when a transpose operation occurs. This block number in which this swap-cell is present can be efficiently computed via binary search on the above stored vector. Here, we can't compute this swap-cell in constant time like in previous case, because we are not storing all the entries (cells with zeroes are omitted out).

* Once the block number of the swap-cell is found, it is loaded into memory. Now, we can find the element with the required cell number inside this block efficiently, as we have used an ordered map (efficient in retrieving key-value, in this case, swap-cell).

* After this swap-cell was found (it implies it had a non-zero value too!), then we use the same method as in non-sparse matrices and swap these 2 cell values in the key-value pairs, in both these ordered maps. This effectively means that the transpose operation has occurred with respect to these cells.

* If this swap-cell was not found in the position it was supposed to be, it clearly implies that it was omitted out, i.e, it was a zero value. Thus, now the current cell would become 0 and this omitted cell would have non-zero contents of the current cell. Thus, we create an entry in the ordered map of second block, indicating that this swap-cell would now have a non-zero value. Also, the current cell entry would be deleted from the first block's ordered map, to ensure transpose occurred correctly.

* This traversal is done for all the cells with row number < column number, and thus we have obtained the transposed matrix, which was transposed in-place.


### LOAD OPERATION 

* We traverse through the matrix like a normal table (similar to blockify() of tables), in a row-major order. We iterate through each element one by one, and store it in corresponding data structure (ignoring zero values in sparse matrices case). When a fixed limit per block is reached, we simply proceed to storing in the next page, by saving the previous one (like in blockify() of tables).

### PRINT OPERATION

* min(n, 20) rows are printed like in tables, with the help of cursor object (which traverses differently for matrices compared to tables, depending on sparsity too).

* For sparse matrices, we iterate from current non-zero element cell number to next non-zero element cell number, and print zeroes for values in between.

### EXPORT OPERATION

* Similar to export of tables and print of matrices. They are just traversed differently based on data structure (cursor).


