#!/bin/bash

start=$(date +%s)
dictionary=$(grep -v '[ -=]' Dictionary.txt | grep -v '^\s*$' | grep -E '^[a-z]{5,12}')
score100Words=()
a=$(printf '%d' "'a")
for word in $dictionary; do
  word=$(echo $word | tr -d '\r\n')
  char_array=($(echo $word|sed  's/\(.\)/\1 /g'))
  score=0
  for (( i=0; i<${#char_array[@]}; i++)); do
    c=$(printf '%d' "'${char_array[$i]}")
    score=$((score+c-a+1))
  done
  if [[ $score == 100 ]]; then
    score100Words+=("$word")
  fi
done
echo "${score100Words[@]}"
echo "words number: ${#score100Words[@]}"
end=$(date +%s)
elapsed=$((end - start))
echo "        time: $elapsed s"
