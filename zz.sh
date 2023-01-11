# export MallocStackLogging=1
LEAKS=$(ps | grep "./minishell" | grep -v "grep" | head -1 | awk {'print $1'})
while true; do leaks --list $LEAKS ; sleep 1 ; echo -n .; sleep 1 ; echo -n .; sleep 1 ; echo .; done;


# minishell에서 minishell 실행시 미니쉘 글자가 두개씩 뜸
# cat << a 에서 컨트롤 C 누르면 오류 
# minishell$ cat << a
# > 1
# > 2
# > 3
# > 
# minishell$ echo $?
# > s
# > d
# > f
# > a
# --- output ---
# 1
# 2
# 3
# echo 0
# s
# d
# f

# 그리고 $? 이 0이 나옴 (배쉬는 1 => 이건 ctl + c로 안끝나서 그런듯)
