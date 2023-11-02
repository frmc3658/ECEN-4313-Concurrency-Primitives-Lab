# Lab 2 - Frank McDermott
### Code Style ###
For this lab I designed two abstract base-classes which serve as class hierarchies for the locks and barriers. There are a couple of advantages to designing the code in such a way:
1) The base class provides a common interface, with which each derived class is designed around. Even though each lock/barrier class is different under-the-hood, we can override common interface functions and abstract away the implementation details and make the process of using the derived classes much simpler. 
2) Having only pure virtual functions means the base classes cannot be instantiated as objects. However, we can use pointers to the base classes in order to construct any derived class. This makes it simpler to write code that is compatible with different lock/barrier classes. Pointers to the base class can be passed around and used in any instance in which a derived class can be expected.
3) For the normal pthread locks/barriers, wrapper classes can be used (also derived).

The disadvantages with this approach:
1) Much more boiler-plate code.
2) In keeping with my usual design of separate translation units, this produces many files, all of which need to be linked and handled by the makefile.

I also encapsulated all classes within the `primitive` namespace in order to avoid any potential name clashes. While this is unlikely, it was necessary for the counter program's fork/join functions, since I threw it together "fast and dirty".


### Challanges ###
The makefile was quite a challange. Having no experience with makefiles, outside of this class, I'm learning as I go. For this lab the main challange was having the makefile create multiple programs simultaneiously. Given the large number of files, and that I based the `mysort ` program on my code from lab 1, it was rather painful to ensure all dependencies we're handled for the two programs. Granted, had I hardcoded the dependencies it likely would have been much easier; but I had already make the makefile dynamic and wanted to retain that, in the event that the next lab would be based on this one.

### Files ###
#### Class Hierarchies ####
As stated previously, each class has it's own translation unit. The Lock and Bar classes serve as abstract-base classes and they pretty much set the common interface for the derivec classes. Each type of lock/barrier class inherits from the corresponding base class, overriding the common interface functions (lock/unlock for Lock classes; init/wait for Bar classes) and abstracting away their internal implementaions. These class translation units represent the the bulk of the files.

#### Parser Class ####
I created a parser class which handles, as the name implies, the parsing of the command line. This is mainly a hold-over from lab 1, just in class form. This is mainly a quality-of-life class which provides code encapsulation and keeps things divided nicely.

#### BucketSort Class ####
I created a BucketSort class to encapsulate the bucketsort algorithm, which I "cheesed" a little bit. The bucketsort algorithm divides the input vector into buckets for each thread to sort. Once the threads finished sorting (after all reaching the barrier) The `buckets` are inserted into an (ordered) std::set, to handle the `merge` step, before being pushed back into a vector for outputing. While we were permitted to leverage `std::set`, I couldn't figure out the merge step, and this was a simple solution that would allow me to move on with the rest of the lab. Undoubtedly, this has a (positive) effect on the runtime of the algorithm since it provides a commonality between the different lock/barrier mechanisms.

#### Timer Class ####
The Timer class is, as the name suggests, used for timing the runtime of the algoritm. It's used for both the `mysort` and `counter` programs.

#### Main Driver Code ####
The `mysort` program uses the `main.cpp` and `main.h` files as driver code, while `counter` uses `counter.cpp` and `counter.h`. The driver code handles the the parsing of the command-line, fork/join parallelism, and handling I/O files.


### Outstanding Bugs ###
The `Parse::selectLockType()` and `Parse:: selectBarrierType()` functions have branches which are intended to work with unimplemented classes of locks/barriers which set the `Lock*` or `Bar*` to nullptr. There isn't any check to validate against these unimplemented lock/barrier classes, so if you were to run either program with an unsupposed lock/barrier it would still run and likely segfault. Ideally, this would be validated in the parser, but I didn't have time to get it done. Though, looking closer at those functions, I suppose if you simply comment out the cases that aren't implemented it would simply default to the pthread wrapper class, effectively eliminating this bug.


### Performance ###
#### ./mysort ####
For testing performance of `mysort`, I tested all locks with `pthread` barriers; and all barriers with `pthread` locks. I also opted to run `mysort` on my `tests/medium.txt` test file. This test file has `300,000` values to sort. In my view, this provides a nice control to the testing methodology, while also minimizes the time spent waiting for `OFSTREAM` to write the sorted data to an output file on my local machine (which isn't being measured).


| **Locks: mysort**|
|-------------------------------------------------------------------------------------------------|
| Primitive    | Runtime (ns) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|--------------|------------------|--------------------------|---------------------|
| MCS          |    94005512  |       8.182      |           2.26           |        8599         |
| pthread      |   357914148  |      22.676      |           2.08           |        8597         |
| TAS          |   219589621  |       4.878      |           1.99           |        8601         |
| Ticket       |   266184649  |      17.460      |           0.72           |        8598         |
| TTAS         |   178347512  |      12.196      |           1.06           |        8600         |
|-------------------------------------------------------------------------------------------------|


| **Barriers: mysort**|
|-------------------------------------------------------------------------------------------------|
| Primitive    | Runtime (ns) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|--------------|------------------|--------------------------|---------------------|
| Sense        |   418815167  |      19.790      |           1.01           |        8602         |
| pthread      |   357914148  |      22.676      |           2.22           |        8597         |
|-------------------------------------------------------------------------------------------------|



#### ./counter ####
For testing the performance of `counter`, I choose a `numIterations` count of `30,000`.

| **Locks: Counter**|
|-------------------------------------------------------------------------------------------------|
| Primitive    | Runtime (ns) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|--------------|------------------|--------------------------|---------------------|
| MCS          |      54477   |     23.848       |          2.98            |       140           |
| pthread      |    3880382   |     10.002       |          1.76            |       137           |
| TAS          |    4369270   |      3.723       |          3.24            |       139           |
| Ticket       |    4565664   |      4.514       |          0.87            |       140           |
| TTAS         |    1916724   |      8.188       |          1.48            |       139           |
|-------------------------------------------------------------------------------------------------|


| **Barriers: Counter**|
|-------------------------------------------------------------------------------------------------|
| Primitive    | Runtime (ns) | L1 cache hit (%) | Branch Pred Hit Rate (%) | Page Fault Count (#)|
|--------------|--------------|------------------|--------------------------|---------------------|
| Sense        |     8617855  |      2.637       |          0.55            |       136           |
| pthread      |  3588929230  |      8.999       |          2.91            |       139           |
|-------------------------------------------------------------------------------------------------|


#### Analysis ####
It's clear from the tables that the MCS lock appears to be the most efficient by several orders of magnitude, with respect to `mysort`, and atleast a order of magnitude with `counter`. This is likely due to less contention between threads, compared to the the other locks. They also tend to have lower a lower memory overhead, compared to, for example, pthread locks, which likely contributes to the better cache performance (with respect to `mysort`). MCS locks also utilize "local spinning" instead of having to share memory resources for lock aqcuisition, which also likely results in better performance.