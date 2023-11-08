(def start (System/currentTimeMillis))
(def lines (clojure.string/split-lines (slurp "Dictionary.txt")))
(defn calculate-score [line] (reduce + (map #(+ (- (int %) (int \a)) 1) line)))
(def score-100-words (filter #(and (not (clojure.string/includes? % " "))
                                   (not (clojure.string/includes? % "-"))
                                   (not (clojure.string/includes? % "="))
                                   (= (calculate-score %) 100)) lines))
(println score-100-words)
(println "\nwords number: " (count score-100-words))
(println "        time: " (- (System/currentTimeMillis) start) "ms")

; 编码体验
; 加分项：
; 1、没有分号
; 2、可以直接调用Java类库
; 3、读取文本文件很方便
; 4、字符串连接很方便
; 5、代码很简洁，跟Java比少了很多代码
;
; 减分项：
; 1、需要很多圆括号
; 2、调用方法时前面又有点又有斜杠，如：clojure.string/split-lines
; 3、运行符需要放在前面，如：(= (calculate-score %) 100)
;
