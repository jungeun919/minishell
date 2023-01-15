LEAKS=$(ps | grep "./minishell" | grep -v "grep" | head -1 | awk {'print $1'})
while true; do leaks --list $LEAKS ; sleep 1 ; echo -n .; sleep 1 ; echo -n .; sleep 1 ; echo .; done;
