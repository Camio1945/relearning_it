using Dates

start = Dates.now()
score100_words = []
for line in readlines("Dictionary.txt")
    if !contains(line, " ") && !contains(line, "-") && !contains(line, "=")
        score = sum([Int(c) - Int('a') + 1 for c in line])
        if score == 100
            push!(score100_words, line)
        end
    end
end

println(score100_words)
println("\nWords number:", length(score100_words))
println("        time:", Dates.now() - start)
