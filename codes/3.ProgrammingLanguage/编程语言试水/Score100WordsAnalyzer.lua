local start = os.clock()
local score100Words = {}
local a = string.byte('a')
for line in io.lines("Dictionary.txt") do
    if not line:find("-") and not line:find(" ") and not line:find("=") then
        local scoreSum = 0
        for i = 1, #line do
            scoreSum = scoreSum + string.byte(line:sub(i,i)) - a + 1
        end
        if scoreSum == 100 then
            table.insert(score100Words, line)
        end
    end
end
print(table.concat(score100Words, ", "))
print("\nwords number: " .. #score100Words)
print("        time: " .. (os.clock() - start) * 1000 .. "ms")

--[[
编码体验：
加分项：
1、不需要引工具包、不需要定义public class、不需要定义main方法
2、不需要分号
3、不需要定义变量类型

减分项：
1、不需要链式编程
2、区分冒号和点，string.byte('a')，line:find("-")
3、代码不够简洁

]]
