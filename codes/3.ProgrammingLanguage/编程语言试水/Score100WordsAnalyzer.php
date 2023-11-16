<?php
$start = microtime(true);
$lines = explode("\r\n", file_get_contents('Dictionary.txt'));
$lines = array_filter($lines, fn($line) => !strpos($line, '-') && !strpos($line, ' ') && !strpos($line, '='));
$score100Words = array_filter($lines, function ($line) {
    $scoreSum = 0;
    for ($i = 0; $i < strlen($line); $i++) {
        $scoreSum += ord($line[$i]) - ord('a') + 1;
    }
    return $scoreSum == 100;
});
echo implode(",", $score100Words);
echo "\nwords number: " . count($score100Words);
echo "\n        time: " . round((microtime(true) - $start) * 1000) . "ms";
