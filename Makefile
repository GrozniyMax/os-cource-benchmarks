disk_build:
	gcc -O0 -Wall -pthread -o disk-thpt-write io/main.c io/disk_thpt_write.c
factorize_build_no_optimization:
	gcc -O0 -Wall -pthread -o factor factorize/main.c factorize/factorize.c
factorize_build_optimize:
	gcc -O3 -march=native -flto -o factor factorize/main.c factorize/factorize.c
factorize_default: factorize_build_no_optimization
	./factor 9223372036854775783 2147483647 true
factorize_optimized: factorize_clear factorize_build_optimize
	./factor 9223372036854775783 2147483647 true -t 20

disk_clear: disk_build
	rm -f disk-thpt-write
	rm -f disk_thpt_test.dat_*.dat
factorize_clear: factorize_build_no_optimization
	rm -f factor