runbenchmark: buildslow runslow buildfast runfast
buildslow: slowsort.c
        gcc -o slowsort slowsort.c
runslow: slowsort
        ./slowsort
buildfast : fastsort.c
        gcc -o fastsort fastsort.c
runfast: fastsort
        ./fastsort
runwriteslow:slowsort.c
        emacs slowsort.c
runwritefast:fastsort.c
        emacs fastsort.c
