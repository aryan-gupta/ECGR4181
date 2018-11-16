Homework 2

Instructions:

Please type your solutions into a document and convert it into a PDF file. Your solution document should contain your name, 
student ID, the course name, and homework number. Please submit your solution PDF via Canvas.
Make reasonable assumptions where necessary and clearly state them!
You may discuss concepts with your classmates. This fosters group learning and improves the class’ progress as a whole. However, 
make sure to submit your own independent and individual solutions.
Combine all files of your submission into a single zip file.
 

Problem 1

Using the same trace of homework1, study the effects of different prefetching policies and report your results based on dineroIV 
simulator, and the provided trace in homework 1. Please use the prefetching capabilities provided in dineroIV (the -tfetch, 
-pfdist and –pfabort switches). Attempt to find the best prefetching policy and discuss why you feel this would be the best 
policy for the given workload. Make sure to explain prefetching policy per each switch (30pts).

 

Problem 2

Write a program as such to detect column-major or row-major memory layout for 2-dimentional array in your system. The suggestion 
is to not use compiler optimization. You can use the example in power point slide. Make sure to run the loop for enough number 
of iterations. For performance evaluation, use gprof. Also use Pin to analyze the effect of column-major and row-major on the 
cache performance (cache hit rate). Report your results and analysis (30pts).

 

 

Problem 3

Please read the provided paper on memory prefetching.

“Cooperative Prefetching: Compiler and Hardware Support for Effective Instruction Prefetching in Modern Processors”

Write one-page analytical review of the paper (single column, single space, font size 11). Your analytical review must contain 
four primary sections:

Summary (Clearly summarize the core idea of the paper and achieved results, DO NOT COPY AND PASTE THE ABSTRACT!!!)
Benefits (Clearly state the benefits of the proposed idea and its impact on computer architecture)
Negatives (Clearly state the negative aspects of the paper. This may include flows in the core idea, experimental/simulation 
results, or unrealistic assumptions)
Make sure to not selectively copy and paste the paper content. The analytical writing should only contain your own wording of 
the paper (40pts).

 

Problem 4 (50 bonus points)

 

Use Pin to create memory trace for Dhrystone and Linpack benchmarks. PinTools provides the instruction of generating memory 
trace. Use your cache simulator (developed in homework 1) to create the following cache model:

 

Data cache size: 16KB

Cache block size: 32B

Associativity: 4-way

 

Then, develop a stride prefetcher module next to the cache, with the following configurations:

 

Prefetcher buffer size: 500B, 1KB, 2KB, 4KB

Prefetching confidence bits: 2 (It prefetches confidence values equal or bigger than 2), 3 (It prefetches confidence values 
equal and bigger than 4).

 

Note: Number of Prefetcher controller entries is equal to Prefetcher buffer size divided by Cache block size.

Use your own desired interval to keep an unused prefetched block in prefetcher buffer.

Evaluate the performance of the cache without prefetcher and with prefetcher with various prefetcher buffer sizes. Can you 
elaborate and identify the percentage of compulsory misses covered by prefetcher? (50pts)
