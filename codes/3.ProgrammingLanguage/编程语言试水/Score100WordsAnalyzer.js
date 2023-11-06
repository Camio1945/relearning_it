const fs = require("fs") // node 写法，性能稍差，运行：node Score100WordsAnalyzer.js
// import fs from 'node:fs' // deno 写法，性能更好，运行：deno run --allow-read=. Score100WordsAnalyzer.js

const start = Date.now();
const score100Words = fs
  .readFileSync("Dictionary.txt", "utf8")
  .split("\r\n")
  .filter(
    (line) => 
      !line.includes("-") 
      && !line.includes(" ") 
      && !line.includes("="),
  )
  .filter(
    (word) =>
      word
        .split("")
        .reduce(
          (sum, c) => 
            sum + c.charCodeAt() - "a".charCodeAt() + 1, 
          0) === 100
  );
console.log(score100Words.join(","));
console.log(`\nwords number: ${score100Words.length}`);
console.log("        time: " + (Date.now() - start) + "ms");

/*
编码体验：
1、句尾的分号很随意，可以有，也可以没有
2、有变量类型
3、校验很松散
*/