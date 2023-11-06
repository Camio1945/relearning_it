const fs = require("fs") 


const start: number = Date.now();
const score100Words: string[] = fs
  .readFileSync("Dictionary.txt", "utf8")
  .split("\r\n")
  .filter(
    (line: string) =>
      !line.includes("-")
      && !line.includes(" ")
      && !line.includes("="),
  )
  .filter(
    (word: string): boolean =>
      word
        .split("")
        .reduce(
          (sum: number, c: string) =>
            sum + c.charCodeAt(0) - "a".charCodeAt(0) + 1,
          0) === 100
  );
console.log(score100Words.join(","))
console.log(`\nwords number: ${score100Words.length}`)
console.log("        time: " + (Date.now() - start) + "ms")

/*
编码体验：
1、句尾的分号很随意，可以有，也可以没有
2、变量的类型比JS清晰很多
3、编码限制比JS多，这是优点，提升了代码的健壮性
*/