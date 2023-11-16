start = Time.now
score100_words = File.readlines("Dictionary.txt")
    .map(&:chomp)
    .select do |line| !line.include?("-") && !line.include?(" ") && !line.include?("=") end
    .select do |line|
        score_sum = 0
        line.chars.each do |c| score_sum += c.ord - 'a'.ord + 1 end
        score_sum == 100
    end
puts score100_words.join(",")
puts "\nwords number: #{score100_words.size}"
puts "        time: #{((Time.now - start) * 1000).round(0)}ms"
