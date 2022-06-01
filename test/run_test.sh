#!/bin/bash
/snap/clion/190/bin/cmake/linux/bin/cmake --build /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug --target benchmark_test -- -j 9

GENERAL_NAME=general.csv
FOLDER_NAME=benchmark_result
TEST_COUNT=100
export INSERT_COUNT=100000
export THREAD_COUNT=12

rm -rf ${FOLDER_NAME}
mkdir ${FOLDER_NAME}

test_names=(
    Hamt_Insert
)

# shellcheck disable=SC2068
for test_name in ${test_names[@]}; do
  printf "THREAD_COUNT=${THREAD_COUNT}, OPERATION_COUNT=${INSERT_COUNT}\n" >> ${FOLDER_NAME}/${GENERAL_NAME}
  printf "THREAD_COUNT=${THREAD_COUNT}, OPERATION_COUNT=${INSERT_COUNT}\n" >> ${FOLDER_NAME}/${test_name}.csv

  for i in $(seq 1 1 ${TEST_COUNT})
  do
     /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug/benchmark_test --benchmark_out_format=csv --benchmark_out=tmp --benchmark_filter=${test_name}
     sed '12!d' tmp >> ${FOLDER_NAME}/${GENERAL_NAME}
     sed '12!d' tmp >> ${FOLDER_NAME}/${test_name}.csv
  done
  printf '\n' >> ${FOLDER_NAME}/${GENERAL_NAME}
done


for i in $(seq 1 1 15)
do
  export THREAD_COUNT=${i}
  printf "THREAD_COUNT=${THREAD_COUNT}\n" >> ${FOLDER_NAME}/Hamt_Multithreading_Insert.txt
  for j in $(seq 1 1 ${TEST_COUNT})
  do
    export THREAD_COUNT=${i}
    /home/damire/space/hash_array_mapped_trie/test/cmake-build-debug/benchmark_test --benchmark_out_format=csv --benchmark_out=tmp --benchmark_filter=Hamt_Insert
    sed '12!d' tmp >> ${FOLDER_NAME}/Hamt_Multithreading_Insert.txt
  done
  printf '\n' >> ${FOLDER_NAME}/Hamt_Multithreading_Insert.txt
done
