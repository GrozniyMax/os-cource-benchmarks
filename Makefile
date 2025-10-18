# BUILDS
disk_build:
	gcc -O0 -Wall -pthread -o disk-thpt-write io/main.c io/disk_thpt_write.c
factorize_build_no_optimization:
	gcc -O0 -Wall -pthread -o factor factorize/main.c factorize/factorize.c
factorize_build_optimize:
	gcc -O3 -march=native -flto -o factor factorize/main.c factorize/factorize.c
broken_malloc_build:
	gcc -shared -fPIC -o libmalloc_broken.so extra/broken_malloc.c -ldl
java_build:
	javac -d . extra/HelloWorld.java

# RUN
factorize_default: factorize_build_no_optimization
	./factor 9223372036854775783 2147483647 true
factorize_optimized: factorize_clear factorize_build_optimize
	./factor 9223372036854775783 2147483647 true -t 20
test_factorize: factorize_build_no_optimization broken_malloc_build
	sh extra/run_factorize.sh
test_java: java_build broken_malloc_build
	sh extra/run_java.sh

# CLEAR
disk_clear: disk_build
	rm -f disk-thpt-write
	rm -f disk_thpt_test.dat_*.dat
factorize_clear: factorize_build_no_optimization
	rm -f factor
broken_malloc_clear:
	rm -f libmalloc_broken.so
factor_test_clear: broken_malloc_build factorize_clear

java_clear: broken_malloc_build
	rm -f HelloWorld.class