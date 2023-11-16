$elapsedTime = (Measure-Command {
    $Dictionary = Get-Content -Path "Dictionary.txt"
    $score100Words = @()
    foreach ($line in $Dictionary) {
        if ($line -notmatch "-| |=|\\d") {
            $scoreSum = 0
            foreach ($c in $line.ToCharArray()) {
                $scoreSum += [int][char]$c - [int][char]'a' + 1
            }
            if ($scoreSum -eq 100) {
                $score100Words += $line
            }
        }
    }
    $score100WordsStr = $score100Words -join ","
    Write-Output $score100WordsStr
    Write-Output "`nwords number: $($score100Words.Count)"
}).TotalMilliseconds
$elapsedTime = [math]::Round($elapsedTime, 0)
Write-Host "        time: $elapsedTime ms"
