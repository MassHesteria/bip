bip: *.cpp
	g++ -std=c++17 *.cpp -o bip
	#g++ $< -o bip -O3

test: bip
	@echo
	@./bip tests/a_to_b.ips tests/a.txt -o tests/b_ips.txt
	diff -q tests/b.txt tests/b_ips.txt
	@echo
	@./bip tests/dash_SGL2020.ips tests/SuperMetroid.sfc -o tests/dash_SGL2020_ips.sfc
	diff -q tests/dash_SGL2020.sfc tests/dash_SGL2020_ips.sfc
	@echo
	@./bip tests/a_to_b.bps tests/a.txt -o tests/b_bps.txt
	diff -q tests/b.txt tests/b_bps.txt
	@echo
	@./bip tests/g4_skip.bps tests/SuperMetroid.sfc -o tests/g4_skip_bps.sfc
	diff -q tests/g4_skip.sfc tests/g4_skip_bps.sfc
	@echo

clean:
	rm -f bip tests/*_ips.txt tests/*_bps.txt
	rm -f tests/*_ips.sfc tests/*_bps.sfc