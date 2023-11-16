start <- Sys.time()
lines <- readLines("Dictionary.txt", encoding = "UTF-8")
lines <- lines[grepl("^[a-z]+$", lines)]
score100Words <- NULL
a <- as.numeric(charToRaw("a"))
for (line in lines) {
  charArray <- strsplit(line, "")[[1]]
  sum <- 0
  for (c in charArray) {
    sum <- sum + as.numeric(charToRaw(c)) - a + 1
  }
  if (sum == 100) {
    score100Words <- c(score100Words, line)
  }
}
print(score100Words)
print(paste("words number:", length(score100Words)))
print(paste("        time:", round((Sys.time() - start) * 1000, 0), "ms"))
