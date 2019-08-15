## step1: results and analysis without thread ##
* data source: data_small(because data directory is too large,the memory fail to support it) *
## Building the OffsetIndex vs ##
The datas got are as follows:
Building the OffsetIndex
Parsing and indexing files...
Took 0.217551 microseconds to parse 24 files.
21397 entries in the index.
Took 0.217683 seconds to execute.
Memory usage:
Cur Real Mem: 26872     Peak Real Mem: 26872     Cur VirtMem: 178776    PeakVirtMem: 219724

Building the TypeIndex
10 entries in the index.
Took 0.073950 seconds to execute.
Memory usage:
Cur Real Mem: 33780     Peak Real Mem: 33780     Cur VirtMem: 186828    PeakVirtMem: 219724
Cur Real Mem: 33780     Peak Real Mem: 33780     Cur VirtMem: 186828    PeakVirtMem: 219724

# Analysis for building offset and type index #
From the data above, I find that the real memory of TypeIndex is larger than 
OffsetIndex.I think that's because TypeIndex use SetofMovies which have list 
of movie structs, so it have more memory to use.
### Querying seattle movies based on OffsetIndex ###
Querying seattle movies based on OffsetIndex
returning movieset
there is no such term
        There are0
Took 0.000412 seconds to execute.
Memory usage:
Cur Real Mem: 26872     Peak Real Mem: 26872     Cur VirtMem: 178776    PeakVirtMem: 219724

## querying movies have Seattle in the title and are Rom/Coms based on OffsetIndex ##
OffsetIndex querying movies have Seattle in the title and are Rom/Coms
returning movieset
Getting docs for movieset term: "seattle"
set begin
lookup end
there is no such term
There are 0 files!
Took 0.000446 seconds to execute.
Memory usage:
Cur Real Mem: 26872     Peak Real Mem: 26872     Cur VirtMem: 178776    PeakVirtMem: 219724
Destroyed OffsetIndex
Cur Real Mem: 25864     Peak Real Mem: 26872     Cur VirtMem: 178776    PeakVirtMem: 219724

## Querying Action movies based on typeIndex ##
There are 162 action movies!
Took 0.003222 seconds to execute.
Memory usage:
Cur Real Mem: 31740     Peak Real Mem: 31740     Cur VirtMem: 186828    PeakVirtMem: 219724
Destroyed typeIndex
Cur Real Mem: 31740     Peak Real Mem: 31740     Cur VirtMem: 186828    PeakVirtMem: 219724

## Querying movies have Seattle in the title and are Rom/Coms based on typeindex ##
Querying movies have Seattle in the title and are Rom/Coms based on typeindex
run query type genre
returning movieset
get set of movies
There are 0 files!
Took 0.000807 seconds to execute.
Memory usage:
Cur Real Mem: 33780     Peak Real Mem: 33780     Cur VirtMem: 186828    PeakVirtMem: 219724
Destroyed typeIndex
Cur Real Mem: 33780     Peak Real Mem: 33780     Cur VirtMem: 186828    PeakVirtMem: 219724


## Analysis for querying ##
OffsetIndex is quicker than TypeIndex and has smaller memory.It may be because OffsetIndex only store the 
docid and rowid of the files, whereas TypeIndex need more time and memory to search out the results.

## Step2: Comparison between multithread and nothread ##
### Conclusion from the comparison of querying: ###
The multi-thread seems to be quicker than querying without threads,that's because for multithread,I think that's because they can access the file at the same time so it cost less time without conficts using locking and unlocking.
Besides, the memory used in multi-threading version is less than without multi-tnreading.I think that's because the they can read those files at the same time and store the data in buffer cache instead of hard disk.

### Multithreads results and data ###
### Building the OffsetIndex ###
Building the OffsetIndex
Parsing and indexing files...
21397 entries in the index.
Took 0.196661 seconds to execute.
Memory usage:
Cur Real Mem: 26828     Peak Real Mem: 26828     Cur VirtMem: 178776    PeakVirtMem: 219724

Building the TypeIndex
10 entries in the index.
Took 0.060583 seconds to execute.
Memory usage:
Cur Real Mem: 31740     Peak Real Mem: 31740     Cur VirtMem: 186828    PeakVirtMem: 219724

## Querying seattle movies based on OffsetIndex ##
Querying seattle movies based on OffsetIndex
returning movieset
Getting docs for movieset term: "seattle"
set begin
lookup end
there is no such term
        There are0
Took 0.000457 seconds to execute.
Memory usage:
Cur Real Mem: 26828     Peak Real Mem: 26828     Cur VirtMem: 178776    PeakVirtMem: 219724

## querying movies have Seattle in the title and are Rom/Coms based on OffsetIndex ##
OffsetIndex querying movies have Seattle in the title and are Rom/Coms
returning movieset
Getting docs for movieset term: "seattle"
There are 0 files!
Took 0.000508 seconds to execute.
Memory usage:
Cur Real Mem: 26828     Peak Real Mem: 26828     Cur VirtMem: 178776    PeakVirtMem: 219724
Destroyed OffsetIndex
Cur Real Mem: 23824     Peak Real Mem: 26828     Cur VirtMem: 178776    PeakVirtMem: 219724

## Querying movies have Seattle in the title and are Rom/Coms based on typeindex ##
There are 0 files!
Took 0.000743 seconds to execute.
Memory usage:
Cur Real Mem: 31740     Peak Real Mem: 31740     Cur VirtMem: 186828    PeakVirtMem: 219724
Destroyed typeIndex
Cur Real Mem: 31740     Peak Real Mem: 31740     Cur VirtMem: 186828    PeakVirtMem: 219724
