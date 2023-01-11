
LEAKS=$(ps | grep "./minishell" | head -1 | awk {'print $1'})
while true; do leaks $LEAKS; sleep 1; done;