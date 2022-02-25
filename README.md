# 2.1 Project Report BigQ

## Team Members
This is an individual project. I did it alone.

## Problems with source code
The producer and consumer in `test.cc` were a little buggy. But they worked after I added `return NULL;` to the end of both of those functions to avoid `std::bad_cast` when joining them.

## How to compile and run
Make sure you have the correct `.bin` files and place them in the `heap-files` folder.
Once you have the bin files ready,
```
make clean
make test.out
```
This is compile all the code necessary to execute `test.cc`. A nice way to test for all the outputs is to just run `./runTestCases.sh` after you compile and you'll see the output in `output1.txt`

## Implementation details
In order to implement `BigQ`, I used a `Run` class and some workers to pop records from the pipe and push them onto the priority queue.

#### `Run::UpdateTopRecord();`
Updates the top record of current run.
#### `Record *Run::topRecord;` 
The actual top record of the current run.
#### `void*  WorkerThread(void*  arg);` 
Retrieves records from input pipe, sorts them into runs and pushes them onto the priority queue. Also gets sorted records from priority queue to the output pipe. Most of the work in Project 2 is mainly this one function.
#### `void*  recordQueueToRun(..);`
Takes a sequence of pages of records, builds a run to hold all these pages and pushes the runs onto the priority queue.



## Output( output1.txt )
```
n_nationkey: [7], n_name: [GERMANY], n_regionkey: [3], n_comment: [l platelets. regular accounts x-ray: unusual, regular acco]
n_nationkey: [19], n_name: [ROMANIA], n_regionkey: [3], n_comment: [ular asymptotes are about the furious multipliers. express dependencies nag above the ironically ironic account]
n_nationkey: [23], n_name: [UNITED KINGDOM], n_regionkey: [3], n_comment: [eans boost carefully special requests. accounts are. carefull]
n_nationkey: [22], n_name: [RUSSIA], n_regionkey: [3], n_comment: [ requests against the platelets use never according to the quickly regular pint]
n_nationkey: [10], n_name: [IRAN], n_regionkey: [4], n_comment: [efully alongside of the slyly final dependencies. ]
n_nationkey: [13], n_name: [JORDAN], n_regionkey: [4], n_comment: [ic deposits are blithely about the carefully regular pa]
n_nationkey: [20], n_name: [SAUDI ARABIA], n_regionkey: [4], n_comment: [ts. silent requests haggle. closely express packages sleep across the blithely]
n_nationkey: [11], n_name: [IRAQ], n_regionkey: [4], n_comment: [nic deposits boost atop the quickly final requests? quickly regula]
n_nationkey: [4], n_name: [EGYPT], n_regionkey: [4], n_comment: [y above the carefully unusual theodolites. final dugouts are quickly across the furiously regular d]
 consumer: removed 25 recs from the pipe
****************************************************************************************************************************************************************************************
 producer: opened DBFile heap-files/customer.bin
 producer: inserted 150000 recs into the pipe
 consumer: removed 150000 recs from the pipe
****************************************************************************************************************************************************************************************
o_orderkey: [4515876], o_custkey: [100685], o_orderstatus: [F], o_totalprice: [510062], o_orderdate: [1993-11-02], o_orderpriority: [4-NOT SPECIFIED], o_clerk: [Clerk#000000105], o_shippriority: [0], o_comment: [carefully accounts: slyly ironic pinto beans detect. brave somas a]
o_orderkey: [2185667], o_custkey: [51796], o_orderstatus: [F], o_totalprice: [511360], o_orderdate: [1992-10-08], o_orderpriority: [1-URGENT], o_clerk: [Clerk#000000574], o_shippriority: [0], o_comment: [. deposits wake quickly unusual deposits. express dependencies wake. ]
o_orderkey: [2199712], o_custkey: [66790], o_orderstatus: [O], o_totalprice: [515532], o_orderdate: [1996-09-30], o_orderpriority: [2-HIGH], o_clerk: [Clerk#000000650], o_shippriority: [0], o_comment: [ the final, ironic deposits inte]
o_orderkey: [3586919], o_custkey: [24049], o_orderstatus: [F], o_totalprice: [522644], o_orderdate: [1992-11-07], o_orderpriority: [1-URGENT], o_clerk: [Clerk#000000924], o_shippriority: [0], o_comment: [are alongside of the pending deposits. quick]
o_orderkey: [2232932], o_custkey: [13940], o_orderstatus: [O], o_totalprice: [522721], o_orderdate: [1997-04-13], o_orderpriority: [2-HIGH], o_clerk: [Clerk#000000245], o_shippriority: [0], o_comment: [lithely stealthy accounts are slyly against the pend]
o_orderkey: [4576548], o_custkey: [108931], o_orderstatus: [O], o_totalprice: [525591], o_orderdate: [1997-12-26], o_orderpriority: [1-URGENT], o_clerk: [Clerk#000000336], o_shippriority: [0], o_comment: [he slyly ironic requests. regular, bold deposits cajol]
o_orderkey: [3043270], o_custkey: [144617], o_orderstatus: [O], o_totalprice: [530604], o_orderdate: [1997-02-12], o_orderpriority: [5-LOW], o_clerk: [Clerk#000000699], o_shippriority: [0], o_comment: [riously final deposits? ]
o_orderkey: [4722021], o_custkey: [128120], o_orderstatus: [F], o_totalprice: [544089], o_orderdate: [1994-04-07], o_orderpriority: [1-URGENT], o_clerk: [Clerk#000000230], o_shippriority: [0], o_comment: [al, express pinto beans are after the careful]
o_orderkey: [1750466], o_custkey: [21433], o_orderstatus: [F], o_totalprice: [555285], o_orderdate: [1992-11-30], o_orderpriority: [4-NOT SPECIFIED], o_clerk: [Clerk#000000040], o_shippriority: [0], o_comment: [ ironic packages. even notornis integrate. fluffily regular foxes snooze furi]
 consumer: removed 1500000 recs from the pipe
****************************************************************************************************************************************************************************************
n_nationkey: [7], n_name: [GERMANY], n_regionkey: [3], n_comment: [l platelets. regular accounts x-ray: unusual, regular acco]
n_nationkey: [19], n_name: [ROMANIA], n_regionkey: [3], n_comment: [ular asymptotes are about the furious multipliers. express dependencies nag above the ironically ironic account]
n_nationkey: [22], n_name: [RUSSIA], n_regionkey: [3], n_comment: [ requests against the platelets use never according to the quickly regular pint]
n_nationkey: [23], n_name: [UNITED KINGDOM], n_regionkey: [3], n_comment: [eans boost carefully special requests. accounts are. carefull]
n_nationkey: [4], n_name: [EGYPT], n_regionkey: [4], n_comment: [y above the carefully unusual theodolites. final dugouts are quickly across the furiously regular d]
n_nationkey: [10], n_name: [IRAN], n_regionkey: [4], n_comment: [efully alongside of the slyly final dependencies. ]
n_nationkey: [11], n_name: [IRAQ], n_regionkey: [4], n_comment: [nic deposits boost atop the quickly final requests? quickly regula]
n_nationkey: [13], n_name: [JORDAN], n_regionkey: [4], n_comment: [ic deposits are blithely about the carefully regular pa]
n_nationkey: [20], n_name: [SAUDI ARABIA], n_regionkey: [4], n_comment: [ts. silent requests haggle. closely express packages sleep across the blithely]
 consumer: removed 25 recs from the pipe
```
## Gtests
I implemented 4 unit tests with Gtests.
After installing gtests with `sudo apt-get install libgtest-dev`
```
make clean
make gtest
./gtest.out
```
![enter image description here](https://i.ibb.co/BNpWCwJ/Screenshot-2022-02-25-014620.png)

## Debugging
It is super hard to debug `C++`, I had to repeated use valgrind and a bunch of print statements to make it through this one.

