# Quicksort and Insertion Sort Report
## Experiment processing
1.  write code for fastsort and slowsort in VM virtualbox(each with **30 trials**);
2.  run slowsort.c and fastsort.c size by size and record results in excel for analysis;
3.  use python to screen out the **large,medium and small size**,the analysis methods are attached below;
4.  make analysis and conclusions.

**The number of items I sorted:**
   - to test the smallest number that can work:
      `from 1 to 10,then 20, 30, 40, 50,...100`
   - to decide the size for small, medium and large:
      `100, 200, 400, 800, 1600,...12800, 204800(the limit of my computer)`
  
## Strategy for choosing small, medium and large size
1.  Consider the ideal difference between different sizes in each type of sort:
  - for slowsort:
    the ratio of time of different sizes should be close to the ratio of their sizes' square:
                **`T(A)/T(B) ≈ sizeA^2/sizeB^2`**
  - for fastsort:
    the ratio of time of different sizes should be close to:
    
      **`T(A)/T(B) ≈ sizeA/sizeB*(LOG(sizeA,2)/LOG(sizeB,2))` **
                           (based on big O theory)

2.  Consider the ideal difference of running time between two sorts in same size:
  - As the size gets larger,the gap of their running time should gets larger too:
    that is:as size turns from small to large,the running time of slowsort should show up with increasing speed of growth
    compared with fastsort sort.

## Strategy for getting the smallest size that makes sense:
- Consider the running time for each sort type:
  the smallest size is the smallest one(or as smaller as possible) that can go slower than the size smaller than it,
  and faster than the size that is bigger than it;
- Consider the comparison between fast and slow sort:
  the smallest size for both sort should meet the rule that slowsort should run slower than fastsort. 

## Strategy for getting the large size that makes sense:
  - the large size running time's ratio to that of the small and median size should meet the equation as close as
    possible as mentioned above:
    1.  for slowsort:
       - **`T(A)/T(B) ≈ sizeA^2/sizeB^2`**
    2.  for fastsort:
       -  `T(A)/T(B) ≈ sizeA/sizeB*(LOG(sizeA,2)/LOG(sizeB,2))`

## Experiment details and illustrations
- Analysize and decide small(S),medium(M) and large(L) size using excel and python:

  **1.  Screen out L,M and S:**
    - record experiment datas based on slowsort and fastsort algorithms programme(written by C):
         The picture below records part of the results of the size when slowsort is quicker than fastsort,and some of them
      shows that in certain range of size(ususally the size is very small),the running time of bigger size is smaller than 
      that of smaller one:
      
      [Part of the tested data of  small size listed here.]       (https://github.ccs.neu.edu/cs5007seaSpr19/angela0212/blob/master/a2/charts/small_size_compare.PNG)
      
         Below is the results of both sorts from ***100 to 204800***
         
      ![alt text](https://github.ccs.neu.edu/cs5007seaSpr19/angela0212/blob/master/a2/charts/origin_data.PNG)
      
    - Python programme and methods
     - I use python to find couples of sizes that meet the runnning time expectation as close as possible.
      - Below is the data array I used as input:
      
          ![alt text](https://github.ccs.neu.edu/cs5007seaSpr19/angela0212/blob/master/a2/charts/origin_array.PNG)
         
     - I measure the degree of closess to the ideal result by setting variables a and b like the code below:
      
      ```python
         # b is the couple of size that has the running time
         # that is close to the expected ratio.

         # a and b is the deviation to the expected ratio 
         # respectively for slow and fast sort.

         # a and b both start from 1.5,then minus 0.01 step by step
         # until get three different size that meet the running time
         # expectation with each other.
         a = 1.09
         b = 1.3
         B = []
         for i in range(len(A)-1,-1,-1):
             for j in range(0,i,1):
                 if(((A[i][1]/A[j][1]) / (math.pow(A[i][0],2)/math.pow(A[j][0],2))) <= a
                     and ((A[i][2]/A[j][2]) / (A[i][0]/A[i][0] * math.log(A[i][0],2)/math.log(A[j][0],2))) <= b
                   ):
                     B.append((i,j))
         print(B)
      ```
      
     ***Results returned:***
      | [(10, 3), (10, 7), (10, 9), (9, 8), (8, 6), (8, 7), (7, 3), (3, 2), (2, 1)] |
      which means size *800, 12800 and 102400* may meet asymptotic analysis.
   
   2. Results verification
    - for each slow sort or fast sort,the three sizes meet the asymptotic analysis with each other:
    
      ![alt text](https://github.ccs.neu.edu/cs5007seaSpr19/angela0212/blob/master/a2/charts/size_verify.PNG)
      
    - for the comparison of both sorts,the graph below show their increasing trend of running time:
         from the graph,we see that they both meet the **exponential growth trend**.
         
       ![alt text](https://github.ccs.neu.edu/cs5007seaSpr19/angela0212/blob/master/a2/charts/running_time_compare.PNG)
      
      
## Results and conclusion
### Final size for small,medium and large
   - small: 800
   - medium: 12800
   - large: 102400
###  The smallest that makes sense
   From the data of result,I find that before certain range of size,hovering around **100**,both slowsort and fastsort 
   is not stable enough,like when size getting bigger,the running time gets smaller instead,which is undesirable;
      Specially before the size of around **50**,fastsort mainly runs slower than slowsort.Considering both,I think the 
      smallest size that makes sense is 100;
###  Size for large makes sense
   From the increasing trend I obtained from the experiment,basically when size gets larger,they are more likely to meet the 
  expected/analyzed times.So I think the large size is unlimited,but due to my limit ability of laptop,my largest size I 
  tested is 204800.
###  Size of list where insertion sort is usually faster than quicksort
   Based on the different small sizes I tested,I find that when size is **1, 2, 3, 4, 5, 10, 39, 52**,insertion sort is usually faster than quicksort.
###  generated times compare the expected/analyzed times 
   As long as the defined size of small, median and large are set appropriately,the ratio of the three size's running time are proportionable to each other.
- small size tend to make quicksort get to worst running time of **O(N)** and as the size gets larger and larger,it is much more
closer to **O(N)**,the average running time for quicksort  is close to **NLOG(N,2)**;
- Running time of insertion sort is almostly always close to **O(N^2)**;
- Both insertion sort and quicksort satisfy **exponential growth trend** during the growing size from small to large;
- The difference between the running time of insertion sort and quicksort grows faster and faster as the size gets bigger:
  [This is the charts showing the increasing trends of their running time gap](https://github.ccs.neu.edu/cs5007seaSpr19/angela0212/blob/master/a2/charts/gap_trend.PNG)

## The hardest part of this assignment
   Due to computer's limited ability of conputing, CPU processing speed,operating speed and so on,the results may sometimes become unstable,like there are difference of operating time and environment more or little when operating each type of sort using different
sizes,which may increase the random error when testing the running time and become unaccurate;Besides,even though we set srand(time(0))
to make the total random arrays of number for testing to make fairness,it's still impossible to avoid the circumstances when some 
arrays of number are more to the benefit of one type of sort than the other.

## Recommendations and Suggestions
   - When doing such experiments,try to keep the running softwares and processes as few as possible to create a more fair and stable 
   environment for testing;
   - When asked to sort small sizes of numbers,both quicksort and insertion sort works well;
   - As to medium size,we should consider the memory space together with running time,and compare the costs of both running speed and required space to reach the least costs;
   - For large sizes,quicksort is a better way due to the absolute advantage of running speed over insertion sort.
        
    
