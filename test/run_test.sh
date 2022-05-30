#!/bin/bash
/snap/clion/190/bin/cmake/linux/bin/cmake --build /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug --target benchmark_test -- -j 9

FOLDER_NAME=benchmark_result
TEST_COUNT=10
export THREAD_COUNT=10

rm -rf ${FOLDER_NAME}
mkdir ${FOLDER_NAME}

test_names=(
#    Hamt_Insert
#    Set_Insert
#    MichaelKVList_Insert
#
#    Hamt_Lookup
#    Set_Lookup
#    MichaelKVList_Lookup
#
#    Hamt_Remove
#    Set_Remove
#    MichaelKVList_Remove
)

for test_name in ${test_names[@]}; do
  for i in $(seq 1 1 ${TEST_COUNT})
  do
     touch ${FOLDER_NAME}/${test_name}.txt
     /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug/benchmark_test --benchmark_out_format=csv --benchmark_out=tmp --benchmark_filter=${test_name}
     sed '12!d' tmp >> ${FOLDER_NAME}/general.txt
     sed '12!d' tmp >> ${FOLDER_NAME}/${test_name}.txt
  done
  printf '\n' >> ${FOLDER_NAME}/general.txt
done


for i in $(seq 1 1 12)
do
  printf "THREAD_COUNT=${THREAD_COUNT}\n"
  for j in $(seq 1 1 10)
  do
    export THREAD_COUNT=${i}
    /home/damire/space/hash_arrkay_mapped_trie/test/cmake-build-debug/benchmark_test --benchmark_out_format=csv --benchmark_out=tmp --benchmark_filter=Hamt_Insert
    sed '12!d' tmp >> ${FOLDER_NAME}/Hamt_Multithreading_Insert.txt
  done
  printf '\n' >> ${FOLDER_NAME}/Hamt_Multithreading_Insert.txt
done
