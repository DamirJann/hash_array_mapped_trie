rm -f perfomance.txt
test_count=100
echo ${test_count}
for i in {1..100}
do
   /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug/benchmark_test --benchmark_out_format=csv --benchmark_out=tmp --benchmark_filter=Hamt_Insert
   sed '12!d' tmp >> perfomance.txt
done


# shellcheck disable=SC2051
for i in {1..100}
do
   /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug/benchmark_test --benchmark_out_format=csv --benchmark_out=tmp --benchmark_filter=Set_Insert
   sed '12!d' tmp >> perfomance.txt
done